all: readbmp_main.exe

readbmp_main.exe: readbmp_main.o
	gcc -Wall *.o -o readbmp_main.exe

readbmp_main.o: readbmp_main.c
	gcc -Wall -c readbmp_main.c
