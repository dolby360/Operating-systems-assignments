#include "Commands.h"
using namespace std;

Commands::Commands(){
    leftPipe.stdin_flag = false;
    leftPipe.stdout_flag = false;
    leftPipe.stderr_flag = false;

    rightPipe.stdin_flag = false;
    rightPipe.stdout_flag = false;
    rightPipe.stderr_flag = false;

    thereIsApipe = false;
    replaceHomePath();
    exitCode = 0;
}

Commands::~Commands(){

}

std::vector<std::string> Commands::pushTokens(std::string line){
    std::string buf; // Have a buffer string
    std::stringstream ss(line); // Insert the string into a stream

    while (ss >> buf){
        //cout << "==" << buf << endl;
        tokens.push_back(buf);
    }
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

void Commands::checkIfThereIs_a_pipe(std::vector<std::string> vec){
    ui i = 0;
    ui j = 0;
    for(; i < vec.size();i++){
        for(j = 0;j < vec[i].length();j++){
            if(vec[i][j] == '|'){
                i = vec.size() + 1; 
                thereIsApipe = true;
                break;
            }
            //cout << vec[i][j];
        }
        leftPipe.vec.push_back(vec[i]);
    }
    for(i = leftPipe.vec.size(); i < vec.size(); i++){
        rightPipe.vec.push_back(vec[i]);
    }
    //FOR DEBUG!!!
    // if(thereIsApipe){
    //     cout << "there is a pipe\n";
    // }
    // for(i = 0; i < leftPipe.vec.size(); i++){
    //     cout << leftPipe.vec[i] << endl;
    // }
    // cout <<  endl;
    // for(i = 0; i < rightPipe.vec.size(); i++){
    //     cout << rightPipe.vec[i] << endl;
    // }
}

void Commands::ParseFDSmeaning(vector<string> vec){
    std::size_t found;
    unsigned int i = 0;
    //int j = 0;
    // for(int i = 0; i < vec.size();i++){
    //     cout << vec[i] << endl;
    // }
    // cout << endl <<endl;

    for(;i < vec.size();i++){
        found = vec[i].find(">");
        if(found != std::string::npos){
            if(found == 1){
                switch(vec[i][0]){
                    case '0':
                        if(vec[i].length() <= 2){
                            //cout << "stdin = " << vec[i+1] << endl;
                        }else{
                            cout << "stdin = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                    case '1':
                        if(vec[i].length() <= 2){
                            //cout << "stdin = " << vec[i+1] << endl;
                        }else{
                            cout << "stdout = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                    case '2':
                        if(vec[i].length() <= 2){
                            //cout << "stdin = " << vec[i+1] << endl;
                        }else{
                            cout << "stderr = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                }
            }else{
                 //cout << "stdin = " << vec[i+1] << endl;
            }
        }
    }
}

void Commands::setFDS(){
    ui i = 0;
    ui j = 0;
    if(thereIsApipe){

    }else{
        for(; i < leftPipe.vec.size(); i++){
            for(j = 0;j < leftPipe.vec[i].length(); j++){
                if(leftPipe.vec[i][j] == '>'){
                    if(j == 0){ //In this case defaults 1 for redirection
                        if( (i + 1) == leftPipe.vec.size() ){
                            std::string msg="No stdout specified";
                            cout << msg << endl;
			                exitCode = BAD_EXIT;
                            return;
                        }
                        leftPipe.stdout_flag = true;
                        leftPipe.stdout = leftPipe.vec[i+1];
                        //cout << leftPipe.stdout << endl;
                    }else{
                        if( (i + 1) == leftPipe.vec.size() ){
                            std::string msg="No stdout specified";
                            cout << msg << endl;
			                exitCode = BAD_EXIT;
                            return;
                        }
                        switch(leftPipe.vec[i][j-1]){
                            case '0':
                                leftPipe.stdin_flag = true;
                                leftPipe.stdin = leftPipe.vec[i+1];
                                break;
                            case '1':
                                leftPipe.stdout_flag = true;
                                leftPipe.stdout = leftPipe.vec[i+1];
                                break;
                            case '2':
                                leftPipe.stderr_flag = true;
                                leftPipe.stderr = leftPipe.vec[i+1];
                                break;
                        }
                        //cout << leftPipe.stdout << endl;
                    }
                }
            }
        }
    }
    exit(0);
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
        checkIfThereIs_a_pipe(vec);
        setFDS();

        cout << "--" << leftPipe.stdin << endl;
        cout << "--" << leftPipe.stdout << endl;
        cout << "--" << leftPipe.stderr << endl;
        exit(0);
        if(exitCode == BAD_EXIT){
            exitCode = 0;
            return;
        }
        makeNewProcess(vec);
    }
    tokens.clear();
}

void Commands::makeNewProcess(std::vector<std::string> vec){
    char **args;
    bool zombie=false;
    int n;
    
    if (vec[vec.size()-1]!="&")
        n=vec.size();
    else{
        n=vec.size()-1;
        zombie=true;
    }
    
    args=new char*[n+1];
    for( int i = 0; i < n; i++){
        args[i] = new char[vec[i].size()+1];
        strcpy(args[i],vec[i].c_str());
    }
    args[n]=NULL;
    
    int pid=fork();
    if (pid<0)
        perror("No child was created!");
    //parent
    else if(pid!=0){
        int status, pidTer,error;
        // if not zombie
        if (!zombie){
            if(waitpid(pid, &status, 0) ==-1){
                exitCode=BAD_EXIT;
                throw runtime_error("Error occured during waitpid");
            }

            if(WIFSIGNALED(status))
                exitCode=DEFAULT_CODE+WTERMSIG(status);
                
            else if(WIFEXITED(status))
                exitCode=WEXITSTATUS(status);
                
            else
                exitCode=SUCCESS;
        }
        // if background zombie
        else{
            cout << '[' << pid << ']' << endl;
            exitCode=SUCCESS;
        }
        /*
        waitpid(): on success, returns the process 
        ID of the child whose state
        has changed; if WNOHANG was specified and one or more child(ren)
        specified by pid exist, but have not yet changed state, then 0 is  
        returned. On error, -1 is returned.
        */
        if((pidTer = waitpid(-1, &status, WNOHANG)) > 0){
            if(WIFSIGNALED(status))
                error=DEFAULT_CODE+WTERMSIG(status);
            
            else if(WIFEXITED(status))
                error=WEXITSTATUS(status);
            
            else
                error=SUCCESS;
                
            cout << '[' << pidTer << "]: exited, status=" << error << endl;
        }		
    }
    else //child process
    {
        /*
        Because the error: "command not found", not supported in errno. 
        I wrote it manually.
        See here
        https://linux.die.net/man/3/errno
        Not supported: perror(msg);
        */
        if(execvp(args[0],args) ==-1)
        {
            cout << vec[0]+": command not found" << endl;
            exit(BAD_EXIT);
        }
    }
    if(args)
        delete [] args;
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