#ifndef _sleep_h_
#define _sleep_h_  

class PCB;
class SleepNode{
public:
    unsigned time;
    PCB* pcb;
    SleepNode* next;
    SleepNode(PCB*,unsigned,SleepNode*);
    ~SleepNode();
    void set_next(SleepNode*);
};

class Sleep{
public:
    SleepNode* first;
    Sleep();
    ~Sleep();
    void sleep(PCB *, unsigned);
    PCB* removeFirst();
    static void wakeUp(Sleep *);
    int checkTimeFirst();
    int decrementFirst();
};
#endif
