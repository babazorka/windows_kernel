#include "SysInfo.h"
#include "SysThr.h"
#include "IVTEntry.h"
#include "pcb.h"

int main(int argc, char *argv[]){
    SysInfo::Argc = argc;
    SysInfo::Argv = argv;
    IVTEntry::initialiseSystem();
    while(MainThread::Main->getState() != FINISHED){};
    IVTEntry::restoreSystem();
    return MainThread::ReturnUserMain;
}

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 3 3 3
*/