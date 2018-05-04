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

Result::~Result()
{
	if(ips)
	{	
		for(int i=0;i<ipsSize;i++)
			if(ips[i])
				delete []ips[i];
		delete[] ips;
	}
}

Result* SafeArray::getByHostName(string hn)
{
	Result* obj=NULL;
	pthread_mutex_lock(&mutex); //lock
	for(int i=0;i<index;i++)
		if(arr[i]->getHostName()==hn)
		{
			obj=arr[i];
			break;
		}
	pthread_mutex_unlock(&mutex); //unlock
	return obj;
}
void SafeArray::add(Result* r){
	pthread_mutex_lock(&mutex); //lock
	arr[index]=r;
	index++;
	pthread_mutex_unlock(&mutex); //unlock
}
