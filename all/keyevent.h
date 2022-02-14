//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    keyevent.h
//   Date:    Maj 2021
//===========================================================================//

#ifndef _OS1_KEYBOARD_EVENT_
#define _OS1_KEYBOARD_EVENT_

#include <thread.h>
#include "IVTEntry.h" //dodato 

#define PREPAREENTRY(numEntry,call)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
    newEntry##numEntry.signal();\
    if (call == 1)\
        newEntry##numEntry.callOld();\
};

class BoundedBuffer;
class KeyboardEvent :public Thread{
public:

	KeyboardEvent (BoundedBuffer*);  
	virtual ~KeyboardEvent () {waitToComplete();}
	Thread* clone() const { return new KeyboardEvent(myBuffer); };
protected:

	virtual void run();

	char znak;

private:
  
	BoundedBuffer* myBuffer;
};




#endif  // _OS1_KEYBOARD_EVENT_
