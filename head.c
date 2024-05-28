#include "head.h"

// 파일의 앞에서부터 지정된 라인 수나 바이트 수만큼 출력하는 함수
void printHead(char *content, int lines, int bytes) {
  if (bytes > 0) { // 만약 c 옵션이 있다면
    int count = 0;
    while (*content != '\0' && count < bytes) {
      putchar(*content);
      content++;
      count++;
    }
    if (count > 0) {
      putchar('\n');
    }
  } else {
    char line[256];
    int count = 0;
    while (*content != '\0' && count < lines) {
      char *newline = strchr(content, '\n');
      if (newline != NULL) {
        size_t length = newline - content;
        strncpy(line, content, length);
        line[length] = '\0';
        content = newline + 1;
      } else {
        strncpy(line, content, sizeof(line) - 1);
        line[sizeof(line) - 1] = '\0';
        content += strlen(content);
      }
      printf("%s\n", line);
      count++;
    }
  }
}

// head 명령어의 옵션을 처리하는 함수
void head(char *argument, DirectTree *dirtree, Users *userlist) {
  if (argument == NULL) {
    return;
  }

  int lines = 10;
  int bytes = -1;
  int quiet = 0;
  char *filenames[100];
  int fileCount = 0;

  char *token = strtok(argument, " ");
  while (option(token)) {
    if (token[0] == '-' && isdigit(token[1])) {
      lines = atoi(token + 1);
    } else if (strcmp(token, "-c") == 0) {
      bytes = atoi(strtok(NULL, " "));
    } else if (strcmp(token, "-q") == 0) {
      quiet = 1;
    }
    token = strtok(NULL, " ");
  }

  // 옵션 처리 이후에 파일 이름 추출
  while (token != NULL) {
    filenames[fileCount++] = token;
    token = strtok(NULL, " ");
  }

  if (fileCount == 0) {
    return;
  }

  for (int i = 0; i < fileCount; i++) {
    char *filename = filenames[i];

    TreeNode *node = Find_Dir(filename, 'f', dirtree);
    if (node == NULL) {
      printf("Failed to find file in directory tree: %s\n", filename);
      continue;
    }

    if (!Permission(node, userlist, 'r')) {
      printf("Permission Denied: %s\n", filename);
      return;
    }

    char *file_content = node->data;
    if (file_content == NULL) {
      continue;
    }

    if (!quiet && fileCount > 1) {
      if (i > 0) {
        printf("\n");
      }
      printf("==> %s <==\n", filename);
    }

    printHead(file_content, lines, bytes);
  }
}
