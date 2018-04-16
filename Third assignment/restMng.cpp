#include "restMng.h"
#include "semaphore.h"
#include "Classes/OrderBoards.h"
#include "Classes/menu.h"
#include "lockerMng.h"
using namespace std;

//Defined in main.
extern Orders* ord;
extern menu* menu1;

//False - time pass
//True  - we have more time
bool restMng::WeAreGoodWithTime(){
    gettimeofday(&timNow, 0);
    if(timNow.tv_sec - timStart.tv_sec > this->simuArgs.simuTime){
        return false;
    }
    //For debug
    //cout << timNow.tv_sec - timStart.tv_sec << " - " << this->simuArgs.simuTime << "\n";
    return true;
}
void restMng::initAllSemaphores(){
    clientsAmount = simuArgs.cusCount;
    lockerMng::initAllSemaphores();
}

void restMng::waiterProcess(int wId){
    srand(static_cast <unsigned> (time(0)) ^ (getpid() << 16));
    int custumerIdForOrder = -1;
    char msg[64];
    bool condition = true; 

    down(Sems.stdOutMutex);
    memset(msg,'\0',64);
    sprintf(msg,"Waiter %d: created PID %d PPID %d",wId,getpid(),getppid());
    mng.printTimeWithMsg(msg);
	up(Sems.stdOutMutex);

    while(WeAreGoodWithTime()){
        sleep(util::getRandomNumberBetweenTwoWithaChanceOfHalf(1,2));
        //Critical section READ FROM ORDERS
        ordersReadEntry();
        custumerIdForOrder = ord->checkForOrders();
        ordersReadExit();
        //Critical section done READ FROM ORDERS
    }
}
void restMng::customerProcess(int custId){
    srand(static_cast <unsigned> (time(0)) ^ (getpid() << 16));
    bool customerAlreadyOrderSomething;
    bool customerWantsToOrderSomething;
    char dishOrd[64];
    char msg[128];
    int orderNumber;
    int amountOfItemsOrdered;
    int slp;
    //Critical section WRITE TO STDOUT
    down(Sems.stdOutMutex);
    memset(msg,'\0',128);
    sprintf(msg,"Customer %d: created PID %d PPID %d",custId,getpid(),getppid());
    mng.printTimeWithMsg(msg);
    up(Sems.stdOutMutex);
    //Critical section WRITE TO STDOUT done

    while(WeAreGoodWithTime()){
        
        down(Sems.takeaNap);
        slp = (3 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (7-3))));
        up(Sems.takeaNap);
        sleep(slp);

        //Critical section READ FROM ORDERS
        ordersReadEntry();
        customerAlreadyOrderSomething = ord->getCustomerStatus(custId);
        ordersReadExit();
        //Critical section done READ FROM ORDERS

        if(customerAlreadyOrderSomething){
            customerWantsToOrderSomething = util::getTrueOrFalseWithaChanceOfHalf();

            //Critical section WRITE TO ORDERS
            ordersWriteEntry();
            ord->picRandomItemAndAmount_andPlaceOrder(amountOfItemsOrdered,custId,customerWantsToOrderSomething);//Here we also set the dish
            ordersWriteExit();
            //Critical section done WRITE TO ORDERS

            //Critical section READ FROM ORDERS
            ordersReadEntry();
            orderNumber = ord->getOrderNumber(custId);
            ordersReadExit();
            //Critical section done READ FROM ORDERS

            //Critical section READ FROM MENU
            menuReadEntry();
            strcpy(dishOrd,menu1->getDishNameById(orderNumber).c_str());
            menuReadExit();
            //Critical section done READ FROM MENU

            if(customerWantsToOrderSomething){
                //Critical section WRITE TO STDOUT
                down(Sems.stdOutMutex);
                sprintf(msg,"Customer ID %d: reads a menu about %s (ordered,amount %d)",custId,dishOrd,amountOfItemsOrdered);
                printTimeWithMsg(msg);
                up(Sems.stdOutMutex);
                //Critical section WRITE TO STDOUT done 
            }else{
                //Critical section WRITE TO STDOUT
                down(Sems.stdOutMutex);
                sprintf(msg,"Customer ID %d: reads a menu about %s (doesn't want to order)",custId,dishOrd);
                printTimeWithMsg(msg);
                up(Sems.stdOutMutex);
                //Critical section WRITE TO STDOUT done 
            }
        }
    }
}
pid_t restMng::execWaiter(){
	pid_t pid = fork();
	for(int i=0; i<0; i++)	
		if (pid == 0){	
			waiterProcess(i);
            break;
		}else{
            pid = fork();
        }
	return pid;
}

void restMng::execCust(pid_t pid){
	for(int i=0; i<20; i++)
		if (!(pid=fork())){	
			customerProcess(i);
            break;
		}  
}

void restMng::printTimeWithMsg(char *msg){
    gettimeofday(&timNow, 0);
	float f=(timNow.tv_sec - timStart.tv_sec) + (float)(timNow.tv_usec - timStart.tv_usec)/1000000;
	cout << flush << fixed << setprecision(3) << f << ' ' << msg << endl;
}
void restMng::switchTime(){
    gettimeofday(&timStart,0);
}

int restMng::getSimulationArguments(int index){
    switch(index){
        case SIM_TIME:
            return simuArgs.simuTime;
        case NUM_OF_ITEMS:
            return simuArgs.numOfItems;
        case CUS_COUNT:
            return simuArgs.cusCount;
        case WAITERS_COUNT:
            return simuArgs.witersCount;
    }
    return -1;
}
/*
System call shmat() accepts a shared memory ID, shm_id, 
and attaches the indicated shared memory to the program's address space. 
The returned value is a pointer of type (void *) to the attached shared memory. 
Thus, casting is usually necessary. If this call is unsuccessful, 
the return value is -1. Normally, the second parameter is NULL. If the flag is SHM_RDONLY, 
this shared memory is attached as a read-only memory; otherwise, it is readable and writable. 
*/
void* restMng::getShmPointer(int sharedMemoryId){
    void* pointer;
    pointer = shmat(sharedMemoryId,NULL,0);
    if(!pointer){
        cerr << "shmat failed\n";
        exit(1);
    }
    return pointer;
}
int restMng::makeSharedMemory(int size){
    int _key = getNewKey();
    int sharedMemoryId;
    key_t key = ftok(".",_key);
    sharedMemoryId = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666 );
    if(sharedMemoryId == -1){
        sharedMemoryId = shmget(key, size, 0);
        if(sharedMemoryId == -1){
            cerr << "shmget failed" << endl;
            exit(1);
        }
    }
    return sharedMemoryId;
}
void restMng::setArgs(char **argv){
    this->simuArgs.simuTime = atoi(argv[1]);
    this->simuArgs.numOfItems = atoi(argv[2]);
    this->simuArgs.cusCount = atoi(argv[3]);
    this->simuArgs.witersCount = atoi(argv[4]);
}
void restMng::printData(){
    printf("=====Simulation arguments=====\n");
    cout << "Simulation time: " << simuArgs.simuTime << endl;
    cout << "Menu items count: "<< simuArgs.numOfItems << endl;
    cout << "Customers count: " << simuArgs.cusCount << endl;
    cout << "Waiters count: "   << simuArgs.witersCount << endl;
    printf("==============================\n");
}
 
void restMng::checkArgsAndPrintData(int argc,char **argv){
    if(argc > 5){
        cerr << "Too many arguments\n";
        exit(1);
    }else if(argc < 5){
        cerr << "Too few arguments\n";
        exit(1);
    }
    for(int i = 1;i<argc;i++){
        for(uint j = 0; j < strlen(argv[i]);j++){
            if(argv[i][j] < '0' || argv[i][j] > '9'){
                cerr << "Input arguments are not valid!" << "\n";
                exit(1);
            }
        }
    }
    setArgs(argv);
    if(!(simuArgs.simuTime < 30 && simuArgs.numOfItems <=10 && simuArgs.witersCount<=3 && simuArgs.cusCount<=10)){
        cerr << "Input arguments are not valid!\n";
        exit(1);
    }
    printData();
}