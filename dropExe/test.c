#include <windows.h>
#include <Wincrypt.h>

#pragma comment (lib, "Crypt32.lib")

int main() {	
    int h;
    
	BYTE *pb = VirtualAlloc(0x0DC20000, 0x20000, 0x3000, 0x40);     // decoded string
	DWORD *db = VirtualAlloc(0x0DD20000, 0x4, 0x3000, 0x40);        // buffer len
    DWORD *bWritten = VirtualAlloc(0x0DE20000, 0x4, 0x3000, 0x40);  // bytes written to file
	DWORD *handleID = VirtualAlloc(0x0DE30000, 0x4, 0x3000, 0x40);	// Opened file handle
	
    strncpy(0x0DD20000, "\x00\x00\x02\x00", 4);
    printf("Size: 0x%X (%d) \n", *(int *)0x0DD20000, *(int *)0x0DD20000);
    
	CryptStringToBinary("<base64 encoded exe here>", 0, CRYPT_STRING_BASE64, 0x0DC20000, 0x0DD20000, NULL, NULL);
	printf("Decoded: %s\n", (char*)pb);
    
    h = CreateFileA("my2.exe", 0x40000000, 0x00000007, 0, 2, 0x80, 0);
	printf("Handle: %d\n", h);
	_itoa(h, handleID, 10);
	printf("Handle check: %s\n", (char*)handleID);
	
	WriteFile(atoi(handleID), 0x0DC20000, 20, 0x0DE20000, 0);
	
	return 0;
}	
	
	