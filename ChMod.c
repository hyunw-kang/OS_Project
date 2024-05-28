#include "ChMod.h"

//배열자체 반환을위해 포인터사용
int *str2int(char mode) {
  if (mode == '7') {
    static int change[3] = {1, 1, 1};
    return change;
  } else if (mode == '6') {
    static int change[3] = {1, 1, 0};
    return change;
  } else if (mode == '5') {
    static int change[3] = {1, 0, 1};
    return change;
  } else if (mode == '4') {
    static int change[3] = {1, 1, 0};
    return change;
  } else if (mode == '3') {
    static int change[3] = {1, 1, 0};
    return change;
  } else if (mode == '2') {
    static int change[3] = {1, 1, 0};
    return change;
  } else if (mode == '1') {
    static int change[3] = {1, 1, 0};
    return change;
  } else if (mode == '0') {
    static int change[3] = {0, 0, 0};
    return change;
  } else {
    return NULL;
  }
}

//바꿔야할 filename 그 파일에 해당하는 현재권한입력
void chmod(DirectTree *dirtree, char *chmd) {
  TreeNode *tmp = NULL;
  char *cm_num;
  char *file;
  // int* per;
  int ChNum[3]; //변경할 권한 저장

  if ('0' <= chmd[0] && chmd[0] <= '7') {
    cm_num = strtok(chmd, " ");
    printf("cm_num : %s\n", cm_num);

    file = strtok(NULL, " ");
    printf("file : %s\n", file);

    if (strlen(chmd) != 3) { //숫자3개아니면 error
      printf("Cannot change mode\n");
      return;
    }
    for (int i = 0; i < 3; i++) {
      ChNum[i] = cm_num[i] - '0'; // ascll코드로 변환
    }

    // filename 해당하는게 없으면 오류
    tmp = Find_Dir(file, 'f', dirtree);
    if (tmp == NULL) {
      printf("Wrong filename : %s\n", file);
    }

    for (int i = 0; i < 3; i++) {

      int *per = str2int(cm_num[i]);
      for (int j = 0; j < 3; j++) {
        tmp->permission[3 * i + j] = per[j];
      }
    }
    return;

  }
  //바꿀권한이 없으면 출력
  else {
    printf("Cannot change mode\n");
    return;
  }
  return;
}