CC=g++
CFLAGS=
INCLUDE=inc/
CCFULL=

# sources subdirectories
SRCDIR=src

# binary subdirectories
BINSUBDIR=
BINBASEDIR=bin
DEBUGSUBDIR=debug
RELEASESUBDIR=release

# selector for compilation mode
ifeq ($(MODE),debug)
	#Setting environment for debug session
	BINSUBDIR=$(BINBASEDIR)/$(DEBUGSUBDIR)
	CFLAGS=-g -Wall -Wextra -O0 -Wconversion -pedantic
	CCFULL=$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BINSUBDIR)/$@ -c $(SRCDIR)
else
	#Setting environment for release
	BINSUBDIR=$(BINBASEDIR)/$(RELEASESUBDIR)
	CFLAGS=-Wall
	CCFULL=$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BINSUBDIR)/$@ -c $(SRCDIR)
endif


all: clean server.app client.app


subdirs:
	mkdir -p $(BINSUBDIR)

clean:
	rm -rvf $(BINBASEDIR)/


# SERVER

# link server
server.app : subdirs server.o Server.o EchoServer.o
	$(CC) \
	$(BINSUBDIR)/server.o \
	$(BINSUBDIR)/Server.o \
	$(BINSUBDIR)/EchoServer.o \
	-I$(INCLUDE) \
	-o $(BINSUBDIR)/server.app

# dependencies
server.o:
	$(CCFULL)/server.cpp

Server.o:
	$(CCFULL)/Server.cpp

EchoServer.o:
	$(CCFULL)/EchoServer.cpp


# CLIENT

# link server
client.app : subdirs client.o Client.o
	$(CC) \
	$(BINSUBDIR)/client.o \
	$(BINSUBDIR)/Client.o \
	-I$(INCLUDE) \
	-o $(BINSUBDIR)/client.app

client.o :
	$(CCFULL)/client.cpp

Client.o :
	$(CCFULL)/Client.cpp