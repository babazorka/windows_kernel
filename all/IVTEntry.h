#ifndef _ivtentry_h_
#define _ivtentry_h_

typedef void interrupt (*pInterrupt)(...);
// typedef unsigned char IVTNo;
typedef unsigned IVTNo;

class Event;
class IVTEntry{
public:
    IVTEntry(IVTNo, pInterrupt);
    ~IVTEntry();
    void signal();
    void callOld();
    static void initialiseSystem();
    static void restoreSystem();
    static Event* Events[10]; // ogranicen broj evenata
private:
    pInterrupt oldRout;
    IVTNo ivtNo;
    Event* myEvent;
};

void interrupt timerEntry(...);
// void interrupt processSignals(...);
void interrupt wakeUpThreads(...);
void tick();

#endif

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 2
*/
