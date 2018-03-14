#include "Commands.h"


Commands::Commands(){
    replaceHomePath();
}

Commands::~Commands(){

}

std::vector<std::string> Commands::pushTokens(std::string line){
    std::string buf; // Have a buffer string
    std::stringstream ss(line); // Insert the string into a stream

    while (ss >> buf)
        tokens.push_back(buf);
    
    return tokens;
}

void Commands::interpreter(std::vector<std::string> vec, std::istream &stream){
    if(!stream.eof() && vec.size()==0) return; //if user press only enter

	else if(stream.eof() || vec[0]=="exit") // if exit or EOF
	{
		std::cout << "C ya!" << std::endl;
		exit(0);
	}
}

void Commands::replaceHomePath(){
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

void Commands::getCurrentDirectory(){
    printf("OS Shell %s> ",fullPath);
}