#ifndef _sysifno_h_
#define _sysifno_h_

void lockic();
void unlockic();
#define lock lockic();
#define unlock unlockic();
class SysInfo{
public:
    static unsigned Counter;
    static unsigned LockFlag;
    static int Argc;
    static char** Argv;
    // static char SystemPermission;
    static char ChangeContext;
};
#endif

/* 
C:\bc31\BIN\vscode\output\VS_TEST.EXE 1 1 5
*/