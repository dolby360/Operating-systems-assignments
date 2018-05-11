#ifndef DUMPER_H
#define DUMPER_H
#include "Queue.h"
#define NUMBER_OF_THREAD_IN_THREAD_POOL 5

class DumperWorker{
private:
    int threadNumber;
    pthread_mutex_t *mutex;
    hostsAndIPstorage** myStorage; 
    char* outputFile;
    int sizeOfMyStorage;
public:
    DumperWorker(int number,pthread_mutex_t *_mutex, hostsAndIPstorage** _myStorage,
    char* _fileName, int _sizeOfMyStorage){
        threadNumber = number;
        mutex = _mutex;
        myStorage = _myStorage;
        outputFile = _fileName;
        sizeOfMyStorage = _sizeOfMyStorage;
    }
    int getThreadNumber(){ return threadNumber; }
    pthread_mutex_t* getnutex(){ return mutex; }
    char* getOutputFile(){return outputFile;}
    int get_sizeOfMyStorage(){return sizeOfMyStorage;}
    hostsAndIPstorage** get_hostsAndIPstorage(){return myStorage;}
};

class DumperThreadPool{
private:
    hostsAndIPstorage** myStorage; 
    int sizeOfMyStorage;
    DumperWorker* d;
    char* outputFile;
public:
    DumperThreadPool(storageManager *resultArray,char* _outputFile);
    static void* execute(void *data){
        DumperWorker *w = (DumperWorker*)data;
        pthread_mutex_t *mutex = w->getnutex();
        pthread_mutex_lock(mutex);
        char* outputFile = w->getOutputFile();
        //int workerNUmber = w->getThreadNumber();
        ofstream file(outputFile, ios:: app);
        file.close();
        pthread_mutex_unlock(mutex);

        // cout << "\n\n\n\n";
        for(int i = 0;i < w->get_sizeOfMyStorage();i++){
            pthread_mutex_lock(mutex);
            hostsAndIPstorage* h_and_ip_storage = w->get_hostsAndIPstorage()[i];
            if(!(h_and_ip_storage->isDumped())){
                h_and_ip_storage->setDumped();
                ofstream file(outputFile, ios:: app);
                file << h_and_ip_storage->getHostName();
                cout << h_and_ip_storage->getHostName();
                file << ", ";
                cout << ", ";

                if(h_and_ip_storage->getips() != 0){
                    char** arr = h_and_ip_storage->getips();
                    for(int j = 0; j < h_and_ip_storage->getIPamount(); j++){
                        file << arr[j];
                        cout << arr[j];
                        file << ", ";
                        cout << ", ";
                    } 
                    file << "\n";
                    cout << "\n";
                }else{
                    file << "Not found any IPs\n";
                    cout << "Not found any IPs\n";
                }

                file.close();
                //cout << i << endl;
            }            
            pthread_mutex_unlock(mutex);
        }



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