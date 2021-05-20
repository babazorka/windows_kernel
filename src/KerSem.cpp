#include "KerSem.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "Thread.h"
#include "List.h"
#include "Sleep.h"
#include "Sysinfo.h"

List<KernelSem*> KernelSem::ListSemaphore;

KernelSem::KernelSem(int init){
    this->val = init;
    this->sleepingPCBs = new Sleep();
    KernelSem::ListSemaphore.insertEnd(this);
};
KernelSem::~KernelSem(){
    delete this->sleepingPCBs;
};
Sleep* KernelSem::getLimitSleeping(){
    return this->sleepingPCBs;
};
int KernelSem::wait(Time maxTimeToWait){
    if(!maxTimeToWait){
        this->val--;
        if(this->val < 0){
            this->blockedPCBs.insertEnd(PCB::running->setState(BLOCKED));
            // cout<<"$ ";
            dispatch();
        }
    }
    else{
        this->sleepingPCBs->sleep(PCB::running->setState(SLEEPING),maxTimeToWait);
        dispatch();
        return 0;
    }
    return 1;
};
int KernelSem::signal(int n){
    PCB* pcb = NULL;
    if(!n){
        this->val++;
        if(this->val<1){
            pcb = this->blockedPCBs.removeFirst();
            if(pcb != NULL){
                if(pcb->getState() == BLOCKED){
                    Scheduler::put(pcb->setState(READY));
                    return 1;
                }
            }
            return 0;
        }
    }
    else if(n > 0){
        int copy = n;
        while(this->sleepingPCBs->checkTimeFirst() != -1 && copy-- > 0){
            pcb = sleepingPCBs->removeFirst();
            if(pcb->getState() == SLEEPING)
                Scheduler::put(pcb->setState(READY));
        }
        return n-copy;
    }
    return n;
};

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 3
*/