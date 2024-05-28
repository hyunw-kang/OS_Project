#pragma once
#ifndef CHMOD_H
#define CHMOD_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include "struct.h"

int* str2int(char mode);
void chmod(DirectTree* dirtree, char* chmd);


#endif