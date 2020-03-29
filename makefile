#makefile

.SUFFIXES: 
.SUFFIXES: .o

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
OBJS_w_PATH = $(patsubst %,$(OBJPATH)/%,$(OBJS)) modules/bld/sqlite3.o
INCPATH = include
SRCPATH = src
BINPATH = bin
CPPFLAGS = -std=c++17 -c -Wall -g -I$(INCPATH) -Imodules/libhttp/include -Imodules/json/single_include

build: $(BINDIR)/$(PROG)

$(OBJDIR) obj/%.o: %.cpp
	$(CC) $(CPPFLAGS) -o $@ $<

$(BINDIR) $(BINDIR)/$(PROG): $(OBJS_w_PATH)
	$(LD) -o $(BINPATH)/$(PROG) $(OBJS_w_PATH) $(LDFLAGS) 

clean:
	rm -f -r $(BINPATH)
cleanall: clean
	rm -f -r $(OBJPATH)

.PHONY: clean init build

	
