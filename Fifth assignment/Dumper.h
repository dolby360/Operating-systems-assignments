#ifndef DUMPER_H
#define DUMPER_H
#include "Queue.h"
#define NUMBER_OF_THREAD_IN_THREAD_POOL 5

class DumperWorker{
private:
    int threadNumber;
    pthread_mutex_t *mutex;
    hostsAndIPstorage** myStorage; 
public:
    DumperWorker(int number,pthread_mutex_t *_mutex, hostsAndIPstorage** _myStorage ){
        threadNumber = number;
        mutex = _mutex;
        myStorage = _myStorage;
    }
    int getThreadNumber(){ return threadNumber; }
};


class DumperThreadPool{
private:
    hostsAndIPstorage** myStorage; 
    int sizeOfMyStorage;
    DumperWorker* d;
    char* outputFile;
public:
    DumperThreadPool(storageManager *resultArray,char* _outputFile);

    char* getOutputFile(){return outputFile;}
    static void* execute(void *data){
        DumperThreadPool* dp = (DumperThreadPool*)data;
        DumperWorker *w = (DumperWorker*)dp->d;
        int workerNUmber = w->getThreadNumber();
        char* outputFile = dp->getOutputFile();
        return NULL;
    }


    ~DumperThreadPool(){
        for(int i = 0; i < sizeOfMyStorage; i++){
            delete myStorage[i];
        }
        delete myStorage;
    }
};

#endif