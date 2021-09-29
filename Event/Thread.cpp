#include "Thread.h"
#include "pcb.h"
#include <dos.h>
#include "SCHEDULE.H"
#include "SysInfo.h"
#include "intLock.h"

Thread::Thread (StackSize stackSize , Time timeSlice){
    this->myPCB = new PCB(stackSize, timeSlice, this);
};
void Thread::waitToComplete(){
    this->getMyPCB()->waitToComplete();
};
void Thread::start(){
    if(this-> getMyPCB()-> getState() != NEW)
        return;
    Scheduler::put(this-> getMyPCB()-> setState(READY));
};
void dispatch(){
    SysInfo::ChangeContext = 1;
    geninterrupt(8); // prenos konteksta
}
Thread::~Thread(){};

ID Thread::getRunningId(){
    return PCB::running->id;
};
Thread* Thread::getThreadById(ID id){ //dovrsiti
    ListNode<PCB*>* firstNode = List<PCB*>::GetFirstNode(PCB::ListPCBs);
    while(firstNode != NULL && firstNode->getData()->id != id)
        firstNode = firstNode->getNext();
    return firstNode->getData()->getMyThread();
};

ID Thread::getId(){
    return this->getMyPCB()->id;
};

Thread* Thread::clone() const{
    return new Thread(0,0);
}


void Thread::exit(){ // proveriti sta radi sa decom ubijene niti
    PCB::running->setState(KILLED);
    if(PCB::running->nitKojaMeCeka != NULL){
        Scheduler::put(PCB::running->nitKojaMeCeka->setState(READY));
        PCB::running->nitKojaMeCeka->nitKojuCekam = NULL; 
    }
    dispatch();
}

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 10 10 10
*/
