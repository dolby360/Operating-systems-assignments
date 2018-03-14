#include <stdio.h>
#include "defs.h"
#include "Commands.h"

int main(){
    //char *buff;
    //buff=NULL;

    Commands com;
    std::string line;

    while(true){
        try{
			com.getCurrentDirectory();
			getline(std::cin,line);
			com.interpreter(com.pushTokens(line),std::cin);
		}	
		
		//Handles the errors that may occure
		catch(const std::invalid_argument &e){ 
			std::cout << e.what() << std::endl;
		}
		catch(const std::runtime_error &e){ 
			std::cout << e.what() << std::endl;
		}
		catch (...){
			std::cout << "Unknown error!" << std::endl;
		}
    }

    return OK;
}