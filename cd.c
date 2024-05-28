#include "cd.h"
#include "Permission.h"
#include <stdbool.h>

void printLocation(TreeNode *current) {
  if (current == NULL) {
    return;
  }
  printLocation(current->parents);
  printf("/%s", current->name);
  return;
}

void cd(char *argument, DirectTree *dirtree, Users *usertree) {
  bool absolutePath = false;

  if (argument == NULL) {
    printf("Missing Directory.\n");
    return;
  }

  // / 입력시 root로 이동
  if (strcmp(argument, "/") == 0) {
    dirtree->current = dirtree->root;
    return;
  } else if (argument[1] == 'r') {
    absolutePath = true;
    argument++;
  }

  // 절대 경로일 경우 root 디렉토리부터 탐색 시작
  TreeNode *currentDir;
  if (absolutePath) {
    currentDir = dirtree->root;
  } else {
    currentDir = dirtree->current;
  }

  // 아무것도 입력을 받지 않거나, cd .d일 때 제자리
  if (argument == NULL || (argument[0] == '.') ||
      (strcmp(argument, currentDir->name) == 0)) {
    dirtree->current = currentDir;
  }
  // ~ 입력시 home 이동
  else if (argument[0] == '~') {
    dirtree->current = dirtree->root->child;
    printLocation(dirtree->current);
    printf("Current Directory : %s\n", dirtree->current->name);
    return;
  } else if (strcmp(argument, "..") == 0) {
    // 이미 root인 경우에 상위 디렉토리 접근이라면 안됨
    if (strcmp(currentDir->name, "root") == 0) {
      printf("Aleady Root Directory\n");
      return;
    }
    currentDir = currentDir->parents;
  } else if (argument[0] == 'r') {
    // 토큰으로 분리 후 순차적 이동
    char *part = strtok(argument, "/");
    if (strcmp(part, "root") != 0) {
      printf("Path should start 'root'.\n");
      return;
    }
    part = strtok(NULL, "/");
    while (!(part == NULL)) {
      // 주어진 이름의 child 디렉토리 검색
      TreeNode *child = currentDir->child;
      bool dirFound = false;
      while (child != NULL) {
        if (strcmp(child->name, part) == 0 && child->type == 'd') {
          currentDir = child;
          dirFound = true;
          break;
        }
        child = child->sib;
      }

      if (!dirFound) {
        printf("Directory not found: %s\n", part);
        return;
      }
      part = strtok(NULL, "/");
    }
  } else {
    // 경로를 토큰으로 분리하여 순차적으로 이동
    char *token = strtok(argument, "/");
    while (token != NULL) {
      // 주어진 이름의 child 디렉토리 검색
      TreeNode *child = currentDir->child;
      bool dirFound = false;
      while (child != NULL) {
        if (strcmp(child->name, token) == 0 && child->type == 'd') {
          currentDir = child;
          dirFound = true;
          break;
        }
        child = child->sib;
      }

      if (!dirFound) {
        printf("Directory not found: %s\n", token);
        return;
      }
      token = strtok(NULL, "/");
    }
  }

  // 변경된 디렉토리 출력
  dirtree->current = currentDir;
  printf("Current Directory: ");
  printLocation(dirtree->current);
  printf("\n");
}