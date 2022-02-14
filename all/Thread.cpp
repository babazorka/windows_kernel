#include "Thread.h"
#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "SCHEDULE.H"
#include "SysInfo.h"
#include "intLock.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	if (stackSize)
		this->myPCB = new PCB(stackSize, timeSlice, this);
	else{
		lock
		printf("poziva se drigi ctor");
		unlock
		this->myPCB = new PCB(this);
	}

}
;
void Thread::waitToComplete() {
	this->getMyPCB()->waitToComplete();
}
;
void Thread::start() {
	if (this->getMyPCB()->getState() != NEW)
		return;
	Scheduler::put(this->getMyPCB()->setState(READY));
}
;
void dispatch() {
	SysInfo::ChangeContext = 1;
	geninterrupt(8); // prenos konteksta
}
Thread::~Thread() {
}
;

ID Thread::getRunningId() {
	return PCB::running->id;
}
;
Thread* Thread::getThreadById(ID id) { //dovrsiti
	ListNode<PCB*>* firstNode = List<PCB*>::GetFirstNode(PCB::ListPCBs);
	if (firstNode == NULL)
		return NULL;
	while (firstNode->getData()->id != id) {
		firstNode = firstNode->getNext();
		if (firstNode == NULL)
			return NULL;
	}
	return firstNode->getData()->getMyThread();
}
;

ID Thread::getId() {
	return this->getMyPCB()->id;
}
;

// posledice potrebe 2og dela

Thread* Thread::clone() const {
	Thread* newThread = new Thread(0, 0);
	return newThread;
}

ID Thread::fork() {
	Thread *newThread = PCB::running->getMyThread()->clone();
	return newThread->getId();
}

void Thread::exit() { // proveriti sta radi sa decom ubijene niti
	PCB::running->setState(KILLED);
	if (PCB::running->nitKojaMeCeka != NULL) {
		Scheduler::put(PCB::running->nitKojaMeCeka->setState(READY));
		PCB::running->nitKojaMeCeka->nitKojuCekam = NULL;
	}
	dispatch();
}

ID Thread::waitForForkChildren() { // ne pise sta bi therad trebalo vrati ali traticu broj niti koje su gotove
	PCB* pcb = PCB::running->childrens->removeFirst();
	ID count = 0;
	while (pcb != NULL) {
		pcb->waitToComplete();
		count++;
		pcb = PCB::running->childrens->removeFirst();
	}
	return count;
	// return PCB::running->id; // ili ako bi trebalo da se vrati ID pozivajuce niti
}

//signali
// void Thread::signal(SignalId signal){
//     if(signal == 2 || signal == 3){
//         if(SysInfo::SystemPermission)
//             this->getMyPCB()->registredSignals.insertEnd(new SignalId(signal));
//     }
//     else this->getMyPCB()->registredSignals.insertEnd(new SignalId(signal));
// };
// void Thread::registerHandler(SignalId signal, SignalHandler handler){
//     this->getMyPCB()->registredHandlers[signal].insertEnd(handler);
// };
// void Thread::unregisterAllHandlers(SignalId id){
//     this->getMyPCB()->registredHandlers[id].freeList();
// };
// void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
//     this->getMyPCB()->registredHandlers[id].swap(hand1, hand2);

// };
// void Thread::blockSignal(SignalId signal){
//     this->getMyPCB()->blockedSignals[signal] = 1;
// };
// void Thread::blockSignalGlobally(SignalId signal){
//     PCB::GlobalyblockedSignals[signal] = 1;
// };
// void Thread::unblockSignal(SignalId signal){
//     this->getMyPCB()->blockedSignals[signal] = 0;
// };
// void Thread::unblockSignalGlobally(SignalId signal){
//     PCB::GlobalyblockedSignals[signal] = 0;
// };
// void signal_0(){
//     // cout<<"KILLING ";
//     PCB::running->setState(KILLED);
//     if(PCB::running->nitKojaMeCeka != NULL){
//         Scheduler::put(PCB::running->nitKojaMeCeka->setState(READY));
//         PCB::running->nitKojaMeCeka->nitKojuCekam = NULL;
//     }
//     dispatch();
// }
// void signal_1(){
//     intLock
//     cout<<"signal_1 "<<endl;
//     intUnlock
// }
// void signal_2(){
//     intLock
//     cout<<"signal_2 "<<endl;
//     intUnlock
// }

/*
 C:\bc31\BIN\vscode\output\VS_TEST.EXE 10 10 10
 */
