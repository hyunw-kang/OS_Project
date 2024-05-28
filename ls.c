#include "ls.h"

void ls(char *argument, DirectTree *dirtree, Users *usertree) {
  bool hidden = false; // -a 구현 변수
  bool detail = false; // -l 구현 변수
  bool noOp = false;   // 옵션이 없는 경우
  char *options[5];    // 권한 확인
  int optionN = 0;

  TreeNode *node = NULL;

  if (argument == NULL || argument == " ") {
    noOp = true;
  } else if (option(argument)) { //옵션이 있는지 확인
    char *part = strtok(argument, " ");
    while (!(part == NULL)) {
      if (part[0] == '-') {
        options[optionN] = part;
        optionN++;
      } else {
        break;
      }
      part = strtok(NULL, " ");
    }
  }

  //자식 노드로 이동
  node = dirtree->current->child;

  for (int i = 0; i < optionN; i++) {
    if (strcmp(options[i], "-l") == 0) { //-l 옵션
      detail = true;
    } else if (strcmp(options[i], "-a") == 0) { //-a 옵션
      hidden = true;
    } else {
      printf("Invalid Option\n"); //-a나 -l이 아닐 경우
      return;
    }
  }
  // 옵션이 없을 때
  if (noOp) {
  } else if (!optionN && !detail && !hidden) { //이상한 옵션 입력
    printf("Invalid Command\n");
    return;
  }

  if (!detail) { // -l이 아닐 때

    if (hidden) { // -a 활성화
      printf(".\t");
      if (!(dirtree->current == dirtree->root)) {
        printf("..\t");
      }
    }

    while (!(node == NULL)) { // sibling Node NULL�϶� �ݺ�
      if (!hidden) {
        if (strncmp(node->name, ".", 1) == 0) { //� �¾�ϵ�
          node = node->sib;                     //형제 노드 연결
          continue;
        }
      }

      printf("%s\t", node->name);

      node = node->sib; //형제 노드 연결
    }
    printf("\n");
  } else { // 상세 정보 확인

    while (!(node == NULL)) { //-l�̰� Node� � sibling NULL�϶� �ݺ�
      if (!hidden) {          //-l 비활성화
        if (strncmp(node->name, ".", 1) == 0) {
          node = node->sib;
          continue;
        }
      }

      //디렉토리인지 파일인지 확인
      if (node->type == 'd') {
        printf("directory:   ");
      }
      if (node->type == 'f') {
        printf("file:\t\t ");
      }
      // array에 있는 권한
      for (int i = 0; i < 9; i += 3) {
        if (node->permission[i] == 1)
          printf("r");
        else
          printf("-");
        if (node->permission[i + 1] == 1)
          printf("w");
        else
          printf("-");
        if (node->permission[i + 2] == 1)
          printf("x");
        else
          printf("-");
      }
      //유저 정보
      printf(" %d", node->User);
      printf(" %d", node->Group);
      //파일 및 디렉토리 크기
      printf(" %d\t", node->Size);
      //노드 생성 및 수정 시간
      printf(" %d-%02d-%02d %02d:%02d", node->Year, node->Month, node->Day,
             node->Hour, node->Minute);
      //파일 및 디렉토리 이름
      printf(" %s\n", node->name);

      node = node->sib; //형제 노드 연결
    }
  }
  return;
}