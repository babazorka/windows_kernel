#include "KerEv.h"
#include "SysInfo.h"
#include "IVTEntry.h"
Event::Event(IVTNo ivtNo){
    this->myImpl = new KernelEv(ivtNo);
    IVTEntry::Events[ivtNo] = this;
};
void Event::wait(){
    this->myImpl->wait();
};
void Event::signal(){
    this->myImpl->signal();
};
Event::~Event(){
    delete this->myImpl;
};

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
*/