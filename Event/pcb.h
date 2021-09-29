#ifndef _pcb_h_
#define _pcb_H_

#include "Thread.h"
#include "List.h"

enum State {NEW, READY, BLOCKED, WAITTO, SLEEPING, FINISHED, KILLED};
enum Type {USER, MAIN, SYSTEM};
class PCB{
public:
    PCB(StackSize, Time, Thread*);
    ~PCB();
    static void wrapper();
    static PCB* running;
    PCB* setState(State);
    State getState(){ return state;}
    unsigned ss;    
    unsigned sp;    
    unsigned bp;
    unsigned timeSlice;
    unsigned id;
    unsigned stackSize;
    PCB* maker;
    static List<PCB*>* ListPCBs;
    Thread* getMyThread(){return myThread;}
    Type type;
    void waitToComplete();
    PCB* nitKojuCekam;
    PCB* nitKojaMeCeka;
    
private:
    unsigned *stack;
    Thread* myThread;
    State state;
    static unsigned index;
};
#endif
