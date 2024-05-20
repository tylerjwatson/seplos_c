CC=gcc
CFLAGS= -g -I../library -DLOG_USE_COLOR
OBJS= main.o log.o json.o

LIBS=../library/libseplos.a -lpaho-mqtt3c -luv_a -lpthread -ldl -lrt -ljson-c -lm

seplosd:	$(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
