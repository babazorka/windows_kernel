#ifndef _pcb_h_
#define _pcb_H_

#include "Thread.h"
#include "List.h"

enum State {NEW, READY, BLOCKED, WAITTO, SLEEPING, FINISHED, KILLED};
enum Type {USER, MAIN, SYSTEM};
class PCB{
public:
    PCB(StackSize, Time, Thread*);
    PCB(Thread* ); // posledice potrebe 2og dela
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
    
    // signali
    // SignalId blockedSignals[16];
    // static SignalId GlobalyblockedSignals[16]; // inicjalizovan je nulama po standardu
    // List<SignalId*> registredSignals; //niz listi signala
    // List<SignalHandler> registredHandlers[16]; //niz listi hendlera
    // unsigned isSignalAllowed(SignalId);

    // posledice potrebe 2og dela
    List<PCB*>* childrens; // lista dece forka()
private:
    unsigned *stack;
    Thread* myThread;
    State state;
    static unsigned index;
};
#endif