#makefile

vpath %.cpp src
vpath %.h include
vpath %.o obj

PROG = rlserv
CC = g++
LD = g++
LDFLAGS = -Lmodules/libhttp -l/libhttp -lpthread -ldl
OBJPATH = obj
OBJS = 					\
	App.o 				\
	ConHandle.o			\
	DependencyService.o	\
	HTTPSocket.o		\
	main.o
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
	$(LD) -o $(BINPATH)/$(PROG) $(OBJS_w_PATH) $(LDFLAGS) 

clean:
	rm -f -r $(OBJPATH) $(BINPATH)


	