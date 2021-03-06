/**
	@author Xiao Chen
*/
#ifndef __SENDER_HELPER__
#define __SENDER_HELPER__

#define SERVERPORT "4950" // the port used by this protocol
#define MAXBUFLEN 256
#define SLOW_START 1
#define CONGESTION_AVOIDANCE 2
#define FAST_RECOVERY 3

unsigned long get_file_size(const char* filename);
int send_buf(char *buf);
int recv_to_buf(char *buf);
void send_packet(packet* pck);
unsigned long recv_ack();
void connect_prepare(char *ip);
void clean_up();
void *send_file_thread(void *param);
#endif