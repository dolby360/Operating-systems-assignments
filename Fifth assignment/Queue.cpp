#include "Queue.hpp"

SafeArray::~SafeArray()
{
	pthread_mutex_destroy(&mutex);
	for(int i=0;i<size;i++)
		if(arr[i])
			delete arr[i];
	if(arr)
		delete[] arr;
}

hostsAndIPstorage::~hostsAndIPstorage(){
	if(ips){	
		for(int i=0; i < ipsSize; i++){
			if(ips[i]){
				delete []ips[i];
			}else{
				delete []ips;
			}
		}
	}
}

hostsAndIPstorage* SafeArray::getByHostName(string hn){
	hostsAndIPstorage* obj = NULL;
	pthread_mutex_lock(&mutex); 
	for(int i=0; i < index; i++)
		if(arr[i]->getHostName() == hn){
			obj=arr[i];
			break;
		}
	pthread_mutex_unlock(&mutex); //unlock
	return obj;
}
void SafeArray::add(hostsAndIPstorage* r){
	pthread_mutex_lock(&mutex); //lock
	arr[index]=r;
	index++;
	pthread_mutex_unlock(&mutex); //unlock
}
