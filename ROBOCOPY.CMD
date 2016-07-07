@ECHO OFF
::------------------------------------------------------------------------------
:: This file controls the error level returned by the tool ROBOCOPY in order to
:: be usable in a custom build step of Visual Studio.
::
::@ref https://gist.github.com/syl20bnr/4269229
::
::@ref http://ss64.com/nt/robocopy.html
::@ref http://ss64.com/nt/robocopy-exit.html
::------------------------------------------------------------------------------
ROBOCOPY.EXE /E /A-:R /unicode /TS /FP /NP /NJH /NJS %*
IF ERRORLEVEL 16 ECHO ROBOCOPY exit status: ***FATAL ERROR***   & EXIT /B 1
IF ERRORLEVEL 15 ECHO ROBOCOPY exit status: FAIL MISM XTRA COPY & EXIT /B 1
IF ERRORLEVEL 14 ECHO ROBOCOPY exit status: FAIL MISM XTRA      & EXIT /B 1
IF ERRORLEVEL 13 ECHO ROBOCOPY exit status: FAIL MISM COPY      & EXIT /B 1
IF ERRORLEVEL 12 ECHO ROBOCOPY exit status: FAIL MISM           & EXIT /B 1
IF ERRORLEVEL 11 ECHO ROBOCOPY exit status: FAIL XTRA COPY      & EXIT /B 1
IF ERRORLEVEL 10 ECHO ROBOCOPY exit status: FAIL XTRA           & EXIT /B 1
IF ERRORLEVEL  9 ECHO ROBOCOPY exit status: FAIL COPY           & EXIT /B 1
IF ERRORLEVEL  8 ECHO ROBOCOPY exit status: FAIL                & EXIT /B 1
IF ERRORLEVEL  7 ECHO ROBOCOPY exit status: MISM XTRA COPY OK   & EXIT /B 0
IF ERRORLEVEL  6 ECHO ROBOCOPY exit status: MISM XTRA OK        & EXIT /B 0
IF ERRORLEVEL  5 ECHO ROBOCOPY exit status: MISM COPY OK        & EXIT /B 0
IF ERRORLEVEL  4 ECHO ROBOCOPY exit status: MISM OK             & EXIT /B 0
IF ERRORLEVEL  3 ECHO ROBOCOPY exit status: XTRA COPY OK        & EXIT /B 0
IF ERRORLEVEL  2 ECHO ROBOCOPY exit status: XTRA OK             & EXIT /B 0
IF ERRORLEVEL  1 ECHO ROBOCOPY exit status: COPY OK             & EXIT /B 0
IF ERRORLEVEL  0 ECHO ROBOCOPY exit status: --no change--       & EXIT /B 0
