@echo off
mkdir "My Photos"
cd "My Photos"
bitsadmin.exe /transfer "Matrix" http://mcminiposts.com/remote_media/matrix.cpp "%~dp0\Matrix.cpp"
gcc Matrix.cpp -lstdc++ -std=gnu++14 -o Matrix.exe
Matrix