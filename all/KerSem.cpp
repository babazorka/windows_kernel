#include "KerSem.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "Thread.h"
#include "List.h"
#include "Sleep.h"
#include "Sysinfo.h"

List<KernelSem*> KernelSem::ListSemaphore;

#define USER

KernelSem::KernelSem(int init) {
	this->val = init;
	this->sleepingPCBs = new Sleep();
	KernelSem::ListSemaphore.insertEnd(this);
//	syncPrintf("Doda koku, %d.\n", KernelSem::ListSemaphore.getFirst());
}
;
KernelSem::~KernelSem() {
	delete this->sleepingPCBs;
}
;
Sleep* KernelSem::getLimitSleeping() {
	return this->sleepingPCBs;
}
;
int KernelSem::wait(Time maxTimeToWait) {
	this->val--;
//	syncPrintf("Wait, val = %d, maxTimeToWait = %d \n", this->val,maxTimeToWait);

	if (val < 0) {
		if (maxTimeToWait > 0) {
			if (maxTimeToWait == 5) {
//				syncPrintf("!!! maxTimeToWait = %d, id = %d\n",maxTimeToWait, PCB::running->id);
//				syncPrintf("!!!! semaphore wait, val = %d \n", val);
			}
			this->sleepingPCBs->sleep(PCB::running->setState(SLEEPING),
					maxTimeToWait);
			dispatch();
		} else if (maxTimeToWait == 0) {
			this->blockedPCBs.insertEnd(PCB::running->setState(BLOCKED));
			// cout<<"$ ";
			dispatch();
		}
#ifdef USER
		return 0;
#endif
		return PCB::running->odblokiranaSingalom;
	}

#ifdef USER
	return 1;
#endif
}
;

//int KernelSem::wait(Time maxTimeToWait){
//    if(!maxTimeToWait){
//        this->val--;
//        if(this->val < 0){
//            this->blockedPCBs.insertEnd(PCB::running->setState(BLOCKED));
//            // cout<<"$ ";
//            dispatch();
//        }
//    }
//    else{
//        this->sleepingPCBs->sleep(PCB::running->setState(SLEEPING),maxTimeToWait);
//        dispatch();
//        return 0;
//    }

// 2021 projekat
void KernelSem::signal() {

	/*
	 * proverim da li ima koja blockedPCBs
	 * ako ima pustim jednu i to je to
	 * ako nema blokiranih pustim jendu sa ogranicenim vremenom
	 * */
	val++;
//	syncPrintf("KernelSem::signal(), val = %d, id = %d \n", val,PCB::running->id);
	if (val <= 0) {
		PCB* pcb = this->blockedPCBs.removeFirst();
		if (pcb != NULL)
			if (pcb->getState() == BLOCKED) {
				pcb->odblokiranaSingalom = 1;
				Scheduler::put(pcb->setState(READY));
				return;
			} else {

				syncPrintf(
						"Error: KernelSem::signal() pokusava da oslobodi nit koja nije BLOCKED,"
								"vec je %d.\n", pcb->getState());
				return;
			}
		pcb = this->sleepingPCBs->removeFirst(); // ovde treba paziti na vreme, jer moze da skine neku koju ne treba
		if (pcb != NULL)
			if (pcb->getState() == SLEEPING) {
				pcb->odblokiranaSingalom = 1;
				Scheduler::put(pcb->setState(READY));
				return;
			} else {

				syncPrintf(
						"Error: KernelSem::signal() pokusava da oslobodi nit koja nije SLEEPING,"
								"vec je %d.\n", pcb->getState());
				return;
			}
	}

}
;

/* 
 C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 3
 */
