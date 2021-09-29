#include "IVTEntry.h"
#include "SysInfo.h"
#include "SysThr.h"
#include "SCHEDULE.h"
#include "Sleep.h"
#include "KerEv.h"
#include "KerSem.h"
#include "pcb.h"
#include <iostream.h>
#include <dos.h>

Event* IVTEntry::Events[10];

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt pInter){
	this->ivtNo = ivtNo;
#ifndef BCC_BLOCK_IGNORE
	this->oldRout = getvect(ivtNo);
	setvect(ivtNo, pInter);
#endif
};
IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, this->oldRout);
#endif
};
void IVTEntry::signal(){
	if(IVTEntry::Events[this->ivtNo] != NULL)
		IVTEntry::Events[this->ivtNo]->signal();

};
void IVTEntry::callOld(){
	oldRout();
};
void IVTEntry::initialiseSystem(){
	lock;
    // setvect(71,processSignals);
#ifndef BCC_BLOCK_IGNORE
    setvect(72,wakeUpThreads);
#endif
	SysThread::System = new SysThread()->getMyPCB();
	MainThread::Main = new MainThread()->getMyPCB();
#ifndef BCC_BLOCK_IGNORE
    setvect(70,getvect(8));
    setvect(8,timerEntry);
#endif

	unlock
};
void IVTEntry::restoreSystem(){
#ifndef BCC_BLOCK_IGNORE
    setvect(8, getvect(70));
#endif
	delete MainThread::Main->getMyThread();
	PCB::running = NULL;
};
volatile unsigned tsp, sp, tss, ss, tbp, bp;

void interrupt timerEntry(...){

	// cout<<"+-";
	if(!SysInfo::ChangeContext){
	// if(SysInfo::Counter > 1 && !SysInfo::ChangeContext){
		geninterrupt(70); //old timer
		tick();
		SysInfo::Counter--; 
		// geninterrupt(71); // singals, premesteni ovde
		geninterrupt(72); // wakeup threads
	}
	if(!SysInfo::Counter || SysInfo::ChangeContext){
	// else{
		// cout<<PCB::running->id<<"- ";
		// geninterrupt(71); // singals, premesteni gore
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
			}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		
		// cout<<"type = "<< PCB::running->type<<" ";
		// cout<<"state = "<< PCB::running->getState()<<" ";
		// if(PCB::running->getState() == KILLED)
			// cout<<"###### ";
		if((PCB::running->type == USER) && (PCB::running->getState() == READY)){
			Scheduler::put(PCB::running);
			// cout<<"put $"<< PCB::running->id<<" ";
		}
		PCB::running = Scheduler::get();
		if(PCB::running != NULL){}
		else if (MainThread::Main->getState() == READY){
				PCB::running = MainThread::Main;
				// cout<<"main ";
		}
		else if (SysThread::System->getState() == READY){
				PCB::running = SysThread::System;
				// cout<<"system ";
		}

		// geninterrupt(71); // singals 
		SysInfo::ChangeContext = 0;
		SysInfo::Counter = PCB::running->timeSlice;
		tsp = PCB::running->sp;
		tss = PCB::running->ss; 
		tbp = PCB::running->bp;  
#ifndef BCC_BLOCK_IGNORE

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif
		// cout<<"#"<<PCB::running->id<<" ";
	}
	// cout<<"-+ ";
};
void interrupt wakeUpThreads(...){
	ListNode<KernelSem*>* node = KernelSem::ListSemaphore.getFirst();
	while(node != NULL){
		Sleep::wakeUp(node->getData()->getLimitSleeping());
		node = node->getNext();
	}
};
/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 0 0
*/
