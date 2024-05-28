#ifndef HEAD_H
#define HEAD_H
#pragma once

#include <ctype.h>
#include <getopt.h>
#include <unistd.h>

#include "Initial.h"
#include "Permission.h"
#include "option.h"
#include "struct.h"

void head(char *argument, DirectTree *dirtree, Users *userlist);

#endif