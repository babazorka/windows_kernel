#include "pcb.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "SysInfo.h"
#include "intLock.h"

PCB* PCB::running = NULL;
unsigned PCB::index = 0;
// SignalId PCB::GlobalyblockedSignals[16];
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
    this->stack[stackSize - 2] = FP_SEG(PCB::wrapper);
    this->stack[stackSize - 3] = FP_OFF(PCB::wrapper);
    this->ss = FP_SEG(stack + stackSize - 12);
    this->sp = FP_OFF(stack + stackSize - 12);
    this->bp = this->sp;

    // if(maker != NULL)
    //     for(int i=0;i<16;i++)
    //         this->blockedSignals[i] = maker->blockedSignals[i];
    //     else for(int i=0;i<16;i++)
    //         this->blockedSignals[i] = 0;
        
    // this->registredHandlers[0].insertEnd(signal_0);
    // this->registredHandlers[1].insertEnd(signal_1);
    // this->registredHandlers[2].insertEnd(signal_2);

    
    this->childrens = new List<PCB*>();
    PCB::ListPCBs->insertEnd(this);
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
    // SysInfo::SystemPermission = 1;
    // PCB::running->getMyThread()->signal(2);
    // if(PCB::running->maker != NULL)
    //     PCB::running->maker->getMyThread()->signal(1);
    // SysInfo::SystemPermission = 0;
    dispatch();
}
// unsigned PCB::isSignalAllowed(SignalId id){
//     if(PCB::GlobalyblockedSignals[id] || this->blockedSignals[id])
//         return 0;
//     return 1;
// }

// posledice potrebe 2og dela

PCB::PCB(Thread* myThread){ // poziva ga clone
    this->id = PCB::index++;
    this->maker = PCB::running;
    this->type = PCB::running->type;
    this->state = PCB::running->state;
    
    this->myThread = myThread;
    this->timeSlice = PCB::running->timeSlice;
    this->stackSize = PCB::running->stackSize;
    this->stack = new unsigned[this-> stackSize];

    unsigned long pozicija_ss_sp = PCB::running->ss;
    pozicija_ss_sp = pozicija_ss_sp<<16;
    pozicija_ss_sp += PCB::running->sp; 
    pozicija_ss_sp -= (unsigned)PCB::running->stack;
    memcpy(this->stack,PCB::running->stack,stackSize*sizeof(unsigned));

    this->sp = FP_OFF(this->stack + pozicija_ss_sp);
    this->ss = FP_SEG(this->stack + pozicija_ss_sp);

    if(PCB::running->sp != PCB::running->bp) // ako nisu na istim adresama
        pozicija_ss_sp -= PCB::running->sp;
        pozicija_ss_sp += PCB::running->bp;

    this->bp = (unsigned)this->stack + pozicija_ss_sp;

    this->childrens = PCB::running->childrens; // ne znam da li treba da imaju istu decu
    PCB::ListPCBs->insertEnd(this);
}

/*
C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 3
*/