#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "SysInfo.h"

int syncPrintf(const char *format, ...) {
	int res;
	va_list args;
//	lock();
	lock
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
//	unlock();
	unlock
	return res;
}
