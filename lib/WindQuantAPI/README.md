# Create `WindQuantData.lib` from `WindQuantData.dll`

**NOTE** As of 2019.09.09, Wind is publishing the required LIB files along with WFT upgrades. The steps below are no longer required.

## For 32-bit build

1. In command prompt, run `vsvars32.bat` to initial MSVC environment.

2. Run:

   ```
    dumpbin /exports ...\WindNET\bin\windquantdata.dll > windquantdata.def
   ```

3. Edit [`windquantdata.def`](x86/windquantdata.def), remove unneeded header lines and exports, reformat with proper DEF file format by adding `EXPORTS` header.

   - For each `__stdcall` entry, two lines must be produced in this file. See [`windquantdata.def`](x86/windquantdata.def) for examples.

4. Run:

   ```
    lib /def:windquantdata.def /out:WindQuantData.lib /machine:X86 /LTCG /subsystem:CONSOLE
   ```

## For 64-bit build

1. In command prompt, run `vsvars32.bat` to initial MSVC environment.

2. Run:

   ```
    dumpbin /exports ...\WindNET\x64\windquantdata.dll > windquantdata.def
   ```

3. Edit [`windquantdata.def`](x64/windquantdata.def), remove unneeded header lines and exports, reformat with proper DEF file format by adding `EXPORTS` header.

   - Unlike the 32-bit version, no extra treatment is required for `__stdcall` entries.

4. Run:

   ```
    lib /def:windquantdata.def /out:WindQuantData.lib /machine:X64 /LTCG /subsystem:CONSOLE
   ```

## References

- <https://adrianhenke.wordpress.com/2008/12/05/create-lib-file-from-dll/>
- <https://qualapps.blogspot.com/2007/08/how-to-create-32-bit-import-libraries.html>
- <https://stackoverflow.com/questions/2393178/creating-an-msvc-import-library-from-a-dll-that-uses-stdcall>
- [Calling conventions & name decoration](https://stackoverflow.com/questions/15660722/why-are-cdecl-calls-often-mismatched-in-the-standard-p-invoke-convention/15664100#15664100)
- [32-bit vs 64-bit name mangling](https://stackoverflow.com/questions/32187514/32-bit-vs-64-bit-library-decoration)
