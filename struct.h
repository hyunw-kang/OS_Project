#pragma once
#define HAVE_STRUCT_TIMESPEC
#define _CRT_SECURE_NO_WARNINGS

#ifndef STRUCT_H
#define STRUCT_H

#define Name_Len 100
#define SIZE 1000
#define ROW_SIZE 100
#define COLUM_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// user관련 정보를 저장
typedef struct UserNode {
  char name[Name_Len];
  int User, Group;
  int Year, Month, Day, Hour, Minute;
  struct UserNode *link;
} UserNode;

typedef struct User {
  int top_user, top_group;
  UserNode *head;
  UserNode *tail;
  UserNode *current;
} Users;

// TreeNode / directory => d, file => f
typedef struct Tree_Node {
  char name[Name_Len];
  char type;
  char data[ROW_SIZE * COLUM_SIZE];
  int User, Group;
  int Year, Month, Day, Hour, Minute;
  int permission[9];
  int Size;
  struct Tree_Node *parents;
  struct Tree_Node *child;
  struct Tree_Node *sib;
} TreeNode;

typedef struct DirectTree {
  TreeNode *root;
  TreeNode *current;
} DirectTree;

typedef struct FileInfo {
  const char *filename; // touch structure
  DirectTree *dirtree;
} FileInfo;

typedef struct ThreadTree {
  DirectTree *threadTree;
  DirectTree *name;
  char *command;
  char *username;
  int mode;
  int option;
  char *content;
} ThreadTree;

//실제로 경로가 존재하는지 판단하고 d => directory/ f=>file
static TreeNode *Find_Dir(char *path, char Type, DirectTree *dirtree) {
  TreeNode *temp = dirtree->root;

  //절대경로
  // mkdir /root/home/file <- /root/home만 path에 전달
  if (path[0] == '/') {
    char *token;
    token = strtok(path, "/");
    // /root로 경로설정안하면 오류발생
    if (strcmp(token, "root") != 0) {
      printf("잘못된 경로 입니다. : %s\n", token);
      return NULL;
    }
    // /root 까지는 맞을경우 token = home
    token = strtok(NULL, "/");
    while (token != NULL) {
      // child로 내려
      temp = temp->child;
      // temp갈때까지 무한루프
      while (temp != NULL) {
        // 경로와 타입이 존재하면 break
        if (strcmp(temp->name, token) == 0 && temp->type == Type) {
          break;
        }
        // 없으면 sib으로 이동
        temp = temp->sib;
      }

      if (temp == NULL)
        return temp;
      //모든 sib검색, 없으면 잘라서 판단
      token = strtok(NULL, "/");
    }
  }

  //상대경로 : 현재 디렉토리에 생
  // mkdir file
  else {
    temp = dirtree->current->child;
    while (temp != NULL) {
      // 입력한 경로가 존재하고 원하는 타입의 형태일 경우 break

      if (strcmp(temp->name, path) == 0 && temp->type == Type) {

        return temp;
      }
      // 없으면 sib로 이동,판단

      temp = temp->sib;
    }
  }
  return temp;
}
#endif