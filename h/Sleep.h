#ifndef _sleep_h_
#define _sleep_h_  

class PCB;
class SleepNode{
public:
    PCB* pcb;
    unsigned time;
    SleepNode* next;
    SleepNode(PCB*, SleepNode*,unsigned);
};

class Sleep{
public:
    Sleep();
    void sleep(PCB *, unsigned);
    PCB* removeFirst();
    static void wakeUp(Sleep *);
    int checkTimeFirst();
    int decrementFirst();
    SleepNode* first;
};
#endif