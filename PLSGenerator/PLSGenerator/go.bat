@echo off
cls
echo.
echo ----- COMPILE -----
g++ -W -Wall -Wextra -pedantic plsgen.cpp -o plsgen
if %ERRORLEVEL% NEQ 0 echo ----- !! Compile Failed !! -----
if %ERRORLEVEL% EQU 0 rename a.exe plsgen.exe
if %ERRORLEVEL% EQU 0 echo ----- RUN PROGRAM -----
if %ERRORLEVEL% EQU 0 plsgen.exe
