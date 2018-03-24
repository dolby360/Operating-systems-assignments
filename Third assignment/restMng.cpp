#include "restMng.h"

using namespace std;

void restMng::setArg(char **argv){
    this->simuArgs.simuTime = atoi(argv[1]);
    this->simuArgs.manItems = atoi(argv[2]);
    this->simuArgs.cusCount = atoi(argv[3]);
    this->simuArgs.witersCount = atoi(argv[4]);
}
void restMng::printData(){
    cout << "Simulation time: " << simuArgs.simuTime << endl;
    cout << "Menu items count: "<< simuArgs.manItems << endl;
    cout << "Customers count: " << simuArgs.cusCount << endl;
    cout << "Waiters count: "   << simuArgs.witersCount << endl;
}
void restMng::checkArgsAndPrintData(int argc,char **argv){
    if(argc > 5){
        cerr << "Too many arguments\n";
        exit(1);
    }
    for(int i = 1;i<argc;i++){
        for(uint j = 0; j < strlen(argv[i]);j++){
            if(argv[i][j] < '0' || argv[i][j] > '9'){
                cerr << "Input arguments are not valid!" << "\n";
                exit(1);
            }
        }
    }
    setArgs(argv);
    if(!(simuArgs.simuTime < 30 && simuArgs.manItems <=10 && simuArgs.witersCount<=3 && simuArgs.cusCount<=10)){
        cerr << "Input arguments are not valid!\n";
        exit(1);
    }
    printData();
}