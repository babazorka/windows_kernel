#include "Sleep.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "Sysinfo.h"

SleepNode::SleepNode(PCB* pcb, SleepNode* next, int time) {
	this->pcb = pcb;
	this->next = next;
	this->time = time;
//	syncPrintf("time = %d!\n", time);
}
;
Sleep::Sleep() {
	this->first = NULL;
	this->size = 0;
}
;
void Sleep::sleep(PCB *pcb, unsigned time) {
	static int id = 0;
	if (this->first == NULL) { // lista prazna, stavlja na pocetak
		this->first = new SleepNode(pcb, NULL, time);
//		syncPrintf("!!!!!!!!! Prva, time = %d!\n", time);
	} else {
//		syncPrintf("id = %d, time = time!\n", id++);
		SleepNode* node = first;
		SleepNode* prev = NULL;
		while (time > node->time) {
			time -= node->time;
			if (node->next == NULL) { // stavalja na kraj
//				syncPrintf("!!!!!!!!! Kraj, time = %d!\n", time);
				node->next = new SleepNode(pcb, node->next, time);
				return;
			}
			prev = node;
			node = node->next;
		}
//		syncPrintf("\n Sredina ili pocetak, time = %d, size = %d \n", time,				size);
		if (prev != NULL) { // stavlja u sredinu, jer prvi postoji
			prev->next = new SleepNode(pcb, node, time);
			node->time -= time;
//			syncPrintf("\n Sredina first = %d,  size = %d \n", first->next,	size);
		} else { // stavlja na pocetak, ali ima neko nakon prvog
			this->first = new SleepNode(pcb, this->first, time);
			this->first->next->time -= time;
//			syncPrintf("\n Kraj first = %d, size = %d \n", first, size);
		}
	}
	size++;
//	syncPrintf("!!!!!!!!! first = %d!\n", first->next);
}
;
int Sleep::decrementFirst() {
//	syncPrintf("###### First  = %d.\n", this->first);
	if (this->first == NULL) //prazna
		return -1;
	this->first->time--;
	if (this->first->time > 0) //vece od 0
		return this->first->time;
	if (this->first->time == 0)
		return 0; //0 je , nit se budi
	else
		syncPrintf("Sta ova fja ovde radi decrementFirst, time = %d ?!\n",
				this->first->time);
	return 0; // default greska je jer ne nit vec trabala da bude budna, probudi je naknadno
}
;
PCB* Sleep::removeFirst() {
	if (first == NULL)
		return NULL;
	PCB* ret = this->first->pcb;
	if (first->time > 0) { // ovo mora zbog semafora jer moze ovu metodu semafor da pozove pa da ne zezne
		if (first->next) {
			first->next->time += first->time;
//			syncPrintf("first->time = %d , first->next->time = %d \n",first->time, first->next->time);
		}

	}
//	syncPrintf("###### First  = %d , first->pcb  = %d \n", this->first,this->first->pcb);
	this->first = this->first->next;
	size--;
	return ret;
}
;
int Sleep::checkTimeFirst() {
	if (this->first == NULL)
		return -1;
	return this->first->time;
}
;

/*
 * decrement
 * check je je while gde izbacuje sve sa 0 ili -1 (kako je implementirano)
 * 	ako je nula mrs napolje
 * */
void Sleep::wakeUp(Sleep* sleep) {
	PCB* pcb = NULL;
//	syncPrintf("###### Time first  = %d.\n", sleep->checkTimeFirst());
	while (sleep->decrementFirst() == 0) { // 2 0 3
		while (sleep->checkTimeFirst() == 0) {
			pcb = sleep->removeFirst();
			if (pcb->getState() == SLEEPING)
				Scheduler::put(pcb->setState(READY));
			else
				syncPrintf("Sta ova nit sa ovim flegom radi ovde ?!\n");
		}
	}
}
;

/*
 C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
 */
