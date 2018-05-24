#ifndef DRIVER_H
#define DRIVER_H

#include "defs.h"

using namespace std;

typedef struct inputRedirection{
    bool inputFlag;
    int fileNumber;
    string fileName;
}inputRedirection;

typedef struct FDS_Handl{
    inputRedirection myInputs;

    bool stdin_flag;
    bool stdout_flag;
    bool stderr_flag;
    bool unknownFD_flag;

    string stdin;
    string stdout;
    string stderr;
    string unknownFD;

    std::vector<std::string> vec;
}FDS_Handl;

class Commands{
private:
    char* fullPath;
    std::vector<std::string> tokens; // Create vector to hold our tokens
    int exitCode;

    //pipe handling.
    bool thereIsApipe;

    //fds handling.
    FDS_Handl leftPipe;
    FDS_Handl rightPipe;

public:
    Commands();
    ~Commands();
    void replaceHomePath();
    void getCurrentDirectory();
    std::vector<std::string> pushTokens(std::string line);
    void interpreter(std::vector<std::string> vec, std::istream& stream);
    std::vector<std::string> eval(std::vector<std::string> vec);
    void makeNewProcessWithoutPipe(FDS_Handl FDS_data);
    void checkIfThereIs_a_pipe(std::vector<std::string> vec);
    void setFDS();
    void ParseFDSmeaning(FDS_Handl &l_r_pipe);
    void FDS_changes(FDS_Handl myFDS);
    void clearAllFDS();
    void makeNewProcessesWithA_pipe(FDS_Handl &l_pipe, FDS_Handl &r_pipe,bool im_a_process,int* fdsArry_forParent);
};

#endif