#include "pcb.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "SysInfo.h"
#include "intLock.h"

PCB* PCB::running = NULL;
unsigned PCB::index = 0;
List<PCB*>* PCB::ListPCBs = new List<PCB*>();

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread){
    this->id = PCB::index++;
    this->maker = PCB::running;
    this->type = USER;
    this->state = NEW;
    this->myThread = myThread;
    this->timeSlice = timeSlice;
    stackSize /= sizeof(unsigned);
    this->stackSize = stackSize;
    this->stack = new unsigned[this->stackSize];
    this->stack[stackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
    this->stack[stackSize - 2] = FP_SEG(PCB::wrapper);
    this->stack[stackSize - 3] = FP_OFF(PCB::wrapper);
    this->ss = FP_SEG(stack + stackSize - 12);
    this->sp = FP_OFF(stack + stackSize - 12);
#endif
    this->bp = this->sp;
    this->nitKojuCekam = NULL;
    this->nitKojaMeCeka = NULL;

};
PCB* PCB::setState(State state){
    this->state = state;
    return this;
};
PCB::~PCB(){};
void PCB::waitToComplete(){
    if(this->state == NEW){
        this->state = READY;
        Scheduler::put(this);
        PCB::running->nitKojuCekam = this;
        PCB::running->state = WAITTO;
        this->nitKojaMeCeka = PCB::running;
        dispatch();
    }
    if(this->state != FINISHED && this->state != KILLED){
            if(PCB::running != NULL){ 
                PCB::running->nitKojuCekam = this;
                PCB::running->state = WAITTO;
                this->nitKojaMeCeka = PCB::running;
                dispatch();
            }
        }
    return;
}
void PCB::wrapper(){
    PCB::running -> myThread -> run();
    PCB::running -> state = FINISHED;
    if(PCB::running->nitKojaMeCeka != NULL){
        Scheduler::put(PCB::running->nitKojaMeCeka->setState(READY));
        PCB::running->nitKojaMeCeka->nitKojuCekam = NULL; 
    }
    dispatch();
}

/*
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 0 0
*/
