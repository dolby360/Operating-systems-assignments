#include "Threads.hpp"

RequestTask::RequestTask(char* name, SafeQeueu<Task*> *_queue, storageManager *storage, pthread_mutex_t *cm){
	fileName = name;
	queue = _queue;
	result = storage;
	consoleMutex = cm;
}

void ResolverTask::Action(void *arg){
	ResolverTask *rt = (ResolverTask *)arg;
	string host_name = rt->hostName;
	pthread_cond_t *cond = rt->cond;
	hostsAndIPstorage *res;
	char **ips = new char*[MAX_IPS];
	for(int i = 0; i < MAX_IPS; i++)ips[i] = new char[MAX_IP_LEN];
	int adressesfound;
	
	if(dnslookupAll(host_name.c_str(), ips, MAX_IPS, &adressesfound) == UTIL_SUCCESS){
		char **fixedips = new char*[adressesfound];
		for(int i = 0; i < adressesfound; i++){
            fixedips[i] = ips[i];
        }
		res = new hostsAndIPstorage(host_name,fixedips,adressesfound);
	}
	else
		res = new hostsAndIPstorage(host_name,NULL,adressesfound);

	for(int i = adressesfound; i < MAX_IPS; i++){
		delete []ips[i];
	}
	
	rt->result->add(res);
	pthread_cond_signal(cond);
}

void RequestTask::Action(void *arg)
{
	ifstream file(fileName);
	string name;
	while (getline(file,name))
	{
		pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
		ResolverTask *rt = new ResolverTask(name,result,&cond);
		queue->push(rt);
		pthread_mutex_lock(consoleMutex);
		pthread_cond_wait(&cond,consoleMutex);
		hostsAndIPstorage *r = result->getByHostName(name);
		char **ips = r->getips();
		int IPsAmount = r->getIPamount();
		cout << name << ", ";
		if(!IPsAmount){
			cout << "No IP was found for this host" << endl;
        }
		else{	
			for(int i=0; i < IPsAmount; i++){
				cout << ips[i];
				if(i < IPsAmount-1)
					cout << ", ";
			}
			cout << endl;
		}
		pthread_mutex_unlock(consoleMutex);
	}
	file.close();
}

ThreadPool::ThreadPool(int n)
{
	numOfThreads=n;
	threads=new pthread_t[n];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

ThreadPool::ThreadPool(int n, SafeQeueu<Task*> *q)
{
	numOfThreads=n;
	queue=q;
	threads=new pthread_t[n];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

void ThreadPool::start(){
	for(int t = 0; t < numOfThreads; t++){
		int rc = pthread_create(&threads[t], &attr, startWorking, (void *)this);
		if (rc){
			cerr << "ERROR; return code from pthread_create() is " << rc << endl;
			exit (1);
		}
	}
}


bool storageManager::isFull(){
	pthread_mutex_lock(&mutex); 
	bool full = (index >= (size-1));
	pthread_mutex_unlock(&mutex);
	return full;
}

ThreadPool::~ThreadPool(){
	// wait for child threads to finish their work
	for(int t=0; t<numOfThreads; t++){	
		void *status;
		int rc = pthread_join(threads[t], &status);
		if (rc){
			cerr << "ERROR; return code from pthread_join() is " << rc << endl;
			exit(-1);
		}
	}
	if(queue) delete queue;
	if(threads) delete threads;
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
}
