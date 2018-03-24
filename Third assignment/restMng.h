#ifndef REST_H
#define REST_H

#include "defs.h"

typedef struct simulationArgument{
    int simuTime;
    int manItems;
    int cusCount;
    int witersCount;
}simulationArgument;

class restMng{
    private:
    simulationArgument simuArgs;

    public:
    restMng(){

    }
    ~restMng(){

    }
    void setArgs(char **argv);
    void printData();
    void checkArgsAndPrintData(int argc,char **argv);
    int  makeSharedMemory(int size,int _key);
};


#endif