#ifndef TAIL_H
#define TAIL_H
#pragma once

#include <ctype.h>
#include <getopt.h>
#include <unistd.h>

#include "Initial.h"
#include "Permission.h"
#include "option.h"
#include "struct.h"

void tail(char *argument, DirectTree *dirtree, Users *userlist);

#endif