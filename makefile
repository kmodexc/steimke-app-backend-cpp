#makefile

vpath %.cpp src
vpath %.h include
vpath %.o obj

PROG = rlserv
CC = g++
LDFLAGS = 
OBJPATH = obj
_OBJS = main.o
OBJS = $(patsubst %,$(OBJPATH)/%,$(_OBJS))
INCPATH = include
SRCPATH = src
BINPATH = bin
CPPFLAGS = -c -Wall -g -I$(INCPATH)

all: clean $(PROG)

main.o:
	mkdir -p $(OBJPATH)
	$(CC) $(CPPFLAGS) -o $(OBJPATH)/main.o $(SRCPATH)/main.cpp

$(PROG): $(_OBJS)
	mkdir -p $(BINPATH)
	$(CC) -o $(BINPATH)/$(PROG) $(OBJS)

clean:
	rm -f -r $(OBJPATH) $(BINPATH)


	