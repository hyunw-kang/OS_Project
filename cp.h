#ifndef CP_H
#define CP_H
#pragma once

#include <ctype.h>
#include <getopt.h>
#include <unistd.h>

#include "Initial.h"
#include "Permission.h"
#include "Save_Status.h"
#include "option.h"
#include "struct.h"

void cp(char *argument, DirectTree *dirtree, Users *userlist);

#endif