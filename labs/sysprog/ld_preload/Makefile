SONAME = libhook.so
GCC = gcc

all: hook
    $(GCC) -g -c -fPIC hook.c -o hook.o
    $(GCC) -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) hook.o

hook:
    @echo "TODO : create a shared library with hook.c"
