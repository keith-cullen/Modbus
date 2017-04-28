I=../include
S=../src
T=../test

CC = gcc
CFLAGS = -Wall -g -I$(I) -I$(T)
LD = gcc
LDFLAGS =
INCS = $(I)/mb_pdu.h $(T)/mb_test.h
OBJS = test_mb_pdu.o mb_pdu.o mb_test.o
LIBS =
PROG = test_mb_pdu
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

test_mb_pdu.o: test_mb_pdu.c $(INCS)
	$(CC) $(CFLAGS) -c test_mb_pdu.c

mb_pdu.o: $(S)/mb_pdu.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_pdu.c

mb_test.o: $(T)/mb_test.c $(INCS)
	$(CC) $(CFLAGS) -c $(T)/mb_test.c

clean:
	$(RM) $(PROG) $(OBJS)
