#ifndef QUEUE_HPP
#define QUEUE_HPP


#include "defs.hpp"

class hostsAndIPstorage{
	private:
		string hostName;
		char** ips;
		int ipsSize;
	public:
		~hostsAndIPstorage();
		hostsAndIPstorage(string host_name, char** ip ,int s){
			hostName = host_name;
			ips = ip;
			ipsSize = s;
		};
		string getHostName(){
			return hostName;
		};
		char** getips(){
			return ips;
		};
		int getipsSize(){
			return ipsSize;
		};
};

class SafeArray{
	private:       
        int size;
        int index=0;
        char* outputFile;
        hostsAndIPstorage **arr;
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    public:
    	SafeArray(int s,char* of){
			size = s; 
			arr = new hostsAndIPstorage*[s];
			outputFile = of;
		};
    	~SafeArray();
		hostsAndIPstorage* getByIndex(int i);
		hostsAndIPstorage* getByHostName(string hn);
		void add(hostsAndIPstorage* r);
		bool isFull();
		char* getoutputFile(){return outputFile;};
		int getSize(){return size;};
};

class Task
{
	protected:
		SafeArray *result;
	public:
        //pure virtual class to make this class abstract.
		virtual void Action(void *arg) = 0;
		virtual ~Task(){};
};

template <class T>
class Node
{
	public:
		T data;
		Node *next;
};

template <class T>
class SafeQeueu
{
	private:
        Node<T> *rear=NULL;
        Node<T> *front=NULL;
        int size=0;
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
	bool f;
	pthread_mutex_lock(&mutex);
	f=(size==0);
	pthread_mutex_unlock(&mutex);
	return f;
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
	Node<T> *temp=front;
	front=front->next;
	size--;
	pthread_mutex_unlock(&mutex);
	T d=temp->data;
	delete temp;
	return d;
}

#endif