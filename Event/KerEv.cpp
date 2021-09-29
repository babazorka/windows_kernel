#include "KerEv.h"
#include "pcb.h"
#include "Thread.h"
#include "SCHEDULE.h"
#include "Sysinfo.h"

List<KernelEv*> KernelEv::ListKernelEvs;

KernelEv::KernelEv(IVTNo ivtNo) { // inicijalna vrednost za val
    this->ivtNo = ivtNo;
    this->val = 1;
    this->maker = PCB::running;
    KernelEv::ListKernelEvs.insertEnd(this);
};
KernelEv::~KernelEv(){
};
PCB* KernelEv::getMaker(){
    return this->maker;
};
IVTNo KernelEv::getIvtNo(){
    return this->ivtNo;
};
void KernelEv::wait(){
	// cout<<"w- ";
    if(this->maker == PCB::running){
        if(this->val > 0)
            this->val--;
        else{
            PCB::running->setState(BLOCKED);
            dispatch();
        }
    }
	// cout<<"-w ";

};
void KernelEv::signal(){
	// cout<<"s- ";
    if(this->maker->getState() == BLOCKED)
        Scheduler::put(this->maker->setState(READY));
    else
        this->val++;
	// cout<<"-s ";
};

/*
C:\bc31\BIN\vscode\output\VS_TEST.EXE 5 2 5
*/