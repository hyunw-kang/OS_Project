#include "tail.h"

// 파일의 뒤에서부터 지정된 라인 수나 바이트 수만큼 출력하는 함수
void printTail(char *content, int lines, int bytes) {
  long file_size = strlen(content);

  if (bytes > 0) {
    // bytes만큼 뒤에서부터 출력
    long start_pos = file_size - bytes;
    if (start_pos < 0) {
      start_pos = 0;
    }
    printf("%s\n", content + start_pos);
  } else {
    // lines만큼 뒤에서부터 출력
    int lineCount = 0;
    char *end = content + file_size;
    char *start = end;

    // 파일의 끝에서부터 라인 개수만큼 읽어오기
    while (start > content) {
      start--;
      if (*start == '\n') {
        lineCount++;
        if (lineCount > lines) {
          start++;
          break;
        }
      }
    }

    // start부터 파일 끝까지 출력
    printf("%s", start);
  }
}

// tail 명령어의 옵션을 처리하는 함수
void tail(char *argument, DirectTree *dirtree, Users *userlist) {
  if (argument == NULL) {
    return;
  }

  int lines = 10;
  int bytes = -1;
  int quiet = 0;
  int verbose = 0;
  char *filenames[100];
  int fileCount = 0;

  // 매개변수에서 옵션 처리
  char *token = strtok(argument, " ");
  while (token != NULL) {
    if (token[0] == '-' && isdigit(token[1])) {
      lines = atoi(token + 1);
    } else if (strcmp(token, "-c") == 0) {
      bytes = atoi(strtok(NULL, " "));
    } else if (strcmp(token, "-q") == 0) {
      quiet = 1;
    } else if (strcmp(token, "-v") == 0) {
      verbose = 1;
    } else {
      filenames[fileCount++] = token;
    }
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
      continue;
    }

    char *file_content = node->data;
    if (file_content == NULL) {
      continue;
    }

    if (!quiet && verbose) {
      printf("==> %s <==\n", filename);
    }

    printTail(file_content, lines, bytes);
  }
}
