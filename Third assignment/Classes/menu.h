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
public:
    menu(int manItems){
        numOfDish = manItems;
        int shMenuKey = mng.makeSharedMemory(SHARED_MEMORY_SIZE);
    }
};

#endif