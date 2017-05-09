I=../include
S=../src

CC = gcc
CFLAGS = -Wall -g -I$(I)
LD = gcc
LDFLAGS =
INCS = $(I)/mb_rtu_master.h $(I)/mb_rtu_con.h $(I)/mb_rtu_adu.h $(I)/mb_pdu.h $(I)/mb_log.h
OBJS = test_mb_rtu_master.o mb_rtu_master.o mb_rtu_con.o mb_rtu_adu.o mb_pdu.o mb_log.o
LIBS =
PROG = test_mb_rtu_master
RM = /bin/rm -f

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

test_mb_rtu_master.o: test_mb_rtu_master.c $(INCS)
	$(CC) $(CFLAGS) -c test_mb_rtu_master.c

mb_rtu_master.o: $(S)/mb_rtu_master.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_rtu_master.c

mb_rtu_con.o: $(S)/mb_rtu_con.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_rtu_con.c

mb_rtu_adu.o: $(S)/mb_rtu_adu.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_rtu_adu.c

mb_pdu.o: $(S)/mb_pdu.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_pdu.c

mb_log.o: $(S)/mb_log.c $(INCS)
	$(CC) $(CFLAGS) -c $(S)/mb_log.c

clean:
	$(RM) $(PROG) $(OBJS)
