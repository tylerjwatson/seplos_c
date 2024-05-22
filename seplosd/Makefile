CC=gcc
CFLAGS= -g -I../library -DLOG_USE_COLOR
OBJS= main.o log.o json.o config.o

LIBS=../library/libseplos.a -lpaho-mqtt3c -luv_a -lpthread -ldl -lrt -ljson-c -lm -lconfig

# PREFIX is environment variable, but if it is not set, then set default value
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

seplosd:	$(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

install: seplosd
	install -m 744 seplosd $(PREFIX)/bin/
	install -m 644 seplosd.conf /etc/
	install -m 644 seplosd.service /usr/lib/systemd/system/
	systemctl daemon-reload