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
static void usage() {
  fprintf(stderr, "Usage: dns_query queryType name\n");
  fprintf(stderr, "\t- queryType:\t 1 for classical resolution (resolv.conf), 2 for pvd binding, 3 for both\n");
  fprintf(stderr, "\t- name:     \t name that should be resolved\n");
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
      usage();
      return -1;
  }

  int x = atoi(argv[1]);
  
  if (x == 1 || x == 3) {
    printf(" -------------------\n");
    printf("| using resolv.conf |\n");
    printf(" -------------------\n");
    if (mk_query(argv[2]) == -1) 
      return -1;
    
    if (x == 1)
      return 0; 
  }

  if (x == 2 || x == 3) {
    printf(" ------------------\n");
    printf("| using pvd attr 1 |\n");
    printf(" ------------------\n");

    /* creating pvd */
    char pvd_name[PVDNAMSIZ] = "pvd.cisco.com";
    if (kernel_create_pvd(pvd_name) == -1) {
      fprintf(stderr, "Creation of pvd.cisco.com failed!\n");
      return -1;
    }
    
    /* inserting attributes (using dedicated bash script) */
    if (system("../tests/sh-tests/pvdid-setattr.sh") != 0) {
      fprintf(stderr, "Attribute update through bash script failed!\n");
      goto free_pvd;
    }


    /* binding process to pvd */
    if (proc_bind_to_pvd(pvd_name) == -1) {
      fprintf(stderr, "Binding to pvd.cisco.com failed!\n");
      goto free_pvd;
    }
    
    /* dns query with pvd values */
    if (mk_query(argv[2]) == -1) 
      goto free_pvd;
    
    /* updating attributes (using dedicated bash script) */
    if (system("../tests/sh-tests/pvdid-setattr2.sh") != 0) {
      fprintf(stderr, "Attribute update through bash script failed!\n");
      goto free_pvd;
    }

    printf(" ------------------\n");
    printf("| using pvd attr 2 |\n");
    printf(" ------------------\n");

    /* dns query with new pvd values */
    if (mk_query(argv[2]) == -1) 
      goto free_pvd;

free_pvd:  
    /* binding process to pvd */
    if (proc_bind_to_nopvd() == -1) {
      fprintf(stderr, "Unbinding to pvd.cisco.com failed!\n");
      return -1;
    }
    
    /* pvd deletion */
    if (kernel_update_pvd_attr(pvd_name, ".deprecated", "1") == -1) {
      fprintf(stderr, "Deletion pvd.cisco.com failed!\n");
      return -1;
    }
    
    return 0;
  }

  usage();
  return -1;
}
