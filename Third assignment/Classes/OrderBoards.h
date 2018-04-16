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
			custs[i].setDone(true);
		}
	}
	~Orders() {}
	bool getCustomerStatus(int customerId){return custs[customerId].getDone();}
	
	void picRandomItemAndAmount_andPlaceOrder(int &amount,int custID,bool TheCustomerWantToOrder){
		amount = util::chooseRandomlyBetweenRange(1,4);
		if(TheCustomerWantToOrder){
			custs[custID].setItem(util::chooseRandomlyBetweenRange(0,DISH_AMOUNT));
			custs[custID].setAmount(amount);
			custs[custID].setDone(false);
		}else{
			custs[custID].setItem(util::chooseRandomlyBetweenRange(0,DISH_AMOUNT));
		}
	}	
	int getOrderNumber(int customerId){
		return custs[customerId].getItem();
	}
	int checkForOrders(){
		for(int i = 0; i < numOfCustomers;i++){
			if(!custs[i].getDone()){
				return i;
			}
		}
		return -1;
	}
};


#endif