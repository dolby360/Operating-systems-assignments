#ifndef COUSTOMER_H
#define COUSTOMER_H
class customer{
private:
    int id;
    int item;
    int amount;
    bool done;
public:
    customer(){
        done  = true;
    }
    ~customer(){
        
    }
    bool getDone(){return done;}


};

#endif 