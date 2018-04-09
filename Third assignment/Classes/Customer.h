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
        done = true;
    }
    ~customer(){
        
    }
    bool getDone(){return done;}
    int getId(){return id;}
    int getItem(){return item;}
    int getAmount(){return amount;}

    void setId(int val){id = val;}
    void setItem(int val){item = val;}
    void setAmount(int val){amount = val;}
    void setDone(bool var){done = var;}
};

#endif  