#ifndef _kersem_h_
#define _kersem_h_
#include "Semaphor.h"

class PCB;
class Sleep;
class KernelSem{
public:
    KernelSem(int init = 1);
    ~KernelSem();
    int wait(Time maxTimeToWait);
    int signal(int n = 0);
    int val;
    static List<KernelSem*> ListSemaphore;
    Sleep* getLimitSleeping();
private:
    Sleep* sleepingPCBs;
    List<PCB*> blockedPCBs;
};

#endif