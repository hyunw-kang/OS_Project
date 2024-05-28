#ifndef TOUCH_H
#define TOUCH_H

#include "struct.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utime.h>
#include <locale.h>
#include <errno.h>

void touch(char *argument, DirectTree *dirtree);

#endif
