#include "defs.h"

restMng mng; 

int main(int argc,char **argv){
    mng.checkArgsAndPrintData(argc,argv);

    int menuSharedMemory = mng.makeSharedMemory(1024,1);

    return OK;
}
