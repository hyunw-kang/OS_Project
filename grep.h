#ifndef GREP_H
#define GREP_H
#pragma once

#include "Initial.h"
#include "Permission.h"
#include "option.h"
#include "struct.h"

#include <ctype.h>
#include <getopt.h>
#include <unistd.h>

void grep(char *argument, DirectTree *dirtree, Users *userlist);

#endif