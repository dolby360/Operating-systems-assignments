#ifndef DISH_H
#define DISH_H
#include "defs.h"

class Dish{
private:
    int id,totalorders;
    string name;
    float price;
public:
    Dish(int i, float p){
        price=p;
        totalorders=0;

        switch((id=i)){
            case 0:
                name="Pizza";
                break;
            case 1:
                name="Salad";
                break;
            case 2:
                name="Burger";
                break;
            case 3:
                name="Pasta";
                break;
            case 4:
                name="Pie";
                break;
            case 5:
                name="Omelet";
                break;
            case 6:
                name="Coffee";
                break;
            case 7:
                name="Fish";
                break;
            case 8:
                name="Fries";
                break;
            case 9:
                name="Lasagna";
                break;
    }
}	
    int getID() {return id;}
    float getPrice() {return price;}
    string getName() {return name;}
    int getTotal() {return totalorders;}
    void incOreder(int a) { totalorders+=a;}
};

#endif