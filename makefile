#makefile

.SUFFIXES: 
.SUFFIXES: .o

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
	Place.o				\
	SimpleConHandle.o	\
	SimpleSocket.o		\
	SQLDataBaseItem.o	\
	SQLDataBasePlaces.o	\
	SQLDataBaseUser.o	\
	User.o			
OBJS_w_PATH = $(patsubst %,$(OBJPATH)/%,$(OBJS))
INCPATH = include
SRCPATH = src
BINPATH = bin
CPPFLAGS = -std=c++17 -c -Wall -g -I$(INCPATH) -Imodules/libhttp/include -Imodules/json/single_include

bin/$(PROG): $(OBJS_w_PATH) $(BINPATH)
	$(LD) -o $(BINPATH)/$(PROG) $(OBJS_w_PATH) $(LDFLAGS)

obj/%.o: %.cpp $(OBJPATH)
	$(CC) $(CPPFLAGS) -o $@ $<

$(BINPATH):
	mkdir -p $(BINPATH)

$(OBJPATH):
	mkdir -p $(OBJPATH)

clean:
	rm -f -r $(BINPATH)
cleanall: clean
	rm -f -r $(OBJPATH)

.PHONY: clean init build

	
