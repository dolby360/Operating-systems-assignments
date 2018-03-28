#ifndef COUSTOMER_H
#define COUSTOMER_H
class coustomer{
private:
    int id;
    int item;
    int amount;
    bool done;
public:
coustomer(){
    done  = true;
}
~coustomer(){}
};

#endif