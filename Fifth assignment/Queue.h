#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "defs.h"

class hostsAndIPstorage{
	private:
		string hostName;
		char** ips;
		int IPsAmountForThisHost;
		bool dumped;
	public:
		~hostsAndIPstorage();
		hostsAndIPstorage(string host_name, char** ip ,int amount){
			hostName = host_name;
			ips = ip;
			IPsAmountForThisHost = amount;
			dumped = false;
		};
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
        int size;
		char* outputFile;
        hostsAndIPstorage **storage;
        int index;
		//This mutex helps us to make a safe access
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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

class Task
{
	protected:
		storageManager *result;
	public:
        //pure virtual class to make this class abstract.
		virtual void Action(void *arg) = 0;
		virtual ~Task(){};
};

template <class T>
class Node{
	public:
		T data;
		Node *next;
};

template <class T>
class SafeQeueu{
	private:
        Node<T> *back = NULL;
        Node<T> *front = NULL;
        int size = 0;
		//This mutex helps us to make a safe queue
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    public:
    	~SafeQeueu(){
			pthread_mutex_destroy(&mutex);
		}
        void push(T data);
        T pop();
        bool isEmpty();
};

template <class T>
bool SafeQeueu<T>::isEmpty(){
	pthread_mutex_lock(&mutex);
	bool full = (size==0);
	pthread_mutex_unlock(&mutex);
	return full;
}

template <class T>
void SafeQeueu<T>::push(T thread){
	Node<T> *toPush = new Node<T>();
	//Making node to push in the queue
	toPush->data = thread;
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
	//Because this is FIFO
	Node<T> *temp = front;
	front = front->next;
	size--;
	pthread_mutex_unlock(&mutex);
	T d = temp->data;
	delete temp;
	return d;
}

#endif