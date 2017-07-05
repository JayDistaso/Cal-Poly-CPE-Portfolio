#include <netinet/ether.h>
#include <stdio.h>
#include <pcap.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cpe464.h"
#include "networks.h"


#define NUM_RCOPY_ARGUMENTS 8
#define NUM_RETRIES 10
#define MAX_BUFFER_SIZE 1400
#define FNAME_FLAG 7
#define LONG_TIME 10
#define SHORT_TIME 1
#define MAX_LEN 1500
#define START_SEQ_NUM 1
#define MAX_RETRIES 10
#define SIZE_OF_BUF_SIZE sizeof(int32_t)

typedef struct connection Connection;

//Struct that keeps connection details
struct connection {
   int32_t socket_num;
   struct sockaddr_in remote;
   uint32_t length;
};

enum SELECT {
   SET_NULL, NOT_NULL 
};

//Flags documented in README
enum FLAG {
   ZERO, SETUP, SETUP_RESPONSE, DATA, UNUSED, ACK, SREJ, FNAME, FNAME_OK, FNAME_BAD, END_OF_FILE, EOF_ACK, CRC_ERROR = -1 
};

typedef struct header Header;

//
struct header {
   uint32_t seq_num;
   uint16_t checksum;
   uint8_t flag;
   uint32_t r_num;
};

typedef struct window Window;

struct window {
   int windowSize;
   double errorRate;
   uint32_t lowerWindow;
   uint32_t seq_num;
   uint32_t highest_seq_sent;
};

//Everything we need to resend a packet
typedef struct packetHistory PacketHistory;

struct packetHistory {
   uint8_t buf[MAX_LEN];
   int32_t len_read;
};

int32_t send_buf(uint8_t *buf, uint32_t length, Connection *connection, uint8_t flag, uint32_t sequence_number, uint32_t r_num,  uint8_t *packet);
int32_t safeSend(uint8_t *packet, uint32_t len, Connection *connection);
int32_t safeRecv(int recv_sk_num, char *data_buf, int len, Connection *connection);
int processSelect(Connection *client, int *retryCount, int selectTimeoutState, int dataReadyState, int doneState);
int32_t select_call(int32_t socket_num, int32_t seconds, int32_t microseconds, int32_t set_null);
int32_t recv_buf(uint8_t *buf, int32_t len, int32_t recv_sk_num, Connection *connection, uint8_t *flag, int32_t *seq_num, uint32_t *r_num, Header *header);
int createHeader(uint32_t len, uint8_t flag, uint32_t seq_num, uint32_t r_num, uint8_t *packet);
int retrieveHeader(char *data_buf, int recv_len, uint8_t *flag, int32_t *seq_num, uint32_t *r_num, Header *header);
int32_t udp_client_setup(char *hostname, uint16_t port_num, Connection *connection);
int udp_server(int portNumber);

//called send_buf in unnamed handout
int32_t send_buf(uint8_t *buf, uint32_t length, Connection *connection, uint8_t flag, uint32_t sequence_number, uint32_t r_num, uint8_t *packet) {
   int32_t sentLen = 0;
   int32_t sendingLen = 0;
   
   if (length > 0) {
      memcpy(&packet[sizeof(Header)], buf, length);
   }
   sendingLen = createHeader(length, flag, sequence_number, r_num, packet);
   sentLen = safeSend(packet, sendingLen, connection);
   return sentLen; 
}

int32_t safeSend(uint8_t *packet, uint32_t len, Connection *connection) {
   int send_len = 0;
   if ((send_len = sendtoErr(connection->socket_num, packet, len, 0, (struct sockaddr *) &(connection->remote), connection->length)) < 0) {
      perror("in send_buf, sento() call");
      exit(-1);
   }
   return send_len;
}

int32_t safeRecv(int recv_sk_num, char *data_buf, int len, Connection *connection) {
   int recv_len = 0;
   uint32_t remote_len = sizeof(struct sockaddr_in);
   
   if ((recv_len = recvfrom(recv_sk_num, data_buf, len, 0, (struct sockaddr *) &(connection->remote), &remote_len)) < 0) {
      perror("in send_buf, sento() call");
      exit(-1);
   }
   connection->length = remote_len;
   return recv_len;
}

int processSelect(Connection *client, int *retryCount, int selectTimeoutState, int dataReadyState, int doneState) {
   (*retryCount)++;
   if (*retryCount > NUM_RETRIES) {
      printf("Sent data %d times, no ACK, so I am going to terminate\n", NUM_RETRIES);
      return doneState;
   }
   else {
      if(select_call(client->socket_num, SHORT_TIME, 0, NOT_NULL) == 1) {
         *retryCount = 0;
         return dataReadyState;
      }
      else {
         //no data ready
         return selectTimeoutState;
      }
   }
}

//In handout page networks.c
int32_t select_call(int32_t socket_num, int32_t seconds, int32_t microseconds, int32_t set_null) {
   fd_set fdvar;
   struct timeval aTimeout;
   struct timeval *timeout = NULL;
   
   if (set_null == NOT_NULL) {
      aTimeout.tv_sec = seconds;
      aTimeout.tv_usec = microseconds;
      timeout = &aTimeout;
   }
   
   FD_ZERO(&fdvar);
   FD_SET(socket_num, &fdvar);
   if (select(socket_num + 1, (fd_set *) &fdvar, (fd_set *)0, (fd_set *)0, timeout) < 0) {
      perror("select\n");
      exit(-1);
   }
   if (FD_ISSET(socket_num, &fdvar)) {
      return 1;
   }
   else {
      return 0;
   }
}

//In srej.c
int32_t recv_buf(uint8_t *buf, int32_t len, int32_t recv_sk_num, Connection *connection, uint8_t *flag, int32_t *seq_num, uint32_t *r_num, Header *header) {

	char data_buf[MAX_LEN];
	int32_t recv_len = 0;
   int32_t dataLen = 0;
	recv_len = safeRecv(recv_sk_num, data_buf, len, connection);
   dataLen = retrieveHeader(data_buf, recv_len, flag, seq_num, r_num, header);
   if (dataLen > 0) {
      memcpy(buf, &data_buf[sizeof(Header)], dataLen);
   }
   return dataLen;
}

int createHeader(uint32_t len, uint8_t flag, uint32_t seq_num, uint32_t r_num, uint8_t *packet) {
   //creates the regular header (puts in packet) including the seq num, flag, checksum
   Header *aHeader = (Header *)packet;
   uint16_t checksum = 0;
   
   seq_num = htonl(seq_num);
   memcpy(&(aHeader->seq_num), &seq_num, sizeof(seq_num));
   
   aHeader->r_num = r_num;
   
   aHeader->flag = flag;
   memset(&(aHeader->checksum), 0, sizeof(checksum));
   
   checksum = in_cksum((unsigned short *)packet, len + sizeof(Header));
   memcpy(&(aHeader->checksum), &checksum, sizeof(checksum));
   
   return len + sizeof(Header);
}

int retrieveHeader(char *data_buf, int recv_len, uint8_t *flag, int32_t *seq_num, uint32_t *r_num, Header *header) {
   Header *aHeader = (Header *)data_buf;
   int returnValue = 0;
   
   if(in_cksum((unsigned short *)data_buf, recv_len) != 0) {
      returnValue = CRC_ERROR;
   }
   else {
      if (aHeader->flag == 5) {
         //printf("The RR in this packet is %d\n", aHeader->r_num);
      }
      //memcpy(seq_num, &aHeader->seq_num, sizeof(aHeader->seq_num));
      aHeader->seq_num = ntohl(aHeader->seq_num);
      returnValue = recv_len - sizeof(Header);
   }
   
   *header = *aHeader;
   
   return returnValue;
}

//From networks_c.docx printout Hugh Smith
int32_t udp_client_setup(char *hostname, uint16_t port_num, Connection *connection) {
   struct hostent *hp = NULL;
   
   connection->socket_num = 0;
   connection->length = sizeof(struct sockaddr_in);
   
   if ((connection->socket_num = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("udp_client_stup, socket");
      exit(-1);
   }
   connection->remote.sin_family = AF_INET;
   hp = gethostbyname(hostname);
   
   if (hp == NULL) {
      printf("Host not found: %s\n", hostname);
      return -1;
   }
   memcpy(&(connection->remote.sin_addr), hp->h_addr, hp->h_length);
   
   connection->remote.sin_port = htons(port_num);
   return 0;
}

int udp_server(int portNumber)
{
	struct sockaddr_in local;
	int sk = 0;
   uint32_t len = sizeof(local);
	// create the socket
	if ((sk = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket() call error");
		exit(-1);
	}
	
	// set up the socket
	local.sin_family = AF_INET;    		// internet (IPv6 or IPv4) family
	local.sin_addr.s_addr = INADDR_ANY ;  		// use any local IP address
	local.sin_port = htons(portNumber);   // if 0 = os picks 

	// bind the name (address) to a port
	if (bind(sk,(struct sockaddr *) &local, sizeof(local)) < 0)
	{
		perror("bind() call error");
		exit(-1);
	}

	/* Get the port number */
	//serverAddrLen = sizeof(server);
	getsockname(sk,(struct sockaddr *) &local, &len);
	printf("Using Port #: %d\n", ntohs(local.sin_port));

	return sk;	
	
}

