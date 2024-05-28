#include "Save_Status.h"

// 디렉토리 정보를 로컬 파일에 저장하는 함수
void Save_Directory_Info(DirectTree *dirtree, char *directoryFilename,
                         char *dataFilename) {
  // 디렉토리 정보를 저장할 파일 열기
  FILE *dirFile = fopen(directoryFilename, "w");
  if (dirFile == NULL) {
    printf("Failed to open directory file for writing: %s\n",
           directoryFilename);
    return;
  }

  // 데이터를 저장할 파일 열기
  FILE *dataFile = fopen(dataFilename, "w");
  if (dataFile == NULL) {
    printf("Failed to open data file for writing: %s\n", dataFilename);
    fclose(dirFile);
    return;
  }

  // 현재 디렉토리부터 순회하며 정보 저장
  Save_Directory_Info_Helper(dirtree->current, dirFile, dataFile);

  // 파일 닫기
  fclose(dirFile);
  fclose(dataFile);
}

// 디렉토리 정보를 로컬 파일에 저장하는 보조 함수
void Save_Directory_Info_Helper(TreeNode *node, FILE *dirFile, FILE *dataFile) {
  // NULL 이면 -1을 기록
  if (node == NULL) {
    fprintf(dirFile, "%d ", -100);
    return;
  } else {
    fprintf(dirFile, "%d ", 100);
    // NULL이 아니면 1을 기록
    // 디렉토리 정보를 파일에 쓰기
    fprintf(dirFile, "%s %c %d %d %d %d %d %d %d ", node->name, node->type,
            node->User, node->Group, node->Year, node->Month, node->Day,
            node->Hour, node->Minute);

    for (int i = 0; i < 9; i++) {
      fprintf(dirFile, "%d ", node->permission[i]);
    }
    fprintf(dirFile, "%d ", node->Size);

    // 데이터 정보 파일에 쓰기 (치환된 문자열 사용)
    fprintf(dataFile, "%s ", node->data);
    fprintf(dataFile, "%s", "!_!End Of File!_!\n");

    // 자식 노드들에 대해 재귀 호출
    Save_Directory_Info_Helper(node->child, dirFile, dataFile);
    Save_Directory_Info_Helper(node->sib, dirFile, dataFile);
  }
  return;
}

// 디렉토리 정보를 복원하는 함수
DirectTree *Load_Directory_Info(Users *users, char *dirFilename,
                                char *dataFilename) {
  FILE *dirFile = fopen(dirFilename, "r");
  FILE *dataFile = fopen(dataFilename, "r");
  if (dirFile == NULL || dataFile == NULL) {
    // printf("Failed to open files\n");
    return NULL;
  }

  // 디렉토리 정보 복원 보조 함수 호출
  TreeNode *root = Load_Directory_Info_Helper(dirFile, dataFile, users);

  fclose(dirFile);
  fclose(dataFile);

  // DirTree 생성 및 초기화
  DirectTree *dirtree = (DirectTree *)malloc(sizeof(DirectTree));
  dirtree->root = root;
  dirtree->current = root;

  return dirtree;
}

// 디렉토리 정보 복원 보조 함수
TreeNode *Load_Directory_Info_Helper(FILE *dirFile, FILE *dataFile,
                                     Users *users) {
  // 새로운 노드 생성
  int Is;
  printf("reading \n");
  if (fscanf(dirFile, "%d ", &Is) == -1 || Is == -100) {
    return NULL;
  }

  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

  // 파일에서 디렉토리 정보 읽어오기
  fscanf(dirFile, "%s %c %d %d %d %d %d %d %d ", node->name, &(node->type),
         &(node->User), &(node->Group), &(node->Year), &(node->Month),
         &(node->Day), &(node->Hour), &(node->Minute));

  for (int i = 0; i < 9; i++) {
    fscanf(dirFile, "%d ", &(node->permission[i]));
  }
  fscanf(dirFile, "%d ", &(node->Size));

  char line[100];
  fgets(line, sizeof(line), dataFile);
  strncpy(node->data, line, sizeof(node->data));
  while (1) {
    fgets(line, sizeof(line), dataFile);
    if (strcmp(line, " !_!End Of File!_!\n") == 0) {
      break;
    }
    strcat(node->data, line);
  }
  // 자식 노드들에 대해 재귀 호출
  node->child = Load_Directory_Info_Helper(dirFile, dataFile, users);
  node->sib = Load_Directory_Info_Helper(dirFile, dataFile, users);
  return node;
}

// 데이터 문자열에서 \n을 치환하는 함수
char *ReplaceNewline(const char *str) {
  char *result = (char *)malloc(strlen(str) * 2 + 1);
  if (result == NULL) {
    printf("Failed to allocate memory for result\n");
    return NULL;
  }

  int j = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == '\n') {
      strcat(result, NEWLINE_REPLACEMENT);
      j += strlen(NEWLINE_REPLACEMENT);
    } else {
      result[j++] = str[i];
    }
  }
  result[j] = '\0';

  return result;
}

// 치환된 문자열에서 \n을 복원하는 함수
void RestoreNewline(char *str) {
  char *pos = strstr(str, NEWLINE_REPLACEMENT);
  while (pos != NULL) {
    memcpy(pos, (const char *)NEWLINE_RESTORE,
           strlen((const char *)NEWLINE_RESTORE));
    pos = strstr(pos, NEWLINE_REPLACEMENT);
  }
  return;
}