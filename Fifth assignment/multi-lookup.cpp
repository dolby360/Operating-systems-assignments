#include <stdio.h>
#include <iostream>
#include "defs.h"
#include "Threads.h"
#include "multi-lookup.h"
#include "Dumper.h"

using namespace std;

int main(int argc, char *argv[]){
	int reqTaskNum = 0; 
    int inputAvailableFiles = 0;
    int amountOfLines = getNumberOfLinesInAllFiles(argc, argv);
    SafeQeueu<Task*> *RequestQueue = new SafeQeueu<Task*>;
	pthread_mutex_t consoleMutex = PTHREAD_MUTEX_INITIALIZER;
    storageManager *resultArray = new storageManager(amountOfLines, argv[argc-1]);
    checkHowManyAvalibleFiles(&inputAvailableFiles,argc,argv);
    reqTaskNum = inputAvailableFiles;
    RequestTask** reqTasks = assignRequestTasks(argc, argv, &reqTaskNum, RequestQueue, resultArray,&consoleMutex);
    ThreadPool *requestThreadPool = new ThreadPool(reqTaskNum);
	ThreadPool *resolverThreadPool = new ThreadPool(MAXIMUM_THREADS,RequestQueue);
	requestThreadPool->start();
	resolverThreadPool->start();
	for(int i = 0; i < reqTaskNum; i++){ requestThreadPool->addTask(reqTasks[i]); }	
	while(!resultArray->isFull());
	requestThreadPool->stop();
	resolverThreadPool->stop();
	delete requestThreadPool;
	delete resolverThreadPool;
	DumperThreadPool s(resultArray,argv[argc - 1]);
    return 0;
}

RequestTask **assignRequestTasks(int argc, char *argv[], int *reqTaskNum, SafeQeueu<Task*> *RequestQueue, storageManager *result,pthread_mutex_t *consoleMutex){
    RequestTask **tasks=NULL;
    tasks = new RequestTask*[*reqTaskNum];
    for(int i=1, j=0; i<argc-1; i++){
        ifstream input(argv[i]);
        if(!input.fail()){	
            input.close();
            tasks[j++]=new RequestTask(argv[i], RequestQueue, result, consoleMutex);
        }
    }	
	return tasks;
}

void checkHowManyAvalibleFiles(int *inputAvailableFiles,int argc, char *argv[]){
	if (argc >= 3){
		for(int i=1 ;i < argc-1 ;i++){
			ifstream input(argv[i]);
			if(input.fail()){
				cerr << "Failed to open file \"" << argv[i] << "\" Skipping..." << endl;
			}else{	
				input.close();
				(*inputAvailableFiles)++;
			}
		}
		if(*inputAvailableFiles == 0){ errorMsgAndExit("No valid input found!"); }
		else{ /*cout << "number of avilable files -  " << *inputAvailableFiles << endl; */}
		ifstream output(argv[argc-1]);
		if(output.fail()){ errorMsgAndExit("Cannot open the output file - " + string(argv[argc-1]) + "\nYou have to provide output file"); }
		output.close();
	}
	else{ errorMsgAndExit("Not enough input arguments!"); }
}

int getNumberOfLinesInAllFiles(int numberOfFiles,char *filesNames[]){
    int i = 1;
    int numberOfLines = 0;
    string wholeLine;
    for(; i < numberOfFiles - 1; i++){
        //Next line for debug.
        //cout << filesNames[i] << endl;
        ifstream input(filesNames[i]);
		if(!input.fail()){
            wholeLine.clear();
            //cout << " - " << wholeLine << endl;
			while (getline(input,wholeLine)) {
				numberOfLines++;
            }
			input.close();
		}
    }
    return numberOfLines;
}

void errorMsgAndExit(string msg){
    cerr << msg << endl;
    exit(1);
}