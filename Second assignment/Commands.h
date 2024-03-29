#ifndef DRIVER_H
#define DRIVER_H

#include "defs.h"

class Commands{
private:
    char* fullPath;
    std::vector<std::string> tokens; // Create vector to hold our tokens
    int exitCode;
public:
    Commands();
    ~Commands();
    void replaceHomePath();
    void getCurrentDirectory();
    std::vector<std::string> pushTokens(std::string line);
    void interpreter(std::vector<std::string> vec, std::istream& stream);
    std::vector<std::string> eval(std::vector<std::string> vec);
    void makeNewProcess(std::vector<std::string> vec);
};

#endif