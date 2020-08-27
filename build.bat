@echo off

IF NOT EXIST build mkdir build
pushd build
cl /nologo /Zi ..\main.c /link user32.lib advapi32.lib
popd
