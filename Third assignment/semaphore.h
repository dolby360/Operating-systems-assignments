#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>  

int makeSemaphore(int key,int value);

#endif