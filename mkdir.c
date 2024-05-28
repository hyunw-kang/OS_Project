#include "mkdir.h"

//디렉토리 생성 함수
void make_dir(char *DirName, char Type, TreeNode *parent, DirectTree *dirtree,
              Users *usertree) {
  TreeNode *tempNode = Find_Dir(DirName, Type, dirtree);
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));

  // 파일 이름이 이미 존재할 때
  if (!(tempNode == NULL)) {
    printf("Already exist name : %s\n", DirName);
    free(new);
    return;
  }
  //생성 타입이 디렉토리와 파일이 아닐 때
  if (parent->type != 'd' && parent->type != 'f') {
    printf("%s is not a directory or file.\n", parent->name);
    free(new);
    return;
  }
  // 파일 이름이 .일때 거절
  if (DirName[0] == '.') {
    printf(". is Invalid Directory Name\n");
    free(new);
    return;
  }
  // 파일 이름이 ..일때 거절
  if (DirName[0] == '.' && DirName[1] == '.') {
    printf("..is Invalid Directory Name\n");
    free(new);
    return;
  }

  if (DirName[0] == '~') {
    printf("~ is Invalid Directory Name\n");
    free(new);
    return;
  }
  //노드 연결
  new->parents = parent;
  new->child = NULL;
  new->sib = NULL;
  if (parent->child == NULL) {
    parent->child = new;
  } else {
    parent = parent->child;
    while (!(parent->sib == NULL)) {
      parent = parent->sib;
    }
    parent->sib = new;
  }
  //노드 정보 설정
  time_t Time = time(NULL);
  struct tm *t = localtime(&Time);
  strncpy(new->name, DirName, Name_Len);
  new->type = Type;
  if (new->type == 'd') {
    strcpy(new->data, "\n");
    new->Size = 0; //디렉토리 경우 크기 없음
  }
  new->User = usertree->current->User;
  new->Group = usertree->current->Group;

  //노드 시간 설정
  new->Year = t->tm_year + 1900;
  new->Month = t->tm_mon + 1;
  new->Day = t->tm_mday;
  new->Hour = t->tm_hour;
  new->Minute = t->tm_min;

  int newPer[9] = {1, 1, 1, 1, 0, 1, 1, 0, 1};
  memcpy(new->permission, newPer, sizeof(newPer));

  return;
}

void mkdir(char *path, DirectTree *dirtree, Users *usertree) {
  char *temp;
  char *dirName;
  if (path == NULL) {
    printf("Wrong Directory : path is NULL\n");
    return;
  }

  char *pathCopy = strdup(path);
  char *tempPath = pathCopy; // free를 위한 초기 포인터 보존

  if (option(pathCopy) == 1) {
    char *part = strtok(pathCopy, " ");
    if (!(part == NULL) && strcmp(part, "-p") == 0) {
      char *route = strtok(NULL, " ");
      if (route[0] == '/') {
        TreeNode *tempNode = dirtree->root;
        TreeNode *parent = dirtree->root;
        part = strtok(route, "/");
        // 절대경로로 생성시 /root로 시작하지 않을 경우 오류 발생
        if (part == NULL || strcmp(part, "root") != 0) {
          printf("%s is Wrong Directory\n", part);
          free(tempPath); // 할당된 메모리 해제
          return;
        }

        part = strtok(NULL, "/");

        while (!(part == NULL)) {
          tempNode = tempNode->child;
          while (tempNode != NULL) {
            if (strcmp(tempNode->name, part) == 0 || tempNode->type == 'd') {
              parent = tempNode;
              break;
            }
            tempNode = tempNode->sib;
          }

          if (tempNode == NULL) {
            while (!(part == NULL)) {
              make_dir(part, 'd', parent, dirtree, usertree);
              part = strtok(NULL, "/");

              parent = parent->child;
              if (!(part == NULL)) { // parent가 NULL이 아닌지 확인
                while (!(parent->sib == NULL)) {
                  parent = parent->sib;
                }
              }
            }
          }
          part = strtok(NULL, "/");
        }
        free(tempPath); // 할당된 메모리 해제
        return;
      } else {
        //작업 위치 변경
        TreeNode *tmpNode = dirtree->current;
        part = strtok(route, "/");
        while (!(part == NULL)) {
          make_dir(part, 'd', tmpNode, dirtree, usertree);
          tmpNode = tmpNode->child;
          while (!(tmpNode->sib == NULL)) {
            tmpNode = tmpNode->sib;
          }
          part = strtok(NULL, "/");
        }
      }
    } else {
      printf("Wrong Option : %s\n", part);
      free(tempPath);
      return;
    }
  }

  else {
    //옵션이 없을 때 해당 위치에서 생성
    if (pathCopy[0] == '/') {
      temp = strrchr(pathCopy, '/');
      temp[0] = '\0';
      dirName = temp + 1;
      TreeNode *parent = Find_Dir(pathCopy, 'd', dirtree);
      make_dir(dirName, 'd', parent, dirtree, usertree);
    } else {
      dirName = pathCopy;
      char *part;
      part = strtok(dirName, "/");
      part = strtok(NULL, "/");
      if (!(part == NULL)) {
        printf("/ is Invalid Directory Name\n");
        return;
      }
      make_dir(dirName, 'd', dirtree->current, dirtree, usertree);
    }
  }

  free(tempPath);
}

void *multithread(void *arg) {
  MkdirThread *data = (MkdirThread *)arg;
  mkdir(data->dirName, data->dirtree, data->user);
  free(arg);
  return NULL;
}

void multi_mkdir(char *path, DirectTree *dirtree, Users *users) {
  pthread_t threads[50];
  int i = 0;
  //옵션이 있는 경우
  if (option(path) == 1) {
    mkdir(path, dirtree, users);
  }
  //옵션이 없는 경우
  else {
    char *part = strtok(path, " ");
    for (i = 0; part != NULL; i++) {
      MkdirThread *thread = (MkdirThread *)malloc(sizeof(MkdirThread));
      thread->dirtree = dirtree;
      thread->user = users;
      strncpy(thread->dirName, part, 50);
      thread->dirName[49] =
          '\0'; // array의 크기가 50이기 때문에 마지막 49는 빈칸
      pthread_create(&threads[i], NULL, multithread, thread);
      part = strtok(NULL, " ");
    }
  }

  for (int j = 0; j < i; j++) {
    pthread_join(threads[j], NULL);
  }
}