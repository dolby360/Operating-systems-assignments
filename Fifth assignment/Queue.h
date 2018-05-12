#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"

class hostsAndIPstorage{
	private:
		char** ips;
		string hostName;	
		int IPsAmountForThisHost;
		bool dumped;
	public:
		
		hostsAndIPstorage(string host_name, char** ip ,int amount){
			hostName = host_name;
			ips = ip;
			IPsAmountForThisHost = amount;
			dumped = false;
		};
		~hostsAndIPstorage();
		int getIPamount(){
			return IPsAmountForThisHost;
		};
		string getHostName(){
			return hostName;
		};
		char** getips(){
			return ips;
		};
		void setDumped(){dumped = true;}
		bool isDumped(){return dumped;}
};

class storageManager{
	private:       
		//This mutex helps us to make a safe access
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        int size;
		char* outputFile;
        hostsAndIPstorage** storage;
        int index;
    public:
    	storageManager(int _size,char* of){
			index = 0;
			storage = new hostsAndIPstorage*[_size];
			outputFile = of;
			size = _size; 
		};
    	~storageManager();

		hostsAndIPstorage* getByHostName(string host_name);
		hostsAndIPstorage* getByIndex(int i);
		char* getoutputFile(){return outputFile;};
		int getSize(){return size;};
		void add(hostsAndIPstorage* arr);
		bool isFull();
};

class Task{
	protected:
		storageManager *result;
	public:
        //pure virtual class to make this class abstract.
		virtual void Action(void *arg) = 0;
		virtual ~Task(){};
};


template <typename T> 
struct LinkedList { 
	T node;
	LinkedList *next;
}; 

template <class T>
class SafeQeueu{
	private:
        int size = 0;
		//This mutex helps us to make a safe queue
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        LinkedList<T> *back = NULL;
        LinkedList<T> *front = NULL;
    public:
    	~SafeQeueu(){
			pthread_mutex_destroy(&mutex);
		}
		T pop();
        void push(T data);
        bool isEmpty();
};

template <class T>
bool SafeQeueu<T>::isEmpty(){
	pthread_mutex_lock(&mutex);
	bool full;
	if(size == 0){
		full = true;
	}else{
		full = false;
	}
	pthread_mutex_unlock(&mutex);
	return full;
}

template <class T>
void SafeQeueu<T>::push(T thread){
	LinkedList<T> *toPush = new LinkedList<T>();
	//Making node to push in the queue
	toPush->node = thread;
	toPush->next = NULL;
	//The pushing must to be safe
	pthread_mutex_lock(&mutex);
	if(front==NULL){
		front=toPush;
	}else{
		back->next = toPush;
	}
	back = toPush;
	size++;
	pthread_mutex_unlock(&mutex);
}

template <class T>
T SafeQeueu<T>::pop(){
	pthread_mutex_lock(&mutex); 
	//Because this its a FIFO
	LinkedList<T> *temp = front;
	front = front->next;
	size--;

	T d = temp->node;
	delete temp;
	pthread_mutex_unlock(&mutex);
	return d;
}

#endif