#ifndef _kerev_h_
#define _kerev_h_
#include "Event.h"
#include "List.h"

class PCB;
class KernelEv{
public:
    KernelEv(IVTNo ivtNo);
    ~KernelEv ();
    void wait();
    void signal();
    PCB* getMaker();
    IVTNo getIvtNo();
    static List<KernelEv*> ListKernelEvs;
private:
    IVTNo ivtNo;
    unsigned val;
    PCB *maker;
};


#endif