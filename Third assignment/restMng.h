#ifndef REST_H
#define REST_H

#include "defs.h"
static uint newKey = 0;
typedef struct simulationArgument{
    int simuTime;
    int numOfItems;
    int cusCount;
    int witersCount;
}simulationArgument;
typedef struct semaphores{
    int printSem;
}semaphores;
class restMng{
    private:
    simulationArgument simuArgs;
    semaphores sems;
    public:
    restMng(){

    }
    ~restMng(){

    }
    int getNewKey(){return newKey++;}
    void initAllSemaphores();
    void* getShmPointer(int sharedMemoryId);
    void setArgs(char **argv);
    void printData();
    void checkArgsAndPrintData(int argc,char **argv);
    int  makeSharedMemory(int size);
    int getSimulationArguments(int index);
};


#endif