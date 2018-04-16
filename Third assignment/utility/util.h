#ifndef UTIL_H
#define UTIL_H
#include "../defs.h"
#include <cstdlib>

class util{
private:
    
public:
    util(){}
    ~util(){}

    static int getRandomNumberBetweenTwoWithaChanceOfHalf(int min,int max){
        //std::cout << "-------------------"<<rand()<< "   " << rand() << "   " << rand() << std::endl;
        return (min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max-min))));
        // if(rand()%2){return a;}
        // return b;
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
};

#endif