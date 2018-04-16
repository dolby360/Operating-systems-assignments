#ifndef REST_H
#define REST_H

#include "lockerMng.h"
// class lockerMng;

typedef struct simulationArgument{
    int simuTime;
    int numOfItems;
    int cusCount;
    int witersCount;
}simulationArgument;

class restMng: public lockerMng{
private:
    simulationArgument simuArgs;
    timeval timStart;
    timeval timNow;
public:
    restMng(){}
    ~restMng(){}
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
    void execCust(pid_t pid);
    void waiterProcess(int wId);
    void customerProcess(int custId);

    //False - time pass
    //True  - we have more time
    bool WeAreGoodWithTime();
};


#endif