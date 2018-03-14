#ifndef DRIVER_H
#define DRIVER_H

#include "defs.h"

class Commands{
private:
    char* fullPath;
public:
    Commands();
    ~Commands();
    void getCurrentDirectory();
};

#endif