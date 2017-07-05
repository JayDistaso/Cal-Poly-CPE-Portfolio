/*
   Client for Program 3 (rcopy)
   By Justin Distaso jdistaso@calopoly.edu
   Due June 5th 2017 (6/5/17)
   CPE 464: Networks 
*/

#include "rcopy_server_header.h"
typedef enum State STATE;

enum State {
   DONE, FILENAME, RECV_DATA, FILE_OK, START_STATE, CHECK_INIT
};

STATE filename(char *filename, int32_t buffer_size, Connection *server);
STATE startState(char **argv, Connection *server);
STATE file_ok(int *output_file_fd, char *outputFileName);
STATE recv_data(int32_t output_file, Connection *server);
STATE initOK(Connection *server);
static Header header;

int main(int argc, char *argv[]) {

   Connection server;
   STATE state = START_STATE;
   
   //Basic check of arguments
   if (argc != NUM_RCOPY_ARGUMENTS) {
      perror("Usage: rcopy local-TO-file remote-FROM-file window-size buffer-size error-percent remote-machine remote-port\n");
      exit(EXIT_FAILURE);
   }
   
   //Take in arguments
   char *remote_from_file = argv[2]; //name of the file the server sends to rcopy
   int buffer_size = atoi(argv[4]); // number of data bytes(from the file) trasmitted in a data packet
   double error_percent = atof(argv[5]); // % of packets that are in error (floating point number)
   int32_t output_file_fd = 0;
   sendtoErr_init(error_percent, DROP_ON, FLIP_ON, DEBUG_ON, RSEED_ON);
   
   if (strlen(argv[2]) > 100 || strlen(argv[3]) > 100) {
      printf("Filenames should be under 100 chars\n");
   }  
   while (state != DONE) {
      switch (state) {   
      
      case START_STATE:
         state = startState(argv, &server);
         break;
      case CHECK_INIT:
         state = initOK(&server);
         break;
      case FILENAME:
         state = filename(remote_from_file, buffer_size, &server);
         break;
      case FILE_OK:        
         state = file_ok(&output_file_fd, argv[1]);
         break;
      case RECV_DATA:         
         state = recv_data(output_file_fd, &server);
         break;
      case DONE:
         break;
         
      default:
         printf("ERROR: In Unknown state\n");
         exit(EXIT_FAILURE);
      }
   }
   off_t position = lseek(output_file_fd, -1, SEEK_END);
   //off_t position = ftello(output_file_fd);
   ftruncate(output_file_fd, position);  
   printf("Client reached DONE state, exiting\n");
   return 0;
}
//@@@ STRIP ALL TABS FROM PROGRAM AT THE END
STATE filename(char *filename, int32_t buffer_size, Connection *server) {
   
   int returnValue = START_STATE;
   static int retryCount = 0; //Keep track of how many times we retry   
   uint8_t buffer[MAX_BUFFER_SIZE];
   uint8_t packet[MAX_LEN];
   uint8_t flag = 0;
   int32_t seq_num = 0;
   int32_t recv_check = 0;
   int32_t fname_len = strlen(filename) + 1;
   uint32_t r_num;

   buffer_size = htonl(buffer_size);
   memcpy(buffer, &buffer_size, sizeof(buffer_size));
   memcpy(&buffer[sizeof(buffer_size)], filename, fname_len);
   send_buf(buffer, fname_len + sizeof(buffer_size), server, FNAME, 0, 0, packet);
   
   if ((returnValue = processSelect(server, &retryCount, FILENAME, FILE_OK, DONE)) == FILE_OK) {
      retryCount = 0;
      recv_check = recv_buf(packet, MAX_LEN, server->socket_num, server, &flag, &seq_num, &r_num, &header);
      
      /*Check for bitflip*/
      if (recv_check == CRC_ERROR) {
         return FILENAME;
      }
	   else if (header.flag == FNAME_BAD) {
			printf("File %s not found\n", filename);
			return DONE;
		}
      else if (header.flag == FNAME_OK) {
         return FILE_OK;
      }
      else if (header.flag == SETUP_RESPONSE) {
      //Out fname packet must have gotten lost so lets resend it
         return FILENAME;
      }
      else if (header.flag == DATA) {
         return FILENAME;
      }
	}
   return returnValue;
} 

STATE startState(char **argv, Connection *server) {
   uint8_t packet[MAX_LEN];

   //If we have connected to server before, close it before reconnect
   if (server->socket_num > 0) {
      close(server->socket_num);
   }
   
   //Connection was good
   if (udp_client_setup(argv[6], atoi(argv[7]), server) >= 0) {
      //Send the windowsize
      send_buf(packet, 1, server, SETUP, 0, atoi(argv[3]), packet);  
      return CHECK_INIT;   
   }
   
   return DONE;
}  

STATE file_ok(int *output_file_fd, char *outputFileName) {
   if ((*output_file_fd = open(outputFileName, O_CREAT | O_TRUNC | O_WRONLY, 0600)) < 0) {
      perror("File open error: ");
      return DONE;
   }
   else {
      return RECV_DATA;
   }
}

STATE initOK(Connection *server) {
   
   uint8_t packet[MAX_LEN];
   Header header;
   int32_t recv_check = 0;
      
   if (select_call(server->socket_num, SHORT_TIME, 0, NOT_NULL) == 1) {
      recv_check = recv_buf(packet, MAX_LEN, server->socket_num, server, NULL, NULL, NULL, &header);
      /*Check for bitflip*/
      if (recv_check == CRC_ERROR) {
         return CHECK_INIT;
      }
	   else if (header.flag != SETUP_RESPONSE) {
			return START_STATE;
		}
      else {
         return FILENAME;
      }

   }
   //We timed out on the initial call
   return START_STATE;
}

STATE recv_data(int32_t output_file, Connection *server) {
	int32_t seq_num = 0;
   uint8_t flag = 0;
   uint8_t data_buf[MAX_LEN];
   int32_t data_len = 0;
   uint8_t packet[MAX_LEN];
   static int32_t expected_seq_num = START_SEQ_NUM;
   static int32_t mySeqNum = START_SEQ_NUM;
   uint32_t r_num;
   static int retries = 0;
   //Block until we recieve data
	if (select_call(server->socket_num, 0, 0, SET_NULL) == 0) {
      //These lines should never happen due to the way select is called
      if (retries < 10) {
         //Send ACK 
         send_buf(packet, 1, server, ACK, mySeqNum++, expected_seq_num, packet);  
         retries++;
      }
      else {
         return DONE;
      }
      return RECV_DATA;
	}
	
	data_len = recv_buf(data_buf, MAX_LEN, server->socket_num, server, &flag, &seq_num, &r_num, &header);
   if (data_len == CRC_ERROR) {
      //Pretend we never saw corrupted packet
      return RECV_DATA;
   }
	
		
   if (header.seq_num == expected_seq_num) {
      expected_seq_num++;
      write(output_file, &data_buf, data_len);
   }
   else {
      //We recieved a packet we were not expecting, so reject the next packet
      send_buf(packet, 1, server, SREJ, mySeqNum++, expected_seq_num, packet);
      return RECV_DATA;
   }
   
	if (header.flag == END_OF_FILE) {
		/*send EOF_ACK*/
      send_buf(packet, 1, server, EOF_ACK, mySeqNum++, expected_seq_num, packet);
      printf("--------------File done :)\n");
      return DONE;
	}
   else {
      /*send ACK*/
      send_buf(packet, 1, server, ACK, mySeqNum++, expected_seq_num, packet);
   }

   
   return RECV_DATA;
}