#include "Queue.hpp"

storageManager::~storageManager(){
	pthread_mutex_destroy(&mutex);
	for(int i=0; i < size; i++){
		if(storage[i]){
			delete storage[i];
		}
	}
	if(storage){
		delete[] storage;
	}
}

hostsAndIPstorage::~hostsAndIPstorage(){
	if(ips){	
		for(int i=0; i < this->getIPamount(); i++){
			if(ips[i]){
				delete []ips[i];
			}else{
				delete []ips;
			}
		}
	}
}

hostsAndIPstorage* storageManager::getByHostName(string host_name){
	hostsAndIPstorage* obj = NULL;
	pthread_mutex_lock(&mutex); 
	for(int i=0; i < index; i++)
		if(storage[i]->getHostName() == host_name){
			obj = storage[i];
			pthread_mutex_unlock(&mutex);
			return obj;
		}
	pthread_mutex_unlock(&mutex); 
	return obj;
}
void storageManager::add(hostsAndIPstorage* arr){
	pthread_mutex_lock(&mutex);
	storage[index] = arr;
	index++;
	pthread_mutex_unlock(&mutex);
}
