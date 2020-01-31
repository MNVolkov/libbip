# libbip - Library for BipOS (Amazfit Bip FW MOD)

This is library for programming under the BipOS (Amazfit Bip MOD).

From version BipOS 0.5 the startup code of the applications is completely
redesigned. Each application now is a separate file stored in resources.
The executable file format is Arm-elf (or simply"elf"). The mod got a new name:
BipOS. When the MOD starts, resources are scanned for applications. Each
firmware has a different amount of resources, and applications are always
placed at the end. Therefore, the loader looks for resources by Elf signature
starting with resource 930. Found applications are placed in the menu
"Applications".

In order to improve overall convenience and reduce the number of
incompatibilities designed firmware independent applications. So one and the
same application will work both on Latin 1.1.5.12 and not latin 1.1.2.05 with
no changes (you only need to place it in the RES file of the corresponding
version). Independence from the firmware managed through using of a universal
library libbip.a which contains the addresses of the firmware functions. After
loading the elf into RAM, the internal addresses of functions and variables are
relocated, and library addresses of functions are changed to real addresses of
functions in firmware.
