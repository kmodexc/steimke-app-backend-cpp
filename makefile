#makefile

vpath %.cpp src
vpath %.h include
vpath %.o obj

PROG = rlserv
CC = g++
LDFLAGS = -Lmodules/libhttp -l/libhttp
OBJPATH = obj
OBJS = \
	main.o 				\
	DependencyService.o	\
	App.o				\
	HTTPSocket.o
OBJS_w_PATH = $(patsubst %,$(OBJPATH)/%,$(OBJS))
INCPATH = include
SRCPATH = src
BINPATH = bin
CPPFLAGS = -c -Wall -g -I$(INCPATH) -Imodules/libhttp/include

build: $(PROG)

init:
	mkdir -p $(OBJPATH)
	mkdir -p $(BINPATH)

%.o: %.cpp init
	$(CC) $(CPPFLAGS) -o $(OBJPATH)/$@ $<

$(PROG): $(OBJS) init
	$(CC) -o $(BINPATH)/$(PROG) $(LDFLAGS) $(OBJS_w_PATH)

clean:
	rm -f -r $(OBJPATH) $(BINPATH)


	