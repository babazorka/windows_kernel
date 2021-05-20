#include "Sleep.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "Sysinfo.h"

SleepNode::SleepNode(PCB* pcb, SleepNode* next, unsigned time){
    this->pcb = pcb;
    this->next = next;
    this->time = time;
};
Sleep::Sleep(){
    this->first = NULL;
};
void Sleep::sleep(PCB *pcb, unsigned time){
    SleepNode* node;
    SleepNode* next;
    if(this->first == NULL)
        this->first = new SleepNode(pcb, NULL, time);
    else{
        node = this->first;
        while((node->next != NULL) && (time > node->time)){
            time -= node->time;
            node = node->next;
        }
        next = new SleepNode(pcb, node->next, time);
        if(node->next != NULL){
            node->next->time-= time;
            node->next = next;
        }
    }
};
int Sleep::decrementFirst(){
    if(this->first == NULL) //prazna
        return -1;
    if(--this->first->time) //vece od 0
        return this->first->time;
    return 0; //0 je , nit se budi
};
PCB* Sleep::removeFirst(){
    if(first == NULL)
        return NULL;
    PCB* ret = this->first->pcb;
    this->first = this->first->next;
    return ret;
};
int Sleep::checkTimeFirst(){
    if(this->first == NULL)
        return -1;
    return this->first->time;
};
void Sleep::wakeUp(Sleep* sleep){
    PCB* pcb;
    while(sleep->checkTimeFirst() != -1){
        pcb = sleep->removeFirst();
        if(pcb->getState() == SLEEPING)
            Scheduler::put(pcb->setState(READY));
    }
};

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
*/
