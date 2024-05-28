#include "cat.h"

// 노드를 생성하고 디렉토리 트리에 추가하는 함수
void create_new_node(TreeNode *currentDir, const char *filename, size_t fileSize) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strncpy(newNode->name, filename, sizeof(newNode->name));
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->type = 'f';
    newNode->Size = fileSize;
    newNode->User = currentDir->User;
    newNode->Group = currentDir->Group;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    newNode->Year = tm.tm_year + 1900;
    newNode->Month = tm.tm_mon + 1;
    newNode->Day = tm.tm_mday;
    newNode->Hour = tm.tm_hour;
    newNode->Minute = tm.tm_min;
    for (int i = 0; i < 9; i++) {
        newNode->permission[i] = 1; // 기본적으로 모든 권한 부여
    }
    newNode->parents = currentDir;
    newNode->child = NULL;
    newNode->sib = currentDir->child;
    currentDir->child = newNode;
}

// 가상의 파일 시스템에서 파일 내용을 설정하는 함수
void set_file_content(TreeNode *node, const char *content) {
    strncpy(node->data, content, sizeof(node->data) - 1);
    node->data[sizeof(node->data) - 1] = '\0';
}

// 가상의 파일 시스템에서 파일 내용을 가져오는 함수
const char* get_file_content(TreeNode *node) {
    return node->data;
}

void cat(DirectTree* tree, const char* command) {
    char userInput[100];
    char* parts[5];
    int partCount = 0;
    char* nextPart;
    char inputChar;

    strncpy(userInput, command, sizeof(userInput) - 1);
    userInput[sizeof(userInput) - 1] = '\0'; // Null terminate the string

    size_t length = strlen(userInput);
    if (length > 0 && userInput[length - 1] == '\n') {
        userInput[length - 1] = '\0';
    }

    char* part = strtok_r(userInput, " ", &nextPart);
    while (part != NULL && partCount < 5) {
        parts[partCount++] = part;
        part = strtok_r(NULL, " ", &nextPart);
    }

    if (partCount == 0) {
        printf("Error: 명령어와 파일 이름을 입력하세요.\n");
        return;
    }

    TreeNode *currentDir = tree->current;
    if (partCount == 1) {
        // 단순 파일 내용 출력: cat <filename>
        TreeNode *node = Find_Dir(parts[0], 'f', tree);
        if (node == NULL) {
            printf("Error: 파일이 존재하지 않습니다: %s\n", parts[0]);
            return;
        }

        // 가상의 파일 시스템에서 파일 내용 출력
        printf("%s의 내용:\n%s\n", parts[0], get_file_content(node));
    } else if (partCount == 2 && strcmp(parts[0], ">") == 0) {
        // 파일에 내용 쓰기: cat > <filename>
        TreeNode *node = Find_Dir(parts[1], 'f', tree);
        if (node == NULL) {
            // 파일이 존재하지 않으면 새 파일 생성
            node = (TreeNode *)malloc(sizeof(TreeNode));
            strncpy(node->name, parts[1], sizeof(node->name));
            node->name[sizeof(node->name) - 1] = '\0';
            node->type = 'f';
            node->Size = 0;
            node->User = currentDir->User;
            node->Group = currentDir->Group;
            memset(node->data, 0, sizeof(node->data)); // Initialize data to empty
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            node->Year = tm.tm_year + 1900;
            node->Month = tm.tm_mon + 1;
            node->Day = tm.tm_mday;
            node->Hour = tm.tm_hour;
            node->Minute = tm.tm_min;
            for (int i = 0; i < 9; i++) {
                node->permission[i] = 1; // 기본적으로 모든 권한 부여
            }
            node->parents = currentDir;
            node->child = NULL;
            node->sib = currentDir->child;
            currentDir->child = node;
        }

        // 가상의 파일 시스템에서 파일 내용 입력 받기
        printf("입력하세요 (종료: Ctrl+D)\n");
        char buffer[1024];
        size_t totalSize = 0;
        while (fgets(buffer, sizeof(buffer), stdin)) {
            size_t len = strlen(buffer);
            totalSize += len;
            if (totalSize >= sizeof(node->data)) {
                printf("Error: 파일 크기가 너무 큽니다.\n");
                return;
            }
            strcat(node->data, buffer);
        }
        node->Size = totalSize;

        // Flush stdin to clear any remaining characters
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Clear the EOF flag on stdin
        clearerr(stdin);

        printf("파일이 성공적으로 작성되었습니다.\n");
    } else if (partCount == 3 && strcmp(parts[1], ">") == 0) {
        // 파일 복사: cat <source> > <destination>
        TreeNode *sourceNode = Find_Dir(parts[0], 'f', tree);
        if (sourceNode == NULL) {
            printf("Error: 소스 파일이 존재하지 않습니다: %s\n", parts[0]);
            return;
        }

        TreeNode *destNode = Find_Dir(parts[2], 'f', tree);
        if (destNode == NULL) {
            // 새 노드 생성 및 추가
            create_new_node(currentDir, parts[2], sourceNode->Size);
            destNode = Find_Dir(parts[2], 'f', tree);
        }

        // 가상의 파일 시스템에서 파일 내용 복사
        strncpy(destNode->data, sourceNode->data, sizeof(destNode->data) - 1);
        destNode->data[sizeof(destNode->data) - 1] = '\0';
        destNode->Size = sourceNode->Size;
        printf("파일이 성공적으로 복사되었습니다.\n");
    } else if (partCount == 2 && strcmp(parts[0], "-n") == 0) {
        // 번호가 매겨진 파일 내용 출력: cat -n <filename>
        TreeNode *node = Find_Dir(parts[1], 'f', tree);
        if (node == NULL) {
            printf("Error: 파일이 존재하지 않습니다: %s\n", parts[1]);
            return;
        }

        // 가상의 파일 시스템에서 번호가 매겨진 파일 내용 출력
        printf("%s의 내용:\n", parts[1]);
        char *contentCopy = strdup(node->data); // strtok는 원본 문자열을 변경하므로 복사본 사용
        char *line = strtok(contentCopy, "\n");
        int lineNum = 1;
        while (line != NULL) {
            printf("%d. %s\n", lineNum++, line);
            line = strtok(NULL, "\n");
        }
        free(contentCopy);
    } else {
        printf("Error: 올바른 명령어를 입력하세요!\n");
    }
}
