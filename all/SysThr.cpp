#include "SysThr.h"
#include "SysInfo.h"
#include "pcb.h"
#include "user.h"

PCB* MainThread::Main = NULL;
PCB* SysThread::System = NULL;
int MainThread::ReturnUserMain = 1;

MainThread::MainThread(){
    this->getMyPCB()->type = MAIN;
    this->getMyPCB()->setState(READY);
};
SysThread::SysThread(){
    this->getMyPCB()->type = SYSTEM;
    PCB::running = this->getMyPCB();
    SysInfo::Counter = PCB::running->timeSlice;
    this->getMyPCB()->setState(READY);
};
void MainThread::run(){
    MainThread::ReturnUserMain = userMain(SysInfo::Argc, SysInfo::Argv);
};

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 2
*/


