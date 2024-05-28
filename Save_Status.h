#ifndef SAVE_STATUS_H
#define SAVE_STATUS_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Initial.h"
#include "struct.h"

// 데이터 파일에 저장할 때 \n 대신 치환할 문자열
#define NEWLINE_REPLACEMENT "<NEWLINE>"

// 데이터 파일에서 읽을 때 \n으로 복원할 문자열
#define NEWLINE_RESTORE '\n'

void Save_Directory_Info(DirectTree *dirtree, char *directoryFilename,
                         char *dataFilename);
void Save_Directory_Info_Helper(TreeNode *node, FILE *dirFile, FILE *dataFile);

DirectTree *Load_Directory_Info(Users *users, char *dirFilename,
                                char *dataFilename);

TreeNode *Load_Directory_Info_Helper(FILE *dirFile, FILE *dataFile,
                                     Users *users);

char *ReplaceNewline(const char *str);
void RestoreNewline(char *str);
#endif