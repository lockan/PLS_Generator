@echo off
cls
echo.
echo ----- COMPILE -----
gcc -ansi -Wall -pedantic test.c -o test
if %ERRORLEVEL% NEQ 0 echo ----- !! Compile Failed !! -----
if %ERRORLEVEL% EQU 0 echo ----- RUN PROGRAM -----
if %ERRORLEVEL% EQU 0 test.exe
