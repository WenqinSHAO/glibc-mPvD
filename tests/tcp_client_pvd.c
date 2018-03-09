/* tcp client that can be configured with a proc scope pvd binding */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <libpvd.h>
#include <string.h>
 
#define SERVER_PORT 8888
 
int main(int argc, char *argv[])
{
	int sock_fd = -1;
	struct sockaddr_in6 server_addr;
	int ret;
	char buf[65535];

    if (argc != 3) {
        printf("Usage: %s pvdname server_addr\n", argv[0]);
        return -1;
    }
 
	/* Arguments could be used in getaddrinfo() to get e.g. IP of server */
	//(void)argc;
	//(void)argv;


    /* binding process to pvd */
    if (strcmp(argv[1], "implicit") || strcmp(argv[1], "NUll") || strcmp(argv[1], "null")){
        if (proc_bind_to_pvd(argv[1]) == -1) {
        // TODO: check error code
            fprintf(stderr, "Binding to pvd %s failed!\n", argv[1]);
            return -1;
        }
    }

    char pvdname[256];
    proc_get_bound_pvd(pvdname);
    fprintf(stderr, "Bound to pvd: %s\n", strcmp(pvdname, "") ? pvdname:"no pvd");
 
	/* Create socket for communication with server */
	sock_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sock_fd == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	}
 
	/* Connect to server running on localhost */
	server_addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, argv[2], &server_addr.sin6_addr);
	server_addr.sin6_port = htons(SERVER_PORT);
 
	/* Try to do TCP handshake with server */
	ret = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		perror("connect()");
		close(sock_fd);
		return EXIT_FAILURE;
	}
 
	/* Send data to server */
	ret = write(sock_fd, argv[1], strlen(argv[1]));
	if (ret == -1) {
		perror("write");
		close(sock_fd);
		return EXIT_FAILURE;
	}
 
	/* Wait for data from server */
	ret = read(sock_fd, buf, 65535);
	if (ret == -1) {
		perror("read()");
		close(sock_fd);
		return EXIT_FAILURE;
	}
 
	printf("Received '%s' from server\n", buf);
 
	/* DO TCP teardown */
	ret = close(sock_fd);
	if (ret == -1) {
		perror("close()");
		return EXIT_FAILURE;
	}
 
	return EXIT_SUCCESS;
}