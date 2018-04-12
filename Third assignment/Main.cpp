#include "defs.h"
#include "Classes/menu.h"
#include "Classes/OrderBoards.h"

restMng mng; 
menu* menu1;
Orders* ord; 

int main(int argc,char **argv){
    int stat;
    int pid = 0;
    mng.checkArgsAndPrintData(argc,argv);

    int shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
    void* shmPointer = mng.getShmPointer(shMenuKey);
    menu1 = new(shmPointer)menu(mng.getSimulationArguments(NUM_OF_ITEMS));

    shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
    shmPointer = mng.getShmPointer(shMenuKey);
    ord = new(shmPointer)Orders(mng.getSimulationArguments(NUM_OF_ITEMS));

    char msg[64];
    memset(msg,'\0',64);
    sprintf(msg,"Main process ID %d start",getpid());
    mng.switchTime();
    mng.printTimeWithMsg(msg);
    
    menu1->printManu();

    memset(msg,'\0',64);
    strcpy(msg,"Main process starts creating sub-processes");
    mng.printTimeWithMsg(msg);

    pid = mng.execWaiter();

    if(pid){
        mng.execCust(pid);
        //Here the parent waits for the sub-processes to end
        cout << "THE END\n";
        while ((waitpid(-1, &stat, 0)) > 0);
    }
    return OK;
}
