#include "defs.h"
#include "menu.h"
#include "OrderBoards.h"

restMng mng; 
menu* menu1;
Orders* ord; 

int main(int argc,char **argv){
    mng.checkArgsAndPrintData(argc,argv);

    int shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE,1);
    void* shmPointer = mng.getShmPointer(shMenuKey);
    menu1 = new(shmPointer)menu(mng.getSimulationArguments(NUM_OF_ITEMS));

    shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE,2);
    shmPointer = mng.getShmPointer(shMenuKey);
    ord = new(shmPointer)Orders(mng.getSimulationArguments(NUM_OF_ITEMS));


    return OK;
}
