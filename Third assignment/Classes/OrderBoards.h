#ifndef ORDER_BOARDS_H
#define ORDER_BOARDS_H
#include "../defs.h"
#include "Customer.h"
//Defined in main
extern restMng mng;

class Orders
{
	private:
		customer *custs;
		int numOfCustomers;
		int shmid;
	public:
	Orders(int val){
		numOfCustomers = val;

		int shmCust = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
		void* shmPointer = mng.getShmPointer(shmCust);
		int listSize = sizeof(customer)*numOfCustomers;
		custs = static_cast<customer*>(::operator new(listSize,shmPointer));

		for(int i = 0; i < numOfCustomers;i++){
			new (&custs[i])customer();
		}
	}
	
	~Orders() {}

};


#endif