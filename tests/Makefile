CC = gcc
CFLAGS = -Wall -g -O2
LIBS = /usr/local/lib/libpvd.so

.PHONY: all dns_query

all :	dns_query

dns_query :
	$(CC) dns_query.c -o dns_query $(CFLAGS) $(LIBS)

clean :
	/bin/rm -f dns_query
