#ifndef MENU_H
#define MENU_H
#include "../defs.h" 
#include "Dish.h"

//Defined in main.
extern restMng mng;

class menu{
private:
    int numOfDish;
    Dish* allDish;
    int shmId;
public:
    menu(int manItems){
        float randomPrice;
        srand(time(NULL));
        numOfDish = manItems;

        int shmDishKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
        void* shmPointer = mng.getShmPointer(shmDishKey);
        int listSize = sizeof(Dish)*manItems;
        
        
        //Shared memory for manue list.
        allDish = static_cast<Dish*>(::operator new(listSize,shmPointer));
        for(int i = 0;i < manItems;i++){
            randomPrice = ( (rand() % 793) + 7 )/8;
            if(randomPrice < 10){randomPrice +=10;}
            new(&allDish[i])Dish(i,randomPrice);
            //Next lines for debug;
            // printf("%f",randomPrice);
            // cout << " - " << allDish[i].getPrice() <<endl;
        }
    }
};

#endif