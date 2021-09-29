#include "KerSem.h"
#include "Sysinfo.h"

Semaphore::Semaphore(int init){
    this->myImpl = new KernelSem(init);
};
Semaphore::~Semaphore(){
    delete this->myImpl;
};
int Semaphore::wait(Time maxTimeToWait){
    int ret = this->myImpl->wait(maxTimeToWait);
    return ret;
};

int Semaphore::signal(){    
    int ret = this->myImpl->signal();
    return ret;
};

int Semaphore::val() const{
    int ret = this->myImpl->val;
    return ret;
};
