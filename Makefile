CXX := gcc
CFLAGS := 
CFLAGS +=-Wall


all: build
run: all
	./readbmp_main.exe
clear:
	rm -rf *.o
	rm -rf *.exe
	rm -rf outImg/*.bmp

build: readbmp_main.exe
rebuild: clear build




readbmp_main.exe: readbmp_main.o readbmp.o
	$(CXX) $(CFLAGS) *.o -o readbmp_main.exe

readbmp_main.o: readbmp_main.c
	$(CXX) $(CFLAGS) -c readbmp_main.c
readbmp.o: readbmp/readbmp.c readbmp/readbmp.h
	$(CXX) $(CFLAGS) -c readbmp/readbmp.c