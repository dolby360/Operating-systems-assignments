#include "Dumper.h"

DumperThreadPool::DumperThreadPool(storageManager *resultArray,char* _outputFile){
        outputFile = _outputFile;
        sizeOfMyStorage = resultArray->getSize();
        myStorage = new hostsAndIPstorage*[sizeOfMyStorage];
        for (int i = 0; i < resultArray->getSize() ;i++){
            myStorage[i] = resultArray->getByIndex(i);
            //Next line for debugging.
            //cout << myStorage[i]->getHostName() << endl;
        }
        pthread_t dumperThreads[NUMBER_OF_THREAD_IN_THREAD_POOL];
        pthread_attr_t attr;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        int temp_arg[NUMBER_OF_THREAD_IN_THREAD_POOL];

        for(int i = 0; i < NUMBER_OF_THREAD_IN_THREAD_POOL; i++){
            temp_arg[i] = i;
            d = new DumperWorker(temp_arg[i],&mutex,myStorage,outputFile,sizeOfMyStorage);
            int Res = pthread_create(&dumperThreads[i], &attr, execute, (void *)d);
            if (Res != OK){
                cerr << "ERROR with pthread_create " << Res << endl;
                exit (ERROR);
            }
        }
        void *status;
        int Res;
        for(int i = 0; i < NUMBER_OF_THREAD_IN_THREAD_POOL; i++){
            Res = pthread_join(dumperThreads[i], &status);
            if (Res != OK){
                cerr << "ERROR with pthread_join " << Res << endl;
                exit(ERROR);
            }
        }
        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&mutex);
}

