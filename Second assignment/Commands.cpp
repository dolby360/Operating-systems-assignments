#include "Commands.h"


Commands::Commands(){

}
Commands::~Commands(){

}

void Commands::getCurrentDirectory(){
    //char buff[MAX_PATH_LEN];
    //memset(buff,'\0',MAX_PATH_LEN);
    char *fullPath;//,*home;
    fullPath = get_current_dir_name();
    printf("OS Shell %s> ",fullPath);
    //home=getenv("HOME");

    //printf("%s\n",fullPath);
    //printf("buff = %s\n",buff);
    //printf("%s\n",home);
}