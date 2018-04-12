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
    timeval timStart;
    timeval timNow;
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

    void switchTime();
    void printTimeWithMsg(char *msg);
    int execWaiter();
    void execCust(int pid);
    void waiterProcess(int waiterId);
    void customerProcess(int custId);

    //False - time pass
    //True  - we have more time
    bool WeAreGoodWithTime();
};


#endif