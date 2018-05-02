#include <stdio.h>
#include <iostream>

using namespace std;
int main(int argc, char *argv[]){
    cout << "Hello World" << endl;

    SafeQeueu<Task*> *RequestQueue = new SafeQeueu<Task*>;
	pthread_mutex_t consoleMutex=PTHREAD_MUTEX_INITIALIZER;
    return 0;
}