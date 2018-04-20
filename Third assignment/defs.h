#ifndef DEFS_H
#define DEFS_H


#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iomanip> 
#include "restMng.h"
#include "utility/util.h"
#define OK 0

#define SIM_TIME 0
#define NUM_OF_ITEMS 1
#define CUS_COUNT 2
#define WAITERS_COUNT 3

#define SHARED_MEMORY_SIZE 1024
#define DECENT_SIZE_OF_SHARED_MEMORY 128

typedef unsigned int uint;



#endif
