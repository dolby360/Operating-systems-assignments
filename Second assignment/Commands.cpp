#include "Commands.h"


Commands::Commands(){
    //Set corrent path and replace the home part with ~
    char *home,*temp;
    fullPath = get_current_dir_name();
    home=getenv("HOME");

    int i;    
    for( i = 0 ; i < (int)strlen(home) ; i++ ){
        if(home[i]!=fullPath[i]){
            i--;
            break;
        }
    }
    temp = new char[strlen(fullPath)];
    memset(temp,'\0',strlen(temp));
    strcpy(temp,fullPath);

    memset(this->fullPath,'\0',strlen(this->fullPath));

    int k = 1;
    for(; i < (int)strlen(temp); i++,k++){
        this->fullPath[k] = temp[i];
    }
    this->fullPath[0] = '~';
}
Commands::~Commands(){

}

void Commands::getCurrentDirectory(){
    //char buff[MAX_PATH_LEN];
    //memset(buff,'\0',MAX_PATH_LEN);
    printf("OS Shell %s> ",fullPath);

    //printf("%s\n",fullPath);
    //printf("buff = %s\n",buff);
    //printf("%s\n",home);
}