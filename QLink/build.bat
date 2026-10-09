@echo off
set PATH=D:\Programs\Development\msys2\ucrt64\bin;%PATH%
qmake QLink.pro
mingw32-make
