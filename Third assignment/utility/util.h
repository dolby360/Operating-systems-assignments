#ifndef UTIL_H
#define UTIL_H
#include "../defs.h"

class util{
private:
    int processID;
public:
    util(){
        processID = getpid();
    }
    ~util(){}

static int getRandomNumberBetweenTwoWithaChanceOfHalf(int a,int b){
    if(rand()%2){return a;}
    return b;
}
static bool getTrueOrFalseWithaChanceOfHalf(){
    return rand()%2 == 0;
}
static int chooseRandomlyBetweenRange(int a,int b){
    if(a > b){
        printf("Invalid args\n");
        return -1;
    }
    if(a == b){
        return a;
    }
    int r = rand();
    int mod = r%b;
    if(mod < a){
        return a;
    }
    return mod;
}
static void setProcessID(int pid){processID = pid;}
static int getProcessID(){return processID;}
};

#endif