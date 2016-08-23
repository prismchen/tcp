#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "data_struct.h"
#include "sender_helper.h"

extern int sockfd;
extern struct addrinfo hints, *servinfo, *p;
extern int rv;
extern FILE *fd;
extern char *filename;

int get_file_size(const char* filename) {
	int size;
	FILE *f;

	f = fopen(filename, "rb");
	if (f == NULL) return -1;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);

    return size;
}

int buf_send(char *buf) {

	int numbytes;
	if ((numbytes = sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("sender: sendto");
		exit(1);
	}
	return numbytes;
} 

void buf_send_packet(packet* pck) {
	if (sendto(sockfd, pck, sizeof *pck, 0, p->ai_addr, p->ai_addrlen) == -1) {
		perror("sender: sendto");
		exit(1);
	}
}

int connect_prepare(char *ip) {

	fd = fopen(filename, "r");

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(ip, SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("sender: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "sender: failed to create socket\n");
		return 2;
	}

	return 0;
}

void clean_up() {
	freeaddrinfo(servinfo);
	close(sockfd);
	fclose(fd);
}