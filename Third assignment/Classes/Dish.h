#ifndef DISH_H
#define DISH_H
#include "../defs.h"
using namespace std;

typedef enum dishes{
    Steak = 0,
    Beans = 1,
    Burger = 2,
    Shrimps = 3,
    Pie = 4,
    Blini = 5,
    Caviar = 6,
    Kasha = 7,
    Kulich = DISH_AMOUNT
}dishes;

class Dish{
private:
    int id;
    int totalOrders;
    string name;
    float price;
public:
    Dish(int identity, float p){
        price = p;
        totalOrders = 0;
        id = identity;

        switch(identity){
            case Steak:     name = "Steak";     break;
            case Beans:     name = "Beans";     break;
            case Burger:    name = "Burger";    break;
            case Shrimps:   name = "Shrimps";   break;
            case Pie:       name = "Pie";       break;
            case Blini:     name = "Blini";     break;
            case Caviar:    name = "Caviar";    break;
            case Kasha:     name = "Kasha";     break;
            case Kulich:    name = "Kulich";    break;
        }
    }
	
    int getID() {return id;}
    float getPrice() {return price;} 
    string getName(int i = 0) {
        if(name == ""){
            switch(i){
                case Steak:     return "Steak";   
                case Beans:     return "Beans";   
                case Burger:    return "Burger";  
                case Shrimps:   return "Shrimps"; 
                case Pie:       return "Pie";     
                case Blini:     return "Blini";   
                case Caviar:    return "Caviar";  
                case Kasha:     return "Kasha";   
                case Kulich:    return "Kulich";  
            }
        }
        return name;
    }
    int getTotal() {return totalOrders;}
    int getAmountOfOrd(){return totalOrders;}
    void addOneToTotalOrders() { totalOrders++; }
};

#endif