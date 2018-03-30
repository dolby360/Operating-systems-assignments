#include "defs.h"
#include "Classes/menu.h"
#include "Classes/OrderBoards.h"

restMng mng; 
menu* menu1;
Orders* ord; 

int main(int argc,char **argv){
    mng.checkArgsAndPrintData(argc,argv);

    int shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
    void* shmPointer = mng.getShmPointer(shMenuKey);
    menu1 = new(shmPointer)menu(mng.getSimulationArguments(NUM_OF_ITEMS));

    shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
    shmPointer = mng.getShmPointer(shMenuKey);
    ord = new(shmPointer)Orders(mng.getSimulationArguments(NUM_OF_ITEMS));

    char msg[64];
    sprintf(msg,"Main process ID %d start",getpid());
    mng.switchTime();
    mng.printTimeWithMsg(msg);

    return OK;
}
