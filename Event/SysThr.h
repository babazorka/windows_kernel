#ifndef _systhr_h_
#define _systhr_h_
#include "Thread.h"

int userMain (int , char*[]);
class PCB;
class MainThread: public Thread {
public:
    static PCB* Main;
    static unsigned MainFinished;
    static int ReturnUserMain;
    MainThread();
    ~MainThread(){this->waitToComplete();}
private:
    void run();
};

class SysThread: public Thread{
public:
    static PCB* System;
    SysThread();
    ~SysThread(){this->waitToComplete();}
private:
    void run(){};
};
#endif

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
*/