#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "defs.hpp"

class hostsAndIPstorage{
	private:
		string hostName;
		char** ips;
		int IPsAmountForThisHost;
	public:
		~hostsAndIPstorage();
		hostsAndIPstorage(string host_name, char** ip ,int amount){
			hostName = host_name;
			ips = ip;
			IPsAmountForThisHost = amount;
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

};

class storageManager{
	private:       
        int size;
		char* outputFile;
        hostsAndIPstorage **storage;
        int index;
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
        Node<T> *rear=NULL;
        Node<T> *front=NULL;
        int size = 0;
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
void SafeQeueu<T>::push(T d){
	Node<T> *temp = new Node<T>();
	temp->data=d;
	temp->next=NULL;
	pthread_mutex_lock(&mutex);
	if(front==NULL)
		front=temp;
	else
		rear->next=temp;
	rear=temp;
	size++;
	pthread_mutex_unlock(&mutex);
}

template <class T>
T SafeQeueu<T>::pop(){
	pthread_mutex_lock(&mutex); 
	Node<T> *temp = front;
	front = front->next;
	size--;
	pthread_mutex_unlock(&mutex);
	T d = temp->data;
	delete temp;
	return d;
}

#endif