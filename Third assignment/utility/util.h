#ifndef UTIL_H
#define UTIL_H
#include "../defs.h"

class util{
private:
public:
    util(){}
    ~util(){}

static int getRandomNumberBetweenTwoWithaChanceOfHalf(int a,int b){
    if(rand()%2){return a;}
    return b;
}
static bool getTrueOrFalseWithaChanceOfHalf(){
    return rand()%2 == 0;
}
};

#endif