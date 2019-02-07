#-------------------------- VARS -----------------------------------------

CC=g++
CFLAGS= -D_POSIX_C_SOURCE=200112L
INCLUDE=-Iinc/ -Iinc/interfaces -Iinc/misc -Iinc/models -Iinc/network

# sources subdirectories
SRCDIR=src
SRCMISC=$(SRCDIR)/misc
SRCMODELS=$(SRCDIR)/models
SRCTESTS=$(SRCDIR)/tests
SRCNETWORK=$(SRCDIR)/network

# test data subdir
TESTDATA=$(SRCTESTS)/data

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

# full compiler line
CCFULL=$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BINSUBDIR)/$@ -c

#------------------------------GENERAL------------------------------------

all: clean server.app client.app

subdirs:
	mkdir -p $(BINSUBDIR)

subdirs-release:
	mkdir -p $(RELEASESUBDIR)

subdirs-debug:
	mkdir -p $(DEBUGSUBDIR)

clean:
	rm -rvf $(BINBASEDIR)/

#------------------------------TESTS--------------------------------------

# put test data in binary subdir
copy-test-data:
	cp $(TESTDATA)/* $(BINSUBDIR)

# >> test-uniformrandom <<
test-uniformrandom.test: subdirs test-uniformrandom.o UniformRandom.o
	$(CC) \
	$(BINSUBDIR)/test-uniformrandom.o \
	$(BINSUBDIR)/UniformRandom.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-uniformrandom.test

test-uniformrandom.o:
	$(CCFULL) $(SRCTESTS)/test-uniformrandom.cpp

# >> test-choice <<
test-choice.test: subdirs test-choice.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-choice.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-choice.test

test-choice.o:
	$(CCFULL) $(SRCTESTS)/test-choice.cpp

# >> test-tag <<
test-tag.test: subdirs test-tag.o TagCollection.o Tag.o
	$(CC) \
	$(BINSUBDIR)/test-tag.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-tag.test

test-tag.o:
	$(CCFULL) $(SRCTESTS)/test-tag.cpp

# >> test-questiontitle <<
test-questiontitle.test: subdirs test-questiontitle.o QuestionTitle.o
	$(CC) \
	$(BINSUBDIR)/test-questiontitle.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-questiontitle.test

test-questiontitle.o:
	$(CCFULL) $(SRCTESTS)/test-questiontitle.cpp

# >> test-question <<
test-question.test: subdirs test-question.o Question.o TagCollection.o Tag.o QuestionTitle.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-question.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-question.test

test-question.o:
	$(CCFULL) $(SRCTESTS)/test-question.cpp

# >> test-solvedquestion <<
test-solvedquestion.test: subdirs test-solvedquestion.o utils.o SolvedQuestion.o Question.o TagCollection.o Tag.o QuestionTitle.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-solvedquestion.o \
	$(BINSUBDIR)/utils.o \
	$(BINSUBDIR)/SolvedQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-solvedquestion.test 

test-solvedquestion.o:
	$(CCFULL) $(SRCTESTS)/test-solvedquestion.cpp

# >> test-quizbook <<
test-quizbook.test: subdirs copy-test-data test-quizbook.o QuizBook.o UniformRandom.o SolvedQuestion.o Question.o TagCollection.o Tag.o QuestionTitle.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-quizbook.o \
	$(BINSUBDIR)/QuizBook.o \
	$(BINSUBDIR)/UniformRandom.o \
	$(BINSUBDIR)/SolvedQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-quizbook.test

test-quizbook.o:
	$(CCFULL) $(SRCTESTS)/test-quizbook.cpp

# >> test-socket <<
test-socket.test: subdirs test-socket.o UniformRandom.o Socket.o Host.o
	$(CC) \
	$(BINSUBDIR)/test-socket.o \
	$(BINSUBDIR)/UniformRandom.o \
	$(BINSUBDIR)/Host.o \
	$(BINSUBDIR)/Socket.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-socket.test

test-socket.o:
	$(CCFULL) $(SRCTESTS)/test-socket.cpp

# >> test-quizserver <<
test-quizserver.test: subdirs test-quizserver.o QuizServer.o Socket.o Host.o QuizBook.o UniformRandom.o SolvedQuestion.o Question.o TagCollection.o Tag.o QuestionTitle.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-quizserver.o \
	$(BINSUBDIR)/QuizServer.o \
	$(BINSUBDIR)/Host.o \
	$(BINSUBDIR)/Socket.o \
	$(BINSUBDIR)/QuizBook.o \
	$(BINSUBDIR)/UniformRandom.o \
	$(BINSUBDIR)/SolvedQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-quizserver.test

test-quizserver.o:
	$(CCFULL) $(SRCTESTS)/test-quizserver.cpp

# >> test-quizclient <<
test-quizclient.test: subdirs test-quizclient.o QuizClient.o Socket.o Host.o SolvedQuestion.o Question.o TagCollection.o Tag.o QuestionTitle.o ChoiceCollection.o Choice.o
	$(CC) \
	$(BINSUBDIR)/test-quizclient.o \
	$(BINSUBDIR)/QuizClient.o \
	$(BINSUBDIR)/Host.o \
	$(BINSUBDIR)/Socket.o \
	$(BINSUBDIR)/SolvedQuestion.o \
	$(BINSUBDIR)/Question.o \
	$(BINSUBDIR)/TagCollection.o \
	$(BINSUBDIR)/Tag.o \
	$(BINSUBDIR)/QuestionTitle.o \
	$(BINSUBDIR)/ChoiceCollection.o \
	$(BINSUBDIR)/Choice.o \
	$(INCLUDE) \
	-o $(BINSUBDIR)/test-quizclient.test

test-quizclient.o:
	$(CCFULL) $(SRCTESTS)/test-quizclient.cpp

#------------------------------MODELS-------------------------------------

Choice.o:
	$(CCFULL) $(SRCMODELS)/Choice.cpp

ChoiceCollection.o:
	$(CCFULL) $(SRCMODELS)/ChoiceCollection.cpp

Tag.o:
	$(CCFULL) $(SRCMODELS)/Tag.cpp

TagCollection.o:
	$(CCFULL) $(SRCMODELS)/TagCollection.cpp

QuestionTitle.o:
	$(CCFULL) $(SRCMODELS)/QuestionTitle.cpp

Question.o:
	$(CCFULL) $(SRCMODELS)/Question.cpp

SolvedQuestion.o:
	$(CCFULL) $(SRCMODELS)/SolvedQuestion.cpp

QuizBook.o:
	$(CCFULL) $(SRCMODELS)/QuizBook.cpp

#------------------------------NETWORK------------------------------------
Socket.o:
	$(CCFULL) $(SRCNETWORK)/Socket.cpp

Host.o:
	$(CCFULL) $(SRCNETWORK)/Host.cpp

QuizServer.o:
	$(CCFULL) $(SRCNETWORK)/QuizServer.cpp

QuizClient.o:
	$(CCFULL) $(SRCNETWORK)/QuizClient.cpp


#------------------------------EXTRA--------------------------------------

utils.o:
	$(CCFULL) $(SRCMISC)/utils.cpp

UniformRandom.o:
	$(CCFULL) $(SRCMISC)/UniformRandom.cpp






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