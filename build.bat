@echo off

IF NOT EXIST build mkdir build
pushd build
cl /nologo /Zi /O2 /Fewindows_background_manager.exe ..\main.c /link kernel32.lib user32.lib advapi32.lib
popd
