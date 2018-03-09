CC = gcc
CFLAGS = -Wall -g -O2
LIBS = /usr/local/lib/libpvd.so

.PHONY: all 

all :
	$(CC) dns_query.c -o dns_query $(CFLAGS) $(LIBS)
	$(CC) wq_test.c -o wq_test $(CFLAGS) $(LIBS)
	$(CC) tcp_client_pvd.c -o tcp_client_pvd $(CFLAGS) $(LIBS)
	$(CC) tcp_client_naive.c -o tcp_client_naive

clean :
	/bin/rm -f dns_query wq_test tcp_client_naive tcp_client_pvd
