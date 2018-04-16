#ifndef LOCKER_MNG_H
#define LOCKER_MNG_H

#include "defs.h"
#include "semaphore.h"

typedef struct semaphores{
	int stdOutMutex;
	int endSimSem;
	int readMenuCount;
	int readOrdersCount;
	int writeMenuCount;
	int writeOrdersCount;
	int rmutexMenu;
	int rmutexOrders;
	int wmutexMenu;
	int wmutexOrders;
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
        Sems.endSimSem=makeSemaphore(6,clientsAmount);
        Sems.readMenuCount=makeSemaphore(7,0);
        Sems.readOrdersCount=makeSemaphore(8,0);
        Sems.writeMenuCount=makeSemaphore(9,0);
        Sems.writeOrdersCount=makeSemaphore(10,0);
        Sems.rmutexMenu=makeSemaphore(11,1);
        Sems.rmutexOrders=makeSemaphore(12,1);
        Sems.wmutexMenu=makeSemaphore(13,1);
        Sems.wmutexOrders=makeSemaphore(14,1);
        Sems.readTryMenu=makeSemaphore(15,1);
        Sems.readTryOrders=makeSemaphore(16,1);
        Sems.resourceMenu=makeSemaphore(17,1);
        Sems.resourceOrders=makeSemaphore(18,1);
        Sems.takeaNap=makeSemaphore(19,1);
        // std::cout << "1)----------------" << getSemVal(Sems.readMenuCount,0) << std::endl;
        // std::cout << "2)----------------" << getSemVal(Sems.wmutexMenu,0)<< std::endl;
        // std::cout << "3)----------------" << getSemVal(Sems.stdOutMutex,0)<< std::endl;
        // std::cout << "4)----------------" << getSemVal(Sems.wmutexOrders,0)<< std::endl;
        // std::cout << "5)----------------" << getSemVal(Sems.writeOrdersCount,0)<< std::endl;
        // std::cout << "6)----------------" << getSemVal(Sems.readOrdersCount,0)<< std::endl;
        // std::cout << "7)----------------" << getSemVal(Sems.takeaNap,0)<< std::endl;
    }
    void menuWriteEntry(){
        down(Sems.wmutexMenu);
        up(Sems.writeMenuCount);
        if(getSemVal(Sems.writeMenuCount,0)==1)
            down(Sems.readTryMenu);
        up(Sems.wmutexMenu);
        down(Sems.resourceMenu);
    }
    void menuWriteExit(){
        up(Sems.resourceMenu);
        down(Sems.wmutexMenu);
        down(Sems.writeMenuCount);
        if(getSemVal(Sems.writeMenuCount,0)==0)
            up(Sems.readTryMenu);
        up(Sems.wmutexMenu);
    }
    void ordersWriteEntry(){
        down(Sems.wmutexOrders);
        up(Sems.writeOrdersCount);
        if(getSemVal(Sems.writeOrdersCount,0)==1)
            down(Sems.readTryOrders);
        up(Sems.wmutexOrders);
        down(Sems.resourceOrders);
    }
    void ordersWriteExit(){
        up(Sems.resourceOrders);
        down(Sems.wmutexOrders);
        down(Sems.writeOrdersCount);
        if(getSemVal(Sems.writeOrdersCount,0)==0)
            up(Sems.readTryOrders);
        up(Sems.wmutexOrders);
    }
    void menuReadEntry(){
        down(Sems.readTryMenu);
        down(Sems.rmutexMenu);
        up(Sems.readMenuCount);
        if(getSemVal(Sems.readMenuCount,0)==1)
            down(Sems.resourceMenu);
        up(Sems.rmutexMenu);
        up(Sems.readTryMenu);
    }
    void menuReadExit(){
        down(Sems.rmutexMenu);
        down(Sems.readMenuCount);
        if(getSemVal(Sems.readMenuCount,0)==0)
            up(Sems.resourceMenu);
        up(Sems.rmutexMenu);
    }
    void ordersReadEntry(){
        down(Sems.readTryOrders);
        down(Sems.rmutexOrders);
        up(Sems.readOrdersCount);
        if(getSemVal(Sems.readOrdersCount,0)==1)
            down(Sems.resourceOrders);
        up(Sems.rmutexOrders);
        up(Sems.readTryOrders);
    }
    void ordersReadExit(){
        down(Sems.rmutexOrders);
        down(Sems.readOrdersCount);
        if(getSemVal(Sems.readOrdersCount,0)==0)
            up(Sems.resourceOrders);
        up(Sems.rmutexOrders);
    }
};

#endif