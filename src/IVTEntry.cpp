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
	this->oldRout = getvect(ivtNo);
	setvect(ivtNo, pInter);
};
IVTEntry::~IVTEntry(){
	setvect(ivtNo, this->oldRout);
};
void IVTEntry::signal(){
	if(IVTEntry::Events[this->ivtNo] != NULL)
		IVTEntry::Events[this->ivtNo]->signal();

};
void IVTEntry::callOld(){
	oldRout();
};
void IVTEntry::initialiseSystem(){
	lock
    // setvect(71,processSignals);
    setvect(72,wakeUpThreads);
	SysThread::System = new SysThread()->getMyPCB();
	MainThread::Main = new MainThread()->getMyPCB();
    setvect(70,getvect(8));
    setvect(8,timerEntry);
	unlock
};
void IVTEntry::restoreSystem(){
    setvect(8, getvect(70));
	delete MainThread::Main->getMyThread();
	PCB::running = NULL;
};
volatile unsigned tsp, sp, tss, ss, tbp, bp;

// void interrupt processSignals(...){
// 	if(PCB::running == NULL)
// 		return;
// 	int size = PCB::running->registredSignals.getSize();
// 	ListNode<SignalHandler>* nodeHandler;
// 	SignalId* id;
// 	while(size-- > 0){
// 		id = PCB::running->registredSignals.removeFirst();
// 		if(PCB::running->isSignalAllowed(*id)){
// 			nodeHandler = PCB::running->registredHandlers[*id].getFirst();
// 			while(nodeHandler != NULL){
// 				(*nodeHandler->getData())();
// 				nodeHandler = nodeHandler->getNext();
// 			}
// 		}
// 		else {
// 			PCB::running->getMyThread()->signal(*id);
// 		}
// 	}
// };
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
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
			}

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

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
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
C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 3
*/