#ifndef CD_H
#define CD_H

#include <stdlib.h>
#include <string.h>

#include "Permission.h"
#include "struct.h"

#define NAME_LENGTH 1000

#pragma once
void cd(char *dirname, DirectTree *dirtree, Users *users);
void printDirectory(TreeNode *current);
#endif