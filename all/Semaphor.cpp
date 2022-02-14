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

// 2020 projekat
// int Semaphore::signal(int n){    
//     int ret = this->myImpl->signal(n);
//     return ret;
// };

// 2021 projekat
void Semaphore::signal(){
    this->myImpl->signal();
};

int Semaphore::val() const{
    int ret = this->myImpl->val;
    return ret;
};
