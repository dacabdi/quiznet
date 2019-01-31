CC=g++
CFLAGS=
INCLUDE=-Iinc/ -Iinc/client -Iinc/interfaces -Iinc/misc -Iinc/models -Iinc/server

# sources subdirectories
SRCDIR=src
SRCCLIENT=$(SRCDIR)/client
SRCMISC=$(SRCDIR)/misc
SRCMODELS=$(SRCDIR)/models
SRCSERVER=$(SRCDIR)/server
SRCTESTS=$(SRCDIR)/tests


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

CCFULL=$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BINSUBDIR)/$@ -c






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

#choice
test-choice.test: subdirs test-choice.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-choice.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-choice.test

test-choice.o:
	$(CCFULL) $(SRCTESTS)/test-choice.cpp


#taq
test-tag.test: subdirs test-tag.o TagCollection.o Tag.o
	$(CC) \
	$(BINSUBDIR)/test-tag.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-tag.test

test-tag.o:
	$(CCFULL) $(SRCTESTS)/test-tag.cpp


#question
test-question.test: subdirs test-question.o FullQuestion.o Question.o Choice.o utils.o
	$(CC) \
	$(BINSUBDIR)/test-question.o \
	$(BINSUBDIR)/FullQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/Choice.o \
	$(BINSUBDIR)/utils.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-question.test

test-question.o:
	$(CCFULL)/test-question.cpp


#quizbook
test-quizbook.test: subdirs test-quizbook.o QuizBook.o FullQuestion.o Question.o Choice.o utils.o
	$(CC) \
	$(BINSUBDIR)/test-quizbook.o \
	$(BINSUBDIR)/QuizBook.o \
	$(BINSUBDIR)/FullQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/Choice.o \
	$(BINSUBDIR)/utils.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-quizbook.test

test-quizbook.o:
	$(CCFULL)/test-quizbook.cpp


# MODELS

Choice.o:
	$(CCFULL) $(SRCMODELS)/Choice.cpp

ChoiceCollection.o:
	$(CCFULL) $(SRCMODELS)/ChoiceCollection.cpp

Tag.o:
	$(CCFULL) $(SRCMODELS)/Tag.cpp

TagCollection.o:
	$(CCFULL) $(SRCMODELS)/TagCollection.cpp

Question.o:
	$(CCFULL)/Question.cpp

FullQuestion.o:
	$(CCFULL)/FullQuestion.cpp

QuizBook.o:
	$(CCFULL)/QuizBook.cpp




# UTILS

utils.o:
	$(CCFULL)/utils.cpp


# SERVER

# link server
server.app: subdirs server.o Server.o EchoServer.o
	$(CC) \
	$(BINSUBDIR)/server.o \
	$(BINSUBDIR)/Server.o \
	$(BINSUBDIR)/EchoServer.o \
	$(INCLUDE) \
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
	$(INCLUDE) \
	-o $(BINSUBDIR)/client.app

client.o:
	$(CCFULL)/client.cpp

Client.o:
	$(CCFULL)/Client.cpp