
#ifndef MULLTI_LOOKUP
#define MUILTI_LOOKUP

int getNumberOfLinesInAllFiles(int numberOfFiles,char *filesNames[]);
void checkHowManyAvalibleFiles(int *inputAvailableFiles,int argc, char *argv[]);
void errorMsgAndExit(string msg);
RequestTask **assignRequestTasks(int argc, char *argv[], int *reqTaskNum, SafeQeueu<Task*> *RequestQueue, storageManager *result,pthread_mutex_t *consoleMutex);

#endif