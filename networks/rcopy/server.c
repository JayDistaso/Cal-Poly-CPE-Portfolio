#include "rcopy_server_header.h"

typedef enum State STATE;

enum State {
	START, DONE, FILENAME, SEND_DATA, WAIT_ON_ACK, TIMEOUT_ON_ACK, WAIT_ON_EOF_ACK, TIMEOUT_ON_EOF_ACK
};

void process_server(int server_sk_num, Window *window);
void process_client(int32_t server_sk_num, uint8_t *buf, int32_t recv_len, Connection *client, Window *window);
STATE filename(Connection *client, uint8_t *buf, int32_t recv_len, int32_t *data_file, int32_t *bufsize);
STATE send_data(Connection *client, uint8_t *packet, int32_t *packet_len,  int32_t data_file, int buf_size, int32_t *seq_num, Window *window);
STATE timeout_on_ack(Connection *client, uint8_t *packet, int32_t packet_len);
STATE timeout_on_eof_ack(Connection *client, uint8_t *packet, int32_t packet_len);
STATE wait_on_ack(Connection *client, Window *window);
STATE wait_on_eof_ack(Connection *client, Window *window);
STATE initalConnection(Connection *client, uint8_t *buf, int32_t recv_len, int32_t *data_file, int32_t *buf_size);
int processArgs(int argc, char **argv);

int main(int argc, char *argv[]) {
	int32_t server_sk_num = 0;
	int portNumber = 0;
	
   portNumber = processArgs(argc, argv);
   Window window;
   window.errorRate = strtod(argv[1], NULL);
   window.seq_num = START_SEQ_NUM;
   window.highest_seq_sent = 0;
   window.lowerWindow = 0;
   /*set up main server port*/
   server_sk_num = udp_server(portNumber);
   process_server(server_sk_num, &window);
   return 0;
}

void process_server(int server_sk_num, Window *window) {
	pid_t pid = 0;
	int status = 0;
	uint8_t buf[MAX_LEN];
	Connection client;
	int32_t recv_len = 0;
	int32_t seq_num = 0;
   uint8_t flag = 0;
   uint32_t r_num;
   Header header;
   
   //Get new client connection, fork() child, parent process waits
	while(1) {
		//block waiting for new client
      if (select_call(server_sk_num, LONG_TIME, 0, NOT_NULL) == 1) {
			recv_len = recv_buf(buf, MAX_LEN, server_sk_num, &client, &flag, &seq_num, &r_num, &header);
         if (recv_len != CRC_ERROR && header.flag == SETUP) {
				if ((pid = fork()) < 0) {
					perror("fork");
					exit(-1);
				}
				//child process
				if (pid == 0) {
               sendtoErr_init(window->errorRate, DROP_ON, FLIP_ON, DEBUG_ON, RSEED_ON);
               window->windowSize = header.r_num;
               process_client(server_sk_num, buf, recv_len, &client, window);
					exit(0);
				}
			         //Only non-child gets here
			   // check to see if any children quit
			   while (waitpid(-1, &status, WNOHANG) > 0) {
			   }
         }

		}
	}
}

void process_client(int32_t server_sk_num, uint8_t *buf, int32_t recv_len, Connection *client, Window *window) {
	STATE state = START;
	int32_t data_file = 0;
	int32_t buf_size = 0;
   int32_t packet_len = 0;
   uint8_t packet[MAX_LEN];
	int32_t seq_num = START_SEQ_NUM;

   
	while (state != DONE) {
		
		switch (state) {
			
			case START:
            state = initalConnection(client, buf, recv_len, &data_file, &buf_size);
				break;
			
			case FILENAME:
            state = filename(client, buf, recv_len, &data_file, &buf_size);
				break;
				
			case SEND_DATA:
				state = send_data(client, packet, &packet_len, data_file, buf_size, &seq_num, window);
				break;
			
			case WAIT_ON_ACK:
				state = wait_on_ack(client, window);
				break;
         
         case TIMEOUT_ON_ACK:
				state = timeout_on_ack(client, packet, packet_len);
				break;
            
         case WAIT_ON_EOF_ACK:
				state = wait_on_eof_ack(client, window);
				break;
         
         case TIMEOUT_ON_EOF_ACK:
				state = timeout_on_eof_ack(client, packet, packet_len);
				break;
			
			case DONE:
				break;
			
			default:
				printf("In default and you should not be here!!!!\n");
				state = DONE;
				break;
		}
	}
}

STATE initalConnection(Connection *client, uint8_t *buf, int32_t recv_len, int32_t *data_file, int32_t *buf_size) {
   uint8_t response[1];
   /* Create client socket to allow for processing this particular client */
   if ((client->socket_num = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("filename, open client socket");
		exit(-1);
	}
   send_buf(response, 0, client, SETUP_RESPONSE, 0, 0, buf);
   return FILENAME;
 
}

STATE filename(Connection *client, uint8_t *buf, int32_t recv_len, int32_t *data_file, int32_t *buf_size) {
   uint8_t response[1];
	char fname[MAX_LEN];
	STATE returnValue = DONE;
   static int retryCount = 0;
   Header header;
   
   if ((returnValue = processSelect(client, &retryCount, FILENAME, SEND_DATA, DONE)) == SEND_DATA) {
      retryCount = 0;
      recv_len = recv_buf(buf, MAX_LEN, client->socket_num, client, NULL, NULL, NULL, &header);
      //if crc error ignore packet
      if (recv_len == CRC_ERROR) {
         return FILENAME;
      }
      if (returnValue == DONE) {
         return DONE;
      }
      else if (header.flag == SETUP) {
         //if we send a response and get a seup the response was lost, so resend
         send_buf(response, 0, client, SETUP_RESPONSE, 0, 0, buf);
         return FILENAME; 
      }
      else if (header.flag != FNAME) {
         //Expected a filename packet but got something else
         return FILENAME;
      }
   }
   else if (returnValue == FILENAME) {
         //If we timeout waiting for the filename send the response again
         send_buf(response, 0, client, SETUP_RESPONSE, 0, 0, buf);
         return FILENAME;
   }
 
	memcpy(buf_size, buf, SIZE_OF_BUF_SIZE);
   *buf_size = ntohl(*buf_size);
	memcpy(fname, &buf[sizeof(*buf_size)], recv_len - SIZE_OF_BUF_SIZE);
   //Attempting to open file
	if (((*data_file) = open(fname, O_RDONLY)) < 0) {
		send_buf(response, 0, client, FNAME_BAD, 0, 0, buf);
		//Bad file name
      return DONE;
	}
	else {
      send_buf(response, 0, client, FNAME_OK, 0, 0, buf);
      //Good file name
		return SEND_DATA;
	}
}

STATE send_data(Connection *client, uint8_t *packet, int32_t *packet_len,  int32_t data_file, int buf_size, int32_t *seq_num, Window *window) {
	uint8_t buf[MAX_LEN];
	int32_t len_read = 0;
   STATE returnValue = DONE;
   static int flag = 1;
   //This array will hold all the buffers we send, in case we need to go back
   static PacketHistory *packetArray;
   //Only want to init this once
   if (flag) {  
      flag--;
      packetArray = calloc(window->windowSize * 2, sizeof(PacketHistory));
   }
   //First we will check for any RR's
   if (select_call(client->socket_num, 0, 0, NOT_NULL)) {
      //There is a packet from client so we should to process it
      return WAIT_ON_ACK;
   }
   //Check if window is open
   if (window->seq_num > window->windowSize + window->lowerWindow) {
      //Window is closed so we need to wait on an ACK or resend
      return WAIT_ON_ACK; 
   }
   if (window->highest_seq_sent >= window->seq_num) {
      //We have already sent this packet so we just need to resend it
      (*packet_len) = send_buf(packetArray[window->seq_num % window->windowSize].buf, packetArray[window->seq_num % window->windowSize].len_read, client, DATA, window->seq_num, 0, packet);
      (window->seq_num)++;
      return SEND_DATA;
   }
   //If we are here then window is open so good to send
   
   len_read = read(data_file, buf, buf_size);

	switch (len_read) {
		case -1:
			perror("send_data, read error");
			return DONE;
			break;
		case 0:
         (*packet_len) = send_buf(buf, 1, client, END_OF_FILE, window->seq_num, 0, packet);
         return WAIT_ON_EOF_ACK;
			break;
		
      default:
			memcpy(packetArray[window->seq_num % window->windowSize].buf, buf, MAX_LEN);
			packetArray[window->seq_num % window->windowSize].len_read = len_read;
         
         if (window->highest_seq_sent < window->seq_num) {
            window->highest_seq_sent = window->seq_num;   
         }     
         
         (*packet_len) = send_buf(buf, len_read, client, DATA, window->seq_num, 0, packet);
         (window->seq_num)++;
         returnValue = SEND_DATA;
			break;
	}
	
	return returnValue;
}

STATE wait_on_ack(Connection *client, Window *window) {
	STATE returnValue = DONE;
   uint32_t crc_check = 0;
   uint8_t buf[MAX_LEN];
   int32_t len = MAX_LEN;
   uint8_t flag = 0;
   int32_t seq_num = 0;
   static int retryCount = 0;
   uint32_t r_num;
   Header header;
   uint8_t response[1];
   if ((returnValue = processSelect(client, &retryCount, SEND_DATA, WAIT_ON_ACK, DONE)) == WAIT_ON_ACK) {
      retryCount = 0;
      crc_check = recv_buf(buf, len, client->socket_num, client, &flag, &seq_num, &r_num, &header);
      //if crc error ignore packet
      if (crc_check == CRC_ERROR) {
         return WAIT_ON_ACK;
      }
      else if (header.flag == ACK) {
         window->lowerWindow = header.r_num;
         return SEND_DATA;
      }
      else if (header.flag == SREJ) {
         //SREJ means it got the packet before this one
         window->lowerWindow = header.r_num - 1;
         window->seq_num = header.r_num;
         return SEND_DATA;
      }
      else if (header.flag == EOF_ACK) {
         return DONE;
      }
      else if (header.flag == FNAME) {
         send_buf(response, 0, client, FNAME_OK, 0, 0, buf);
      }
   }
   window->seq_num = window->lowerWindow;
   return returnValue;
}

STATE wait_on_eof_ack(Connection *client, Window *window) {
	STATE returnValue = DONE;
   uint32_t crc_check = 0;
   uint8_t buf[MAX_LEN];
   int32_t len = MAX_LEN;
   uint8_t flag = 0;
   int32_t seq_num = 0;
   static int retryCount = 0;
   uint32_t r_num;
   Header header;
   
   if ((returnValue = processSelect(client, &retryCount, TIMEOUT_ON_ACK, SEND_DATA, DONE)) == SEND_DATA) {
      retryCount = 0;
      crc_check = recv_buf(buf, len, client->socket_num, client, &flag, &seq_num, &r_num, &header);
      //if crc error ignore packet
      if (crc_check == CRC_ERROR) {
         return SEND_DATA;
      }
      else if (header.flag == EOF_ACK) {
         printf("-----FIle transfer completed :)------\n");
         return DONE;
      }
      else if (header.flag == ACK) {
         window->lowerWindow = header.r_num;
         return SEND_DATA;
      }
      else if (header.flag == SREJ) {
         //SREJ means it got the packet before this one
         window->lowerWindow = header.r_num - 1;
         window->seq_num = header.r_num;
         return SEND_DATA;
      }
   }
   return returnValue;
}

STATE timeout_on_ack(Connection *client, uint8_t *packet,int32_t packet_len) {
   if (sendtoErr(client->socket_num, packet, packet_len, 0, (struct sockaddr *) &(client->remote), client->length) < 0) {
      perror("timeout_on_ack sendto");
      exit(-1);
   }
   return WAIT_ON_ACK;
}

STATE timeout_on_eof_ack(Connection *client, uint8_t *packet,int32_t packet_len) {
   if (sendtoErr(client->socket_num, packet, packet_len, 0, (struct sockaddr *) &(client->remote), client->length) < 0) {
      perror("timeout_on_eof_ack sendto");
      exit(-1);
   }
   return SEND_DATA;
}

int processArgs(int argc, char **argv) {
   int portNumber = 0;
   
   if (argc != 2 && argc != 3) {
      printf("Usage %s error_rate [port number]\n", argv[0]);
      exit(-1);
   }
   else if (argc == 3) {
      portNumber = atoi(argv[2]);
   }
   return portNumber;
}