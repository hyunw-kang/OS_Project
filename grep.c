#include "grep.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 100

// 주어진 패턴을 찾아서 출력하는 함수
void match(char (*content)[MAX_LINE_LENGTH], int line_count,
           const char *pattern, int p_n, int p_v, int p_i) {
  char word[200];
  strncpy(word, pattern, 200);
  if (p_i) {
    for (int i = 0; word[i]; i++) {
      word[i] = tolower(word[i]);
    }
  }

  for (int i = 0; i < line_count; i++) {
    char line[MAX_LINE_LENGTH];
    strcpy(line, content[i]);

    if (p_i) {
      for (int j = 0; line[j]; j++) {
        line[j] = tolower(line[j]);
      }
    }

    char *printLine = strstr(line, word);
    if ((printLine != NULL && !p_v) || (printLine == NULL && p_v)) {
      if (p_n) {
        printf("%d\t", i + 1);
      }
      printf("%s", content[i]);
    }
  }
}

// grep 명령어 기능을 구현한 함수
void grep(char *argument, DirectTree *dirtree, Users *userlist) {
  char *options[3];    // 옵션을 저장할 배열
  int optionCount = 0; // 옵션 개수
  int p_n = 0;         // -n 옵션
  int p_v = 0;         // -v 옵션
  int p_i = 0;         // -i 옵션

  char *token = strtok(argument, " ");
  if (option(token)) { // 옵션이 있다면
    while (token != NULL) {
      if (token[0] == '-') {
        options[optionCount] = token; // 옵션을 options 배열에 할당
        optionCount++;
      } else {
        break;
      }
      token = strtok(NULL, " ");
    }

    // options 배열을 탐색하면서 변수 값을 변경
    for (int i = 0; i < optionCount; i++) {
      if (strcmp(options[i], "-n") == 0) {
        p_n = 1;
      } else if (strcmp(options[i], "-v") == 0) {
        p_v = 1;
      } else if (strcmp(options[i], "-i") == 0) {
        p_i = 1;
      } else {
        printf("Wrong option : %s\n", options[i]);
        return;
      }
    }
  }

  // 옵션 외 나머지 인자로 패턴과 파일 이름 설정
  char *pattern = token;
  char *filename = strtok(NULL, "");

  TreeNode *node = Find_Dir(filename, 'f', dirtree);
  if (node == NULL) {
    printf("Failed to find file in directory tree: %s\n", filename);
    return;
  }

  if (!Permission(node, userlist, 'r')) {
    printf("Permission Denied: %s\n", filename);
    return;
  }

  // 파일을 읽어와서 내용을 content 배열에 저장
  char content[MAX_LINES][MAX_LINE_LENGTH];
  int line_count = 0;

  // 파일 내용을 TreeNode의 data 필드에서 읽어오기
  char *file_content = node->data;
  char *line = strtok(file_content, "\n");
  while (line != NULL && line_count < MAX_LINES) {
    strncpy(content[line_count], line, MAX_LINE_LENGTH);
    content[line_count][MAX_LINE_LENGTH - 1] = '\0';
    line_count++;
    line = strtok(NULL, "\n");
  }

  match(content, line_count, pattern, p_n, p_v, p_i);
}