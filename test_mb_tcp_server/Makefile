I=../include
S=../src

CC = gcc
CFLAGS = -Wall -g -I$(I)
LD = gcc
LDFLAGS =
INCS = $(I)/mb_tcp_server.h $(I)/mb_tcp_con.h $(I)/mb_ip_auth.h $(I)/mb_tcp_adu.h $(I)/mb_pdu.h $(I)/mb_log.h
OBJS = test_mb_tcp_server.o mb_tcp_server.o mb_tcp_con.o mb_ip_auth.o mb_tcp_adu.o mb_pdu.o mb_log.o
LIBS =
PROG = test_mb_tcp_server
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

test_mb_tcp_server.o: test_mb_tcp_server.c $(INCS)
	$(CC) $(CFLAGS) -c test_mb_tcp_server.c

mb_tcp_server.o: $(S)/mb_tcp_server.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_tcp_server.c

mb_tcp_con.o: $(S)/mb_tcp_con.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_tcp_con.c

mb_ip_auth.o: $(S)/mb_ip_auth.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_ip_auth.c

mb_tcp_adu.o: $(S)/mb_tcp_adu.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_tcp_adu.c

mb_pdu.o: $(S)/mb_pdu.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_pdu.c

mb_log.o: $(S)/mb_log.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_log.c

clean:
	$(RM) $(PROG) $(OBJS)
