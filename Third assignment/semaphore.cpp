#include "semaphore.h"
using namespace std;

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
}ctl_arg;

int makeSemaphore(int key,int value){
    key_t semKey = ftok(".",key);
    int semaphoreId;//to return.
    int status = 0;
    int numsems = 1;

    semaphoreId = semget(semKey,numsems,IPC_CREAT | IPC_EXCL | 0600);
    if( semaphoreId == -1 ){
        if(errno == EEXIST){ semaphoreId = semget(semKey,numsems,0); }
    }else{
        ctl_arg.val = value;
        status = semctl(semaphoreId,0,SETVAL,ctl_arg);
    }
    if(semaphoreId == -1 || status == -1){
        cerr << "makeSemaphore failed\n";
        exit(1);
    }
    return semaphoreId;
}

void up(int semid){
    struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = 1;
	sops.sem_flg = 0;

	if (semop(semid,&sops,1)==-1){
		cerr << "Up Failed" << endl;
		exit(1);
	}//else{spot on!}
}
void down(int semid){
	struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = 0;

	if (semop(semid,&sops,1)==-1){
		cerr << "Down Failed" << endl;
		exit(1);
	}//else{spot on!}
}

int getSemVal(int semid, int semnum){
	return(semctl(semid, semnum, GETVAL, 0));
}



