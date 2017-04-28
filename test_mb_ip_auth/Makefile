I=../include
S=../src
T=../test

CC = gcc
CFLAGS = -Wall -g -I$(I) -I$(T)
LD = gcc
LDFLAGS =
INCS = $(I)/mb_ip_auth.h $(T)/mb_test.h
OBJS = test_mb_ip_auth.o mb_ip_auth.o mb_test.o
LIBS =
PROG = test_mb_ip_auth
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

test_mb_ip_auth.o: test_mb_ip_auth.c $(INCS)
	$(CC) $(CFLAGS) -c test_mb_ip_auth.c

mb_ip_auth.o: $(S)/mb_ip_auth.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_ip_auth.c

mb_test.o: $(T)/mb_test.c $(INCS)
	$(CC) $(CFLAGS) -c $(T)/mb_test.c

clean:
	$(RM) $(PROG) $(OBJS)
