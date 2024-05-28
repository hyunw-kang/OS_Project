//이 헤더set을 main.c에서 불러오기 --> main.c에서 set_header.h 한 줄로 불러오기


#include <dirent.h> // 디렉토리 엔트리
#include <fcntl.h>
#include <pthread.h> // 멀티스레딩
#include <stdio.h>   // 기본 입출력
#include <stdlib.h>  // malloc, exit 등
#include <string.h>  // 문자열 처리
#include <string.h>
#include <unistd.h> // UNIX 표준 API
#include <utime.h>
#include <stdbool.h>
#include <time.h>

#include "Initial.h"
#include "Permission.h"
#include "option.h"
#include "struct.h"
#include "cat.h"
#include "touch.h"
#include "ChMod.h"
#include "cd.h"
#include "grep.h"
#include "head.h"
#include "mkdir.h"
#include "tail.h"
#include "cp.h"

#include "Save_Status.h"