#include "touch.h"

// 스레드가 실행할 함수
void *create_or_update_file(void *arg) {
  FileInfo *fileInfo = (FileInfo *)arg;
  char *filename = (char *)fileInfo->filename;
  DirectTree *dirtree = fileInfo->dirtree;

  TreeNode *node = Find_Dir(filename, 'f', dirtree);

  if (node == NULL) {
    printf("Creating file: %s\n", filename);
    int fd = open(filename, O_WRONLY | O_CREAT, 0777);
    if (fd == -1) {
      perror("파일 생성 실패");
      printf("Error: %s\n", strerror(errno));
      pthread_exit((void *)2);
    }
    close(fd);
    printf("File created successfully.\n");

    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    strncpy(newNode->name, filename, sizeof(newNode->name));
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    newNode->type = 'f';
    newNode->Size = 0;
    newNode->User = dirtree->current->User;
    newNode->Group = dirtree->current->Group;
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
    newNode->parents = dirtree->current;
    newNode->child = NULL;
    newNode->sib = dirtree->current->child;
    dirtree->current->child = newNode;
  } else {
    printf("Updating file: %s\n", filename);
    utime(filename, NULL);
  }

  pthread_exit((void *)0);
}

// touch 명령어를 처리하는 함수
void touch(char *argument, DirectTree *dirtree) {
  pthread_t threads[100];
  FileInfo fileInfos[100];
  int thread_count = 0;

  char *token = strtok(argument, " ");
  while (token != NULL) {
    fileInfos[thread_count].filename = token;
    fileInfos[thread_count].dirtree = dirtree;
    pthread_create(&threads[thread_count], NULL, create_or_update_file, (void *)&fileInfos[thread_count]);
    thread_count++;
    token = strtok(NULL, " ");
  }

  for (int i = 0; i < thread_count; i++) {
    void *status;
    pthread_join(threads[i], &status);
    if (status != (void *)0) {
      printf("Thread %d returned with status %ld\n", i, (long)status);
    }
  }
}
