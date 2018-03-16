#include "Commands.h"


Commands::Commands(){
    replaceHomePath();
    exitCode = 0;
}

Commands::~Commands(){

}

std::vector<std::string> Commands::pushTokens(std::string line){
    std::string buf; // Have a buffer string
    std::stringstream ss(line); // Insert the string into a stream

    while (ss >> buf)
        tokens.push_back(buf);

    //std::cout << tokens.size() << std::endl;
    return tokens;
}


std::vector<std::string> Commands::eval(std::vector<std::string> vec){
    char *env;
    std::vector<std::string> newVec(vec);

    //For every element in the vector
    for(ui i = 0; i < vec.size();i++){
        //If first charecter equal to ~ then replace it with getenv("HOME");
        if(vec[i].compare(0,1,"~")==0){
            vec[i] = std::string(getenv("HOME")).append(vec[i].erase(0,1));
        }
        //For every letter in the element
        for(ui j = 0; j < vec[i].length()-1 ; j++){ 
            //Replace $? with exit command. 
            if(vec[i].compare(j,1,"$") == 0 && vec[i].compare(j+1,1,"?") == 0){
                vec[i].erase(j+1,1);
                vec[i].replace(j,1,std::to_string(exitCode));
            }
            //If we see enviromment variable
            else if(vec[i].compare(j,1,"$") == 0 && j < vec[i].size() && vec[i].compare(j+1,1,"$") != 0){
                if(
                !(vec[i][j+1] >= 'a' && vec[i][j+1] <= 'z')&&
                !(vec[i][j+1] >= 'A' && vec[i][j+1] <= 'Z')&&
                !(vec[i][j+1] == '_')
                ){continue;}

                std::string tempStr;
                ui t;
                for(t = j+1; t < vec[i].size();t++){
                    tempStr += vec[i][t];
                    //Next line for debuging.
                    //std::cout << " - " << tempStr << " - " << tempStr.back() <<"\n";
                    if(
                        !(tempStr.back() >= 'a' && tempStr.back() <= 'z')&&
                        !(tempStr.back() >= 'A' && tempStr.back() <= 'Z')&&
                        !(tempStr.back() == '_')&&
                        !(t != 1 && tempStr.back() >= '0' && tempStr.back() <= '9')
                        )
                    {
                        tempStr.erase(tempStr.size()-1,1);
                        break;
                    }
                }
                env = getenv(tempStr.c_str());
                tempStr.clear();
                if(env == NULL){
                    vec[i].erase(j,t-j);
                }else{
                    vec[i].erase(j,t-j);
                    vec[i].insert(j,env);
                }
                
            }
        }
        //Use this line for debugging
        //It can show you how it went.
        std::cout << "-- " << vec[i] << std::endl;
    }
    return newVec;
}

void Commands::interpreter(std::vector<std::string> vec, std::istream &stream){
    if(!stream.eof() && vec.size()==0) return; //if user press only enter

	else if(stream.eof() || vec[0]=="exit") // if exit or EOF
	{
		std::cout << "C ya!" << std::endl;
		exit(OK);
	}

    vec = eval(vec);

    //In case that the 
    //users entered too many args after cd
    if(vec[0] == "cd"){ 
        
        if(vec.size() > 2){
            printf("Too many args\n");
            exitCode = BAD_EXIT;
        }else if(vec.size() == 1){
            exitCode = 0;
        }else if(chdir(vec[1].c_str())== -1){
            //perror - Interprets the value of errno as an error message, and prints it to stderr
            std::string msg="cd: "+ vec[1] + " ";
            perror(msg.c_str());
			exitCode = BAD_EXIT;
        }else{
            //To update the path
            replaceHomePath();
            exitCode = 0;
        }

        
    }
    tokens.clear();
}

void Commands::replaceHomePath(){
    //Set corrent path and replace the home part with ~
    char *home,*temp;
    fullPath = get_current_dir_name();
    home=getenv("HOME");

    ui i;    
    for( i = 0 ; i < strlen(home) ; i++ ){
        if(home[i]!=fullPath[i]){
            i--;
            break;
        }
    }
    temp = new char[strlen(fullPath)];
    memset(temp,'\0',strlen(temp));
    strcpy(temp,fullPath);

    memset(this->fullPath,'\0',strlen(this->fullPath));

    ui k = 1;
    for(; i < strlen(temp); i++,k++){
        this->fullPath[k] = temp[i];
    }
    this->fullPath[0] = '~';
}

void Commands::getCurrentDirectory(){
    printf("OS Shell %s> ",fullPath);
}