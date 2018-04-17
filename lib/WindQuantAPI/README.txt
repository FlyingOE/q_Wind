@see https://adrianhenke.wordpress.com/2008/12/05/create-lib-file-from-dll/

Make .lib from .dll:
1. vsvars32.bat
2. dumpbin /EXPORTS XXXXXXXX.dll > XXXXXXXX.def
3. Remove unneeded functions from XXXXXXXX.def, add "EXPORTS" on top of the file
4. lib /def:XXXXXXXX.def /machine:X86 /out:XXXXXXXX.lib
   lib /def:XXXXXXXX.def /machine:X64 /out:XXXXXXXX.lib