#makefile

vpath %.cpp src
vpath %.h include
vpath %.o obj

PROG = rlserv
CC = g++
LD = g++
LDFLAGS = -Lmodules/libhttp -l/libhttp -lpthread -ldl -lsqlite3
OBJPATH = obj
OBJS = 					\
	App.o 				\
	ConHandle.o			\
	DBItem.o			\
	DependencyService.o	\
	HTTPSocket.o		\
	Item.o				\
	JSONSerializer.o	\
	main.o				\
	MockDataStore.o		\
	SimpleConHandle.o	\
	SimpleSocket.o		\
	SQLDataBaseItem.o	\
	SQLDataBaseUser.o	\
	User.o
OBJS_w_PATH = $(patsubst %,$(OBJPATH)/%,$(OBJS))
INCPATH = include
SRCPATH = src
BINPATH = bin
CPPFLAGS = -std=c++17 -c -Wall -g -I$(INCPATH) -Imodules/libhttp/include -Imodules/json/single_include

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


	
