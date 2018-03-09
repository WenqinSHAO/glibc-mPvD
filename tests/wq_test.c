/* name resolution under certain pvd */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <libpvd.h>
#include <errno.h>

/* Usage */
static void usage(char *func) {
  fprintf(stderr, "Usage: %s pvdname query\n", func);
}

/* Simple dns query for name query dns */
static int mk_query(char* query_name) {
  
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  // dns parameters  
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(query_name, NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return -1;
  }
  
  printf("IP addresses for %s:\n", query_name);
  for(p = res;p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }
  printf("\n");
  freeaddrinfo(res);
  return 0;
}


/*
  Arguments: usage described in usage() function
  returns : -1 (and stderr) in case of failure,
            0 otherwise
*/
int main(int argc, char *argv[]) {

  if (argc != 3) {
      usage(argv[0]);
      return -1;
  }
  
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
  fprintf(stderr, "Bound to pvd %s\n", strcmp(pvdname, "") ? pvdname:"implicit pvd");
    
  /* dns query with pvd values */
  if (mk_query(argv[2]) == -1) 
      return -1;

  return 0;
}

