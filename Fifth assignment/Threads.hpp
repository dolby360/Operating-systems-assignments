#ifndef THREADS_HPP
#define THREADS_HPP

#include "Queue.hpp"
#include <unistd.h>

class ThreadPool
{
	private:
		SafeQeueu<Task*> *queue = new SafeQeueu<Task*>;
		pthread_t *threads;
		pthread_attr_t attr;
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		int numOfThreads;
		bool stopflag=false;
		
	public:
		ThreadPool(int n);
		ThreadPool(int n, SafeQeueu<Task*> *q);
		~ThreadPool();
		void addTask(Task *t){
			queue->push(t);
			};
		void start();
		void stop(){stopflag=true;};
		
		
		static void* workingThread(void *data)
		{
			ThreadPool *tp=(ThreadPool*)data;
			SafeQeueu<Task*> *queue= (SafeQeueu<Task*>*)(tp->queue);
			pthread_mutex_t *mutex= (pthread_mutex_t*)&(tp->mutex);
			bool *stopflag=&(tp->stopflag);
			
			while(!(*stopflag)){
				pthread_mutex_lock(mutex); //lock
				if(!queue->isEmpty()){
					Task *t=(Task*)queue->pop();
					pthread_mutex_unlock(mutex); //unlock
					t->Action(t);	
				}else{
					pthread_mutex_unlock(mutex); //unlock	
				}
			}
			pthread_exit(NULL);
			return NULL;
		}
};
class ResolverTask:public Task
{
	private:
		string hostName;
		pthread_cond_t *cond;
	public:
		ResolverTask(string n, SafeArray *r, pthread_cond_t *c){
			hostName = n;
			result = r;
			cond =c;
		};
		virtual void Action(void *arg);
		~ResolverTask(){}
};

class RequestTask:public Task
{
	private:
		char* fileName;
		SafeQeueu<Task*> *queue;
		pthread_mutex_t *consoleMutex;
	public:
		RequestTask(char* name, SafeQeueu<Task*> *q, SafeArray *r, pthread_mutex_t *cm);
		virtual void Action(void *arg);
		virtual char* getName() {return fileName;};
		~RequestTask(){}
};

#endif