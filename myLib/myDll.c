/*
 * Compile:
 * cl /LD mydll.c /link /out:my.dll
 *
 * Test:
 * rundll32 MY.DLL,SayHello
 */
 
#pragma comment(lib, "User32.lib")  /* for MessageBox() */

#include <Windows.h>

  __declspec(dllexport) int SayHello(int p)  {
 		MessageBox(NULL, "Hello world!", "Message from DLL", MB_OK);	
        ExitProcess(0); 
}
