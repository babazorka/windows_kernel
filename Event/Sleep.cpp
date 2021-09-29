#include "Sleep.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "Sysinfo.h"

SleepNode::SleepNode(PCB* pcb, unsigned time, SleepNode* next) {
	this->pcb = pcb;
	this->next = next;
	this->time = time;
}

void SleepNode::set_next(SleepNode* next) {
	this->next = next;
}

SleepNode::~SleepNode() {
	this->time = 0;
	this->pcb = NULL;
	this->next = NULL;
}

Sleep::Sleep() {
	this->first = NULL;
}

Sleep::~Sleep() {
	SleepNode* tmp;
	while (this->first != NULL) {
		tmp = this->first;
		this->first = first->next;
		delete tmp;
	}
}

void Sleep::sleep(PCB *pcb, unsigned time) {
	if (this->first == NULL)
		this->first = new SleepNode(pcb, time, NULL);
	else {
		SleepNode* node = this->first;
		SleepNode* prev;
		while (time >= node->time) {
			time -= node->time;
			prev = node;
			node = node->next;
			if (node == NULL)
				break;
		}
		if (prev == NULL) { // stavlja na pocetak, sledeci mu je bivsi prvi
			this->first->time -= time;
			SleepNode * next = new SleepNode(pcb, time, this->first);
			this->first = next;
		} else if (node == NULL) { // stavlja na kraj
			SleepNode * next = new SleepNode(pcb, time, NULL);
			prev->next = next;
		} else { // stavlja negde u sredinu
			SleepNode * next = new SleepNode(pcb, time, prev->next);
			prev->next = next;
			next->next->time -= next->time;
		}
	}
}

int Sleep::decrementFirst() {
	if (this->first == NULL) //prazna
		return -1;
	this->first->time -= 1;
	return this->first->time; //vece od 0
}

PCB* Sleep::removeFirst() {
	if (this->first == NULL)
		return NULL;
	if(this->first->time > 0)
		return NULL;
	PCB* ret = this->first->pcb;
	this->first = this->first->next;
	return ret;
}

int Sleep::checkTimeFirst() {
	if (this->first == NULL)
		return -1;
	return this->first->time;
}

void Sleep::wakeUp(Sleep* sleep) {
	PCB* pcb = sleep->removeFirst();
	while (pcb) {
		if (pcb->getState() == SLEEPING)
			Scheduler::put(pcb->setState(READY));
		pcb = sleep->removeFirst();
	}
	sleep->decrementFirst();
}

/* 
 C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
 */
