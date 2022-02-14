#include "SysInfo.h"
#include <iostream.h>

unsigned SysInfo::Counter = 1;
unsigned SysInfo::LockFlag = 0;
int SysInfo::Argc = 0;
char** SysInfo::Argv = new char*;
// char SysInfo::SystemPermission = 0;
char SysInfo::ChangeContext = 0;

void lockic(){
    if(!SysInfo::LockFlag)
        asm cli;
    else cout<<"error lockic ";
    SysInfo::LockFlag = 1;
};

void unlockic(){
    if(SysInfo::LockFlag)
        asm sti;
    else cout<<"error unlockic ";
    SysInfo::LockFlag = 0;
};

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
*/