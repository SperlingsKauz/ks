@echo off

set COMPILER=CC65\
%COMPILER%ca65 crt0.s || goto fail
%COMPILER%cc65 -Oi ks.c --add-source || goto fail
%COMPILER%ca65 ks.s || goto fail
%COMPILER%ld65 -C nrom_32k_vert.cfg -o ks.nes crt0.o ks.o nes.lib -Ln labels.txt || goto fail
goto exit

:fail

pause

:exit

move labels.txt .\BUILD\ 1>NUL
move ks.o .\BUILD\ 1>NUL
move ks.s .\BUILD\ 1>NUL
move crt0.o .\BUILD\ 1>NUL
move ks.nes .\BUILD\ 1>NUL

