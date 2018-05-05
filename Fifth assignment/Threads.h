#ifndef THREADS_HPP
#define THREADS_HPP

#include "Queue.h"
#include <unistd.h>

class ThreadPool{
	private:
		SafeQeueu<Task*> *queue = new SafeQeueu<Task*>;
		pthread_attr_t attr;
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_t *threads;
		int numOfThreads;
		bool killMe = false;
		
	public:
		ThreadPool(int _numOfThreads, SafeQeueu<Task*> *_queue);
		ThreadPool(int _numOfThreads);
		~ThreadPool();
		void addTask(Task *t){
			queue->push(t);
			};
		void stop(){ killMe = true; };
		void start();

		
		//The spawning task can't be non static...
		//Something with c signature....
		static void* startWorking(void *data){
			//The way we get over this, is by sending the instance.
			//hahhahahahaha 
			ThreadPool *instanceOfTheThreadPool = (ThreadPool*)data;
			SafeQeueu<Task*> *queue= (SafeQeueu<Task*>*)(instanceOfTheThreadPool->queue);
			pthread_mutex_t *mutex = (pthread_mutex_t*)&(instanceOfTheThreadPool->mutex);
			bool *killMe = &(instanceOfTheThreadPool->killMe);
			
			while(!(*killMe)){
				pthread_mutex_lock(mutex);
				if(!queue->isEmpty()){
					Task *t = (Task*)queue->pop();
					pthread_mutex_unlock(mutex);
					t->Action(t);	
				}else{
					pthread_mutex_unlock(mutex);	
				}
			}
			pthread_exit(NULL);
			return NULL;
		}
};

class ResolverTask:public Task{
	private:
		string hostName;
		pthread_cond_t *cond;
	public:
		ResolverTask(string host_name, storageManager * storage, pthread_cond_t *_cond){
			hostName = host_name;
			result = storage;
			cond = _cond;
		};
		//Implementing the pure virtual function.  
		void Action(void *arg);
		~ResolverTask(){}
};

class RequestTask:public Task {
	private:
		char* fileName;
		SafeQeueu<Task*> *queue;
		pthread_mutex_t *consoleMutex;
	public:
		RequestTask(char* name, SafeQeueu<Task*> *_queue, storageManager *storage, pthread_mutex_t *cm);
		virtual void Action(void *arg);
		virtual char* getName() {return fileName;};
		~RequestTask(){}
};

#endif