#include "Commands.h"
using namespace std;

Commands::Commands(){
    leftPipe.stdin_flag = false;
    leftPipe.stdout_flag = false;
    leftPipe.stderr_flag = false;

    rightPipe.stdin_flag = false;
    rightPipe.stdout_flag = false;
    rightPipe.stderr_flag = false;


    leftPipe.myInputs.inputFlag = false;
    leftPipe.myInputs.fileNumber = -1;
    leftPipe.myInputs.fileName = "";

    leftPipe.vec.clear();
    rightPipe.vec.clear();

    leftPipe.stdin_flag = false;
    leftPipe.stdout_flag = false;
    leftPipe.stderr_flag = false;
    leftPipe.unknownFD_flag = false;
    
    leftPipe.stdin = "";
    leftPipe.stdout = "";
    leftPipe.stderr = "";
    leftPipe.unknownFD_flag = "";

    rightPipe.myInputs.inputFlag = false;
    rightPipe.myInputs.fileNumber = -1;
    rightPipe.myInputs.fileName = "";

    rightPipe.stdin_flag = false;
    rightPipe.stdout_flag = false;
    rightPipe.stderr_flag = false;
    rightPipe.unknownFD_flag = false;
    
    rightPipe.stdin = "";
    rightPipe.stdout = "";
    rightPipe.stderr = "";
    rightPipe.unknownFD_flag = "";


    thereIsApipe = false;
    replaceHomePath();
    exitCode = 0;
}

Commands::~Commands(){
}

void Commands::clearAllFDS(){

    if(thereIsApipe){
        leftPipe.myInputs.inputFlag = false;
        leftPipe.myInputs.fileNumber = -1;
        leftPipe.myInputs.fileName = "";

        leftPipe.vec.clear();
        rightPipe.vec.clear();

        leftPipe.stdin_flag = false;
        leftPipe.stdout_flag = false;
        leftPipe.stderr_flag = false;
        leftPipe.unknownFD_flag = false;
        
        leftPipe.stdin = "";
        leftPipe.stdout = "";
        leftPipe.stderr = "";
        leftPipe.unknownFD_flag = "";

        rightPipe.myInputs.inputFlag = false;
        rightPipe.myInputs.fileNumber = -1;
        rightPipe.myInputs.fileName = "";

        rightPipe.stdin_flag = false;
        rightPipe.stdout_flag = false;
        rightPipe.stderr_flag = false;
        rightPipe.unknownFD_flag = false;
        
        rightPipe.stdin = "";
        rightPipe.stdout = "";
        rightPipe.stderr = "";
        rightPipe.unknownFD_flag = "";
    }else{
        leftPipe.myInputs.inputFlag = false;
        leftPipe.myInputs.fileNumber = -1;
        leftPipe.myInputs.fileName = "";

        leftPipe.vec.clear();

        leftPipe.stdin_flag = false;
        leftPipe.stdout_flag = false;
        leftPipe.stderr_flag = false;
        leftPipe.unknownFD_flag = false;

        leftPipe.stdin = "";
        leftPipe.stdout = "";
        leftPipe.stderr = "";
        leftPipe.unknownFD_flag = "";
    }
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

void Commands::ParseFDSmeaning(FDS_Handl &l_r_pipe){
    std::size_t found;
    unsigned int i = 0;    
    std::vector<std::string> vec = l_r_pipe.vec;
    std::vector<std::string> tempVector;

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
                            l_r_pipe.stdin_flag = true;
                            l_r_pipe.stdin = vec[i+1];
                        }else{
                            //cout << "stdin = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                    case '1':
                        if(vec[i].length() <= 2){
                            //cout << "stdout = " << vec[i+1] << endl;
                            l_r_pipe.stdout_flag = true;
                            l_r_pipe.stdout = vec[i+1];
                        }else{
                            //cout << "stdout = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                    case '2':
                        if(vec[i].length() <= 2){
                            //cout << "stderr = " << vec[i+1] << endl;
                            l_r_pipe.stderr_flag = true;
                            l_r_pipe.stderr = vec[i+1];
                        }else{
                            //cout << "stderr = " << vec[i].erase(0,2) << endl;
                        }
                        break;
                    default:
                            l_r_pipe.unknownFD_flag = true;
                            l_r_pipe.unknownFD = vec[i+1];
                        break;
                }
                i++;
            }else if(found == 0){
                 //cout << "stdin = " << vec[i+1] << endl;
                l_r_pipe.stdout_flag = true;
                l_r_pipe.stdout = vec[i+1];
                i++;
            }
        }else{
            if(vec[i] != ""){
                tempVector.push_back(vec[i]);
                //cout << vec[i] << "  " << i << endl;
            }

        }
    }

    i = 0;
    for(;i < vec.size();i++){
        found = vec[i].find("<");
        if(found != std::string::npos){
            if(found == 1){
                char charNum[1];
                charNum[0] = vec[i][0];
                int fileDescriptorNumber = atoi(charNum);
                l_r_pipe.myInputs.inputFlag = true;
                l_r_pipe.myInputs.fileNumber = fileDescriptorNumber;
                l_r_pipe.myInputs.fileName = vec[i+1];
            }else if(found == 0){
                 //cout << "stdin = " << vec[i+1] << endl;
                l_r_pipe.myInputs.inputFlag = true;
                l_r_pipe.myInputs.fileNumber = 0;
                l_r_pipe.myInputs.fileName = vec[i+1];
            }
        }
    }

    l_r_pipe.vec.clear();
    vec.clear();
    for(i = 0; i < tempVector.size();i++){
        found = vec[i].find("<");
        if(found == std::string::npos){
            vec.push_back(tempVector[i]);
            l_r_pipe.vec.push_back(tempVector[i]);
        }
    }

    // for(i = 0;i < l_r_pipe.vec.size();i++){
    //     cout << "-------- " << l_r_pipe.vec[i] << endl;
    // }
    
    // for(i = 0;i < vec.size();i++){
    //     cout << "-------- " << vec[i] << endl;
    // }
}

void Commands::setFDS(){
    if(thereIsApipe){
        ParseFDSmeaning(leftPipe);
        ParseFDSmeaning(rightPipe);
    }else{
        ParseFDSmeaning(leftPipe);
    }
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

        if(thereIsApipe){
            makeNewProcessesWithA_pipe(leftPipe,rightPipe,false,NULL);
            thereIsApipe = false;
        }else{
            makeNewProcessWithoutPipe(leftPipe);
        }
    }
    tokens.clear();
    // if(thereIsApipe){
    //     leftPipe.vec.clear();
    //     rightPipe.vec.clear();
    // }else{
    //     leftPipe.vec.clear();
    // }
    // for(int kk = 0; kk < tokens.size();kk++){
    //     cout << "----*----" << tokens[kk] << endl;
    // }
}

void Commands::makeNewProcessesWithA_pipe(FDS_Handl &l_pipe, FDS_Handl &r_pipe,bool im_a_process,int* fdsArry_forParent){
    char **args;
    bool zombie=false;
    int n;
    std::vector<std::string> vec;
    if(im_a_process){
        vec = r_pipe.vec;
    }else{
        vec = l_pipe.vec;
    }

    if (vec[vec.size()-1]!="&")
        n = vec.size();
    else{
        n = vec.size()-1;
        //zombie=true;
    }

    if(thereIsApipe){
        if(r_pipe.vec[r_pipe.vec.size()-1] == "&"){
            zombie = true;
        }
    }
    
    args = new char*[n+1];
    for( int i = 0; i < n; i++){
        args[i] = new char[vec[i].size()+1];
        strcpy(args[i],vec[i].c_str());
    }
    args[n]=NULL;
    
    if(im_a_process){

        close(fdsArry_forParent[1]);
        close(0);
        dup2(fdsArry_forParent[0],0);
        close(fdsArry_forParent[0]);
        FDS_changes(r_pipe);

        if(execvp(args[0],args) ==-1){
            cout << vec[0] + ": command not found" << endl;
            exit(BAD_EXIT);
        }
        //We will probebly not get here
        //Because execvp will swipe the process.
        return;
    }

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
        int fdsArry[2];
        if(pipe(fdsArry)){
            cerr << "Error with pipe" << endl;
            exit(1);
        }
        int pid=fork();

        if(pid){// parent - second process
            int status;
            waitpid(pid,&status,0);
            makeNewProcessesWithA_pipe(l_pipe,r_pipe,true,fdsArry);
        }else{// child - first process
            //cout << "===" << args[0] << " " << args[1] << endl;
            close(fdsArry[0]);
            close(1);
            dup2(fdsArry[1],1);
            close(fdsArry[1]);
            FDS_changes(l_pipe);
            if(execvp(args[0],args) ==-1){
                cout << vec[0]+": command not found" << endl;
                exit(BAD_EXIT);
            }
        }
        clearAllFDS();
    }
    if(args){
        for(ui j = 0; j < vec.size();j++){
            if(args[j]){
                delete args[j];
            }
        }
        delete[] args;
    }
}

void Commands::makeNewProcessWithoutPipe(FDS_Handl FDS_data){
    char **args;
    bool zombie=false;
    int n;
    std::vector<std::string> vec = FDS_data.vec;

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
        //cout << "===" << args[0] << " " << args[1] << endl;
        FDS_changes(FDS_data);
        
        if(execvp(args[0],args) ==-1){
            /*
            Because the error: "command not found", not supported in errno. 
            I wrote it manually.
            See here
            https://linux.die.net/man/3/errno
            Not supported: perror(msg);
            */
            cout << vec[0]+": command not found" << endl;
            exit(BAD_EXIT);
        }
        clearAllFDS();
    }
    if(args){
        for(ui j = 0; j < vec.size();j++){
            if(args[j]){
                delete args[j];
            }
        }
        delete[] args;
    }
}

void Commands::FDS_changes(FDS_Handl myFDS){
    if(myFDS.stdout_flag){
        int temp_fd = open(myFDS.stdout.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);  /* Open for output */
        if(temp_fd != -1){
            dup2(temp_fd, 1); /* Make file descriptor 1 (stdout) point to new file name */
            close(temp_fd);  /* Don't need this other file descriptor */
        }else{
            cerr << myFDS.stdout.c_str() << ": Error with creating a file" << endl;
            exitCode = BAD_EXIT;
            //exit(BAD_EXIT);
        }
    }
    if(myFDS.stderr_flag){
        int temp_fd = open(myFDS.stderr.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU);  /* Open for output */
        if(temp_fd != -1){
            dup2(temp_fd, 2); /* Make file descriptor 2 (stdout) point to new file name */
            close(temp_fd);  /* Don't need this other file descriptor */
        }else{
            cerr << myFDS.stderr.c_str() << ": Error with creating a file" << endl;
            exitCode = BAD_EXIT;
            //exit(BAD_EXIT);
        }
    }
    if(myFDS.stdin_flag){
        int temp_fd = open(myFDS.stdin.c_str(),O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU);
        if(temp_fd != -1){
            dup2(temp_fd, 0); /* Make file descriptor 0 (stdout) point to new file name */
            close(temp_fd);  /* Don't need this other file descriptor */
        }else{
            cerr << myFDS.stdin.c_str() << ": No such file or directory" << endl;
            exitCode = BAD_EXIT;
            //exit(BAD_EXIT);
        }
    }




    if(myFDS.myInputs.inputFlag){
        int temp_fd = open(myFDS.myInputs.fileName.c_str(),O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU);
        if(temp_fd != -1){
            dup2(temp_fd, myFDS.myInputs.fileNumber);
            close(temp_fd);  /* Don't need this other file descriptor */
        }else{
            cerr << myFDS.myInputs.fileName.c_str() << ": No such file or directory" << endl;
            exitCode = BAD_EXIT;
            //exit(BAD_EXIT);
        }
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