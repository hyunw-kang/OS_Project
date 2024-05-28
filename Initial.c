#include "Initial.h"

Users *Initial() {
  time_t timer = time(NULL);
  struct tm *t = localtime(&timer);
  Users *users = (Users *)malloc(sizeof(Users));
  UserNode *manager = (UserNode *)malloc(sizeof(UserNode));
  UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
  strncpy(manager->name, "Manager", Name_Len);
  manager->User = 1;
  manager->Group = 1;
  manager->Year = t->tm_year + 1900;
  manager->Month = t->tm_mon + 1;
  manager->Day = t->tm_mday;
  manager->Hour = t->tm_hour;
  manager->Minute = t->tm_min;
  manager->link = newNode;

  strncpy(newNode->name, "Manager2", Name_Len);
  newNode->User = 2;
  newNode->Group = 2;
  newNode->Year = t->tm_year + 1900; // 타임스탬프 현재시간출력 년 월 일 시 분
  newNode->Month = t->tm_mon + 1;
  newNode->Day = t->tm_mday;
  newNode->Hour = t->tm_hour;
  newNode->Minute = t->tm_min;
  newNode->link = NULL;
  users->head = manager;
  users->tail = newNode;
  users->current = manager;

  return users;
}
DirectTree *Initial_Dir(Users *users) {
  time_t timer = time(NULL);
  struct tm *t = localtime(&timer);
  DirectTree *dirtree = (DirectTree *)malloc(sizeof(DirectTree));
  TreeNode *root = (TreeNode *)malloc(sizeof(TreeNode));
  TreeNode *home = (TreeNode *)malloc(sizeof(TreeNode));
  TreeNode *txt = (TreeNode *)malloc(sizeof(TreeNode));

  strncpy(root->name, "root", Name_Len);
  root->type = 'd';
  root->User = users->current->User;
  root->Group = users->current->Group;
  int root_per[9] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
  memcpy(root->permission, root_per, sizeof(root_per));
  root->Size = 4096;
  root->Year = t->tm_year + 1900;
  root->Month = t->tm_mon + 1;
  root->Day = t->tm_mday;
  root->Hour = t->tm_hour;
  root->Minute = t->tm_min;

  root->parents = NULL;
  root->child = home;
  root->sib = NULL;
  strncpy(root->data, "\0", ROW_SIZE * COLUM_SIZE);

  strncpy(home->name, "home", Name_Len);
  home->type = 'd';
  home->User = users->current->User;
  home->Group = users->current->Group;
  int home_per[9] = {1, 1, 1, 1, 0, 1, 1, 0, 1}; // rwxr-xr-x
  memcpy(home->permission, home_per, sizeof(home_per));
  home->Size = 4096;
  home->Year = t->tm_year + 1900;
  home->Month = t->tm_mon + 1;
  home->Day = t->tm_mday;
  home->Hour = t->tm_hour;
  home->Minute = t->tm_min;

  home->parents = root;
  home->child = txt;
  home->sib = NULL;
  strncpy(home->data, "\0", ROW_SIZE * COLUM_SIZE);

  strncpy(txt->name, "text.txt", Name_Len);
  txt->type = 'f';
  txt->User = users->current->User;
  txt->Group = users->current->Group;
  txt->Size = 4096;
  memcpy(txt->permission, home_per, sizeof(home_per));
  txt->Year = t->tm_year + 1900;
  txt->Month = t->tm_mon + 1;
  txt->Day = t->tm_mday;
  txt->Hour = t->tm_hour;
  txt->Minute = t->tm_min;

  txt->parents = home;
  txt->child = NULL;
  txt->sib = NULL;

  dirtree->root = root;
  dirtree->current = home;

  return dirtree;
}