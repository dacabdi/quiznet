CC=g++
CFLAGS=
INCLUDE=inc/
CCFULL=

# sources subdirectories
SRCDIR=src

# binary subdirectories
BINSUBDIR=
BINBASEDIR=bin
DEBUGSUBDIR=$(BINBASEDIR)/debug
RELEASESUBDIR=$(BINBASEDIR)/release

# selector for compilation mode
ifeq ($(MODE),debug)
	#Setting environment for debug session
	BINSUBDIR=$(DEBUGSUBDIR)
	CFLAGS=-g -Wall -Wextra -O0 -Wconversion -pedantic
else
	#Setting environment for release
	BINSUBDIR=$(RELEASESUBDIR)
	CFLAGS=-Wall
endif

CCFULL=$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BINSUBDIR)/$@ -c $(SRCDIR)

all: clean server.app client.app

subdirs:
	mkdir -p $(BINSUBDIR)

subdirs-release:
	mkdir -p $(RELEASESUBDIR)

subdirs-debug:
	mkdir -p $(DEBUGSUBDIR)

clean:
	rm -rvf $(BINBASEDIR)/



# TESTS

test-choice.test: subdirs test-choice.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-choice.o \
	$(BINSUBDIR)/Choice.o \
	-I$(INCLUDE) \
	-o $(BINSUBDIR)/test-choice.test

test-choice.o:
	$(CCFULL)/test-choice.cpp



# MODELS

Choice.o:
	$(CCFULL)/Choice.cpp





# SERVER

# link server
server.app: subdirs server.o Server.o EchoServer.o
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

client.app: subdirs client.o Client.o
	$(CC) \
	$(BINSUBDIR)/client.o \
	$(BINSUBDIR)/Client.o \
	-I$(INCLUDE) \
	-o $(BINSUBDIR)/client.app

client.o:
	$(CCFULL)/client.cpp

Client.o:
	$(CCFULL)/Client.cpp