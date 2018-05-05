#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>

extern "C"	{ 
    #include "util.h"
}

//Limitations according to the instructions.
#define MAXIMUM_THREADS 10
#define MAXIMUM_IP_LENGTH 1025
#define MAXIMUM_IPS 10

using namespace std;

#endif