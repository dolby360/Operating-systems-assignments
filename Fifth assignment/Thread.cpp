#include "Threads.h"

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
	char **ips = new char*[MAXIMUM_IPS];
	for(int i = 0; i < MAXIMUM_IPS; i++)ips[i] = new char[MAXIMUM_IP_LENGTH];
	int adressesfound;
	
	if(dnslookupAll(host_name.c_str(), ips, MAXIMUM_IPS, &adressesfound) == UTIL_SUCCESS){
		char **fixedips = new char*[adressesfound];
		for(int i = 0; i < adressesfound; i++){
            fixedips[i] = ips[i];
        }
		res = new hostsAndIPstorage(host_name,fixedips,adressesfound);
	}
	else
		res = new hostsAndIPstorage(host_name,NULL,adressesfound);

	for(int i = adressesfound; i < MAXIMUM_IPS; i++){
		delete []ips[i];
	}
	
	rt->result->add(res);
	pthread_cond_signal(cond);
}

void RequestTask::Action(void *arg){
	ifstream file(fileName);
	string name;
	while (getline(file,name)){
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

ThreadPool::ThreadPool(int _numOfThreads){
	numOfThreads = _numOfThreads;
	threads = new pthread_t[_numOfThreads];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

ThreadPool::ThreadPool(int _numOfThreads, SafeQeueu<Task*> *_queue){
	numOfThreads = _numOfThreads;
	queue = _queue;
	threads=new pthread_t[_numOfThreads];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

void ThreadPool::start(){
	for(int i = 0; i < numOfThreads; i++){
		int Res = pthread_create(&threads[i], &attr, startWorking, (void *)this);
		if (Res){
			cerr << "Error with executing work " << Res << endl;
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
	for(int i = 0; i < numOfThreads; i++){	
		void *status;
		int Res = pthread_join(threads[i], &status);
		if (Res){
			cerr << "Error with join returned: " << Res << endl;
			exit(-1);
		}
	}
	if(queue){
		delete queue;
	}
	if(threads){ 
		delete threads;
	}
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
}
