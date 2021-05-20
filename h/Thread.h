// File: thread.h
#ifndef _thread_h_
#define _thread_h_
#include <iostream.h>
#include "List.h"

typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef void (*SignalHandler)();
typedef int ID;
typedef unsigned SignalId;
const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2; // default = 2*55ms
class PCB; // Kernel's implementation of a user's thread
class Thread {
public:
    void start();
    void waitToComplete();
    virtual ~Thread();
    static Thread * getThreadById(ID id);
    static ID getRunningId();
    ID getId();

   
    // signali
    // void signal(SignalId signal);
    // void registerHandler(SignalId signal, SignalHandler handler);
    // void unregisterAllHandlers(SignalId id);
    // void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
    // void blockSignal(SignalId signal);
    // static void blockSignalGlobally(SignalId signal);
    // void unblockSignal(SignalId signal);
    // static void unblockSignalGlobally(SignalId signal);

    // posledice potrebe 2og dela
    virtual Thread* clone() const;
    static ID fork();
    static void exit();
    static ID waitForForkChildren();

    //moje
    PCB* getMyPCB(){ return myPCB;}
protected:
    friend class PCB;
    friend class MainThread;
    friend class SysThread;
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run(){};
private:
    PCB* myPCB;
};
void dispatch ();
void signal_0();
void signal_1();
void signal_2();
#endif