#ifndef DISH_H
#define DISH_H
#include "../defs.h"
using namespace std;


typedef enum dishes{
    Goulash = 0,
    Beans = 1,
    Burger = 2,
    Spoonbread = 3,
    Pie = 4,
    Blini = 5,
    Caviar = 6,
    Kasha = 7,
    Kulich = 8
}dishes;

class Dish{
private:
    int id,totalorders;
    string name;
    float price;
public:
    Dish(int i, float p){
        price = p;
        totalorders = 0;
        switch(i){
            case Goulash:   name = "Goulash";   break;
            case Beans:     name = "Beans";     break;
            case Burger:    name = "Burger";    break;
            case Spoonbread:name = "Spoonbread";break;
            case Pie:       name = "Pie";       break;
            case Blini:     name = "Blini";     break;
            case Caviar:    name = "Caviar";    break;
            case Kasha:     name = "Kasha";     break;
            case Kulich:    name = "Kulich";    break;
        }
    }
	
    int getID() {return id;}
    float getPrice() {return price;}
    string getName() {return name;}
    int getTotal() {return totalorders;}
    void incOreder(int a) { totalorders+=a;}
};

#endif