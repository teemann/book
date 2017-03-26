CC=cl
AR=lib
LFLAGS=Ws2_32.lib
CFLAGS=/MD
OBJDIR=obj
LIBDIR=lib
OBJS=$(OBJDIR)/http.obj $(OBJDIR)/test.obj

all: http test

http:
	$(CC) -c http.c $(CFLAGS)
	$(AR) /OUT:http.lib http.obj

test:
	$(CC) test.c http.lib $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	del test.exe
	del test.obj
	del http.lib
	del http.obj
