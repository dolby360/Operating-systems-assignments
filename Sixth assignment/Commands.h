#ifndef DRIVER_H
#define DRIVER_H

#include "defs.h"

using namespace std;

typedef struct FDS_Handl{
    bool stdin_flag;
    bool stdout_flag;
    bool stderr_flag;

    string stdin;
    string stdout;
    string stderr;

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
    void makeNewProcess(std::vector<std::string> vec);
    void checkIfThereIs_a_pipe(std::vector<std::string> vec);
    void setFDS();
    void ParseFDSmeaning(vector<string> vec);
};

#endif