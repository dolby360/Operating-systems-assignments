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
                //Next line for debuging.
                //std::cout << "------" << vec[i] << "\n";
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
        //std::cout << "--" << vec[i] << std::endl;
    }
    return vec;
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
    }else{
            makeNewProcess(vec);
    }



    tokens.clear();
}

void Commands::makeNewProcess(std::vector<std::string> vec){
    char **args;
    bool zombie=false;
    int n;

    int stat = 0;
    int temp = 0;

    if (vec[vec.size()-1]!="&"){
        n=vec.size();
    }else{
        n=vec.size()-1;
        zombie=true;
    }
    
    args=new char*[n+1];
    for(int i=0;i<n;i++)
        args[i]=const_cast<char*>(vec[i].c_str());
    args[n]=NULL;
    
    int pid=fork();
    
    if (pid<0)
        perror("No child was created!");

    //Parent.
    if(pid != 0){
        if(zombie == true){ 
            /*
            waitpid(): on success, returns the process 
            ID of the child whose state
            has changed; if WNOHANG was specified and one or more child(ren)
            specified by pid exist, but have not yet changed state, then 0 is  
            returned. On error, -1 is returned.
            */
            temp = waitpid(-1,&stat,WNOHANG);
            while(temp == 0){
                temp = waitpid(-1,&stat,WNOHANG);
            }
            if(temp != -1){
                WIFSIGNALED(stat);
                exitCode = DEFAULT_CODE + WEXITSTATUS(stat);
            }else{
                exitCode = BAD_EXIT;
            }

        }else{
            /*
            * According to manual page:
            * -1 mean - wait for any child process.
            */
            if(waitpid(-1,&stat,0) < 0){
                exitCode = BAD_EXIT;
            }
            WIFSIGNALED(stat);
            exitCode = DEFAULT_CODE + WEXITSTATUS(stat);
        }



    //In this case this is child process
    }else{
        if(execvp(args[0],args) == -1){
            /*
            Because the error: "command not found", not supported in errno. 
            I wrote it manually.
            See here
            https://linux.die.net/man/3/errno
            Not supported: perror(msg);
            */
            printf("%s: command not found\n",args[0]);
            exit(TERMINATE_PROCESS);
        }
    }
    if(args){
        delete args;
    }

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