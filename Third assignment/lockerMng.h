#ifndef LOCKER_MNG_H
#define LOCKER_MNG_H

#include "defs.h"
#include "semaphore.h"

typedef struct semaphores{
	int stdOutMutex;
	int countAllClients;
	int menuReaderCounter;
	int orderReaderCounter;
	int writerMenuCounter;
	int writeOrdersCount;
	int readMutexMenu;
	int readMutexOrders;
	int writeMutexMenu;
	int writeMutexOrders;
	int readTryMenu;
	int readTryOrders;
	int resourceMenu;
	int resourceOrders;
    int takeaNap;
}semaphores;

static uint newKey = 0;

class lockerMng{
protected:
    int clientsAmount;
    semaphores Sems;
public:
    int getNewKey(){return newKey++;}
    virtual void initAllSemaphores(){
        Sems.stdOutMutex=makeSemaphore(5,1);
        //I used this semaphore to assure that all customers get what they wants
        Sems.countAllClients=makeSemaphore(6,clientsAmount);
        Sems.menuReaderCounter=makeSemaphore(7,0);
        Sems.orderReaderCounter=makeSemaphore(8,0);
        Sems.writerMenuCounter=makeSemaphore(9,0);
        Sems.writeOrdersCount=makeSemaphore(10,0);
        Sems.readMutexMenu=makeSemaphore(11,1);
        Sems.readMutexOrders=makeSemaphore(12,1);
        Sems.writeMutexMenu=makeSemaphore(13,1);
        Sems.writeMutexOrders=makeSemaphore(14,1);
        Sems.readTryMenu=makeSemaphore(15,1);
        Sems.readTryOrders=makeSemaphore(16,1);
        Sems.resourceMenu=makeSemaphore(17,1);
        Sems.resourceOrders=makeSemaphore(18,1);
        Sems.takeaNap=makeSemaphore(19,1);
        // std::cout << "1)----------------" << getSemVal(Sems.menuReaderCounter,0) << std::endl;
        // std::cout << "2)----------------" << getSemVal(Sems.writeMutexMenu,0)<< std::endl;
        // std::cout << "3)----------------" << getSemVal(Sems.stdOutMutex,0)<< std::endl;
        // std::cout << "4)----------------" << getSemVal(Sems.writeMutexOrders,0)<< std::endl;
        // std::cout << "5)----------------" << getSemVal(Sems.writeOrdersCount,0)<< std::endl;
        // std::cout << "6)----------------" << getSemVal(Sems.orderReaderCounter,0)<< std::endl;
        // std::cout << "7)----------------" << getSemVal(Sems.takeaNap,0)<< std::endl;
    }
    void menuWriteEntry(){
        down(Sems.writeMutexMenu);
        up(Sems.writerMenuCounter);
        if(getSemVal(Sems.writerMenuCounter,0)==1)
            down(Sems.readTryMenu);
        up(Sems.writeMutexMenu);
        down(Sems.resourceMenu);
    }
    void menuWriteExit(){
        up(Sems.resourceMenu);
        down(Sems.writeMutexMenu);
        down(Sems.writerMenuCounter);
        if(getSemVal(Sems.writerMenuCounter,0)==0)
            up(Sems.readTryMenu);
        up(Sems.writeMutexMenu);
    }
    void ordersWriteEntry(){
        down(Sems.writeMutexOrders);
        up(Sems.writeOrdersCount);
        if(getSemVal(Sems.writeOrdersCount,0)==1)
            down(Sems.readTryOrders);
        up(Sems.writeMutexOrders);
        down(Sems.resourceOrders);
    }
    void ordersWriteExit(){
        up(Sems.resourceOrders);
        down(Sems.writeMutexOrders);
        down(Sems.writeOrdersCount);
        if(getSemVal(Sems.writeOrdersCount,0)==0)
            up(Sems.readTryOrders);
        up(Sems.writeMutexOrders);
    }
    void menuReadEntry(){
        down(Sems.readTryMenu);
        down(Sems.readMutexMenu);
        up(Sems.menuReaderCounter);
        if(getSemVal(Sems.menuReaderCounter,0)==1)
            down(Sems.resourceMenu);
        up(Sems.readMutexMenu);
        up(Sems.readTryMenu);
    }
    void menuReadExit(){
        down(Sems.readMutexMenu);
        down(Sems.menuReaderCounter);
        if(getSemVal(Sems.menuReaderCounter,0)==0)
            up(Sems.resourceMenu);
        up(Sems.readMutexMenu);
    }
    void ordersReadEntry(){
        down(Sems.readTryOrders);
        down(Sems.readMutexOrders);
        up(Sems.orderReaderCounter);
        if(getSemVal(Sems.orderReaderCounter,0)==1)
            down(Sems.resourceOrders);
        up(Sems.readMutexOrders);
        up(Sems.readTryOrders);
    }
    void ordersReadExit(){
        down(Sems.readMutexOrders);
        down(Sems.orderReaderCounter);
        if(getSemVal(Sems.orderReaderCounter,0)==0)
            up(Sems.resourceOrders);
        up(Sems.readMutexOrders);
    }
};

#endif