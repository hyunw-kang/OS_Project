#include "Permission.h"

int Permission(TreeNode *node, Users *user, char autho) {
  // user명이 같을경우
  if (user->current->User == node->User) {
    if (autho == 'r') {
      if (node->permission[0] == 1)
        return 1;
      else
        return 0;
    } else if (autho == 'w') {
      if (node->permission[1] == 1)
        return 1;
      else
        return 0;
    } else {
      if (node->permission[2] == 1)
        return 1;
      else
        return 0;
    }
  }

  // group명이 같을 경우
  else if (user->current->Group == node->Group) {
    if (autho == 'r') {
      if (node->permission[3] == 1)
        return 1;
      else
        return 0;
    } else if (autho == 'w') {
      if (node->permission[4] == 1)
        return 1;
      else
        return 0;
    } else {
      if (node->permission[5] == 1)
        return 1;
      else
        return 0;
    }
  }

  //예외 경우 처리
  else {
    if (autho == 'r') {
      if (node->permission[6] == 1)
        return 1;
      else
        return 0;
    } else if (autho == 'w') {
      if (node->permission[7] == 1)
        return 1;
      else
        return 0;
    } else {
      if (node->permission[8] == 1)
        return 1;
      else
        return 0;
    }
  }
}