#include "cp.h"

// 파일 복사 함수
void cp(char *argument, DirectTree *dirtree, Users *userlist) {
  if (argument == NULL) {
    return;
  }

  int argc = 0;
  char *argv[100];
  char *token = strtok(argument, " ");
  while (token != NULL) {
    argv[argc++] = token;
    token = strtok(NULL, " ");
  }

  if (argc != 2) {
    return;
  }

  char *srcFilename = argv[0];
  char *destFilename = argv[1];

  // Source 파일을 찾음
  TreeNode *srcNode = Find_Dir(srcFilename, 'f', dirtree);
  if (srcNode == NULL) {
    printf("Source file not found: %s\n", srcFilename);
    return;
  }

  // Source 파일 권한 확인
  if (!Permission(srcNode, userlist, 'r')) {
    printf("Permission Denied: %s\n", srcFilename);
    return;
  }

  // Destination 파일 확인
  TreeNode *destNode = Find_Dir(destFilename, 'f', dirtree);
  if (destNode != NULL) {
    printf("Destination file already exists: %s\n", destFilename);
    return;
  }

  // 새로운 노드 생성 및 초기화
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  strncpy(newNode->name, destFilename, Name_Len);
  newNode->type = 'f';
  newNode->User = userlist->current->User;
  newNode->Group = userlist->current->Group;
  newNode->Size = srcNode->Size;
  memcpy(newNode->permission, srcNode->permission, sizeof(srcNode->permission));

  time_t timer = time(NULL);
  struct tm *t = localtime(&timer);
  newNode->Year = t->tm_year + 1900;
  newNode->Month = t->tm_mon + 1;
  newNode->Day = t->tm_mday;
  newNode->Hour = t->tm_hour;
  newNode->Minute = t->tm_min;

  strncpy(newNode->data, srcNode->data, ROW_SIZE * COLUM_SIZE);
  newNode->parents = srcNode->parents;
  newNode->child = NULL;
  newNode->sib = srcNode->parents->child;
  srcNode->parents->child = newNode;
}