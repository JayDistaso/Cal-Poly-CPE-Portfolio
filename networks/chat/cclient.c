#include "clientServer.h"
#include "gethostbyname6.h"

static Handle *arrayOfHandles;
static int numberBlocked = 0;
static int size_of_handle_list = DEFAULT_HANDLE_LIST_SIZE;
static int waitingForFlag11 = 0;

int main(int argc, char *argv[]) {
   
   if (argc != 4) {
      printf("Invalid arguments\n");
      printf("Usage:  cclient handle server-name server-port \n");
      exit(EXIT_FAILURE);
   }
   client_init(argv);
      return 0;   
}

/*Does initial setup including flag1*/
void client_init(char *argv[]) {
   
   int socket_index, total_packet_length, bytes_transmitted;
   uint8_t handle_length = 0;
   arrayOfHandles = calloc(DEFAULT_HANDLE_LIST_SIZE, sizeof(Handle));
   ChatHeader *chatHeader = calloc(1, sizeof(ChatHeader));  
   char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE);
   
   if ((handle_length = strlen(argv[1])) > HANDLE_SIZE) {
      printf("Handle Length cannot be greater than 250 characters\n");
      exit(EXIT_FAILURE);
   }  
   
   socket_index = init_client_socket(argv[2], argv[3]);
   
   constructInitialPacket(packet_buffer, argv[1]);
   //printf("strlen is %u\n", strlen(argv[1]));
   //printf("handle_length is %u\n", handle_length);
   total_packet_length = addChatHeader(packet_buffer, handle_length + 1, INITIAL_CONNECTION_FLAG);
   
   //Send Inital Packet (flag == 1) 
   if ((bytes_transmitted = send(socket_index, packet_buffer, total_packet_length, 0)) == -1) {
      perror("client send inital packet error\n");
      exit(EXIT_FAILURE);
   }
   
   while (chatHeader->flag != GOOD_HANDLE) {   
      if ((bytes_transmitted = recv(socket_index, packet_buffer, PACKET_BUFFER_SIZE, 0) == -1)) {
         perror("recv call while waiting for flag 2/3 packet\n");
         exit(EXIT_FAILURE);
      }
      memcpy(chatHeader, packet_buffer, sizeof(ChatHeader));
      //chatHeader = (struct chatHeader *)packet_buffer;
      if (chatHeader->flag == BAD_HANDLE) {
         printf("Handle already in use: %s\n", argv[1]);
         exit(EXIT_FAILURE);
      }
   }
   free(packet_buffer);
   free(chatHeader);
   runClient(socket_index, argv[1]);
}


//Returns packet total Length
int addChatHeader(char *packet_buffer, int packet_length_so_far, int flag) {
    ChatHeader chatHeader;
    
    chatHeader.flag = flag;
    //Length of chat-header/handles/payload
    chatHeader.packet_length = htons(packet_length_so_far + CHAT_HEADER_LENGTH);
    
    memcpy(packet_buffer, &chatHeader, CHAT_HEADER_LENGTH);
    
    return packet_length_so_far + CHAT_HEADER_LENGTH;
}

//Constructs flag1 packet
void constructInitialPacket (char *packet_buffer, char *handle_name) {
   //check on the null terminator shit
   //Put the handle length in packet
   int handle_length = strlen(handle_name);
   memcpy(packet_buffer + CHAT_HEADER_LENGTH, &handle_length, sizeof(char));
   //Put the handle in packet
   memcpy(packet_buffer + 1 + CHAT_HEADER_LENGTH, handle_name, handle_length);
}

//The main function, calls select to see where to branch
void runClient(int socket_index, char *my_handle_name) {
   
   int retVal;
   printf("$: ");
   fflush(stdout);
   while (1) {
      retVal = listen_to_sockets(socket_index);
      if (retVal == socket_index) {
         printf("\n");
         recieve_packet_from_server(socket_index);
         printf("\n$: ");
         fflush(stdout);
      }
      else if (retVal == STD_IN_FD) {
         process_user_input(socket_index, my_handle_name);
      }
   }
}

int recieve_packet_from_server(int socket_num) {
   
   char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE);
   uint8_t bytes_transmitted = 1;
   uint16_t messageLen;
   ChatHeader *chatHead = calloc(1, sizeof(ChatHeader));
   static int packets_recieved = 0;
   packets_recieved++;

   if ((bytes_transmitted = recv(socket_num, packet_buffer, CHAT_HEADER_LENGTH, MSG_WAITALL) == -1)) {
         perror("recv call while recieve PL\n");
         exit(EXIT_FAILURE);
   }
   

   memcpy(chatHead, packet_buffer, CHAT_HEADER_LENGTH);
   messageLen = ntohs(chatHead->packet_length);
   if (messageLen > CHAT_HEADER_LENGTH) {
   
      if ((bytes_transmitted = recv(socket_num, packet_buffer + CHAT_HEADER_LENGTH, messageLen - CHAT_HEADER_LENGTH, MSG_WAITALL) == -1)) {
            perror("recv call while recieve rest of packet\n");
            exit(EXIT_FAILURE);
      }
   }

   if (chatHead->flag == MESSAGE_FLAG) {
      process_recieved_message(packet_buffer, chatHead);
   }
   else if (chatHead->flag == ERROR_PACKET) {
      process_error_packet(packet_buffer);  
   }
   else if (chatHead->flag == INCOMING_HANDLE_NUM_FLAG) {
      if (!waitingForFlag11) {
         printf("Recieved a Flag:11 (Incoming handle list) unexpectedly\n");
      }
      else {
         waitingForFlag11 = 0;
         process_incoming_handle_list(socket_num, packet_buffer);
      }
   }
   else if (chatHead->flag == INCOMING_HANDLE) {
      print_incoming_handle(socket_num, packet_buffer);
   }
   else if (chatHead->flag == END_OF_HANDLE_LIST) {
      //Consume the 13 flag, no action needed
   }
   else if (chatHead->flag == CLIENT_ALLOWED_TO_EXIT) {
      exit(EXIT_SUCCESS);
   }
   else {
      printf("Server Terminated\n");
      exit(EXIT_SUCCESS);
   } 
   free(chatHead);
   free(packet_buffer);
   return messageLen;
}

void print_incoming_handle(int socket_num, char *packet_buffer) {
   char *handle_name = calloc(HANDLE_SIZE, 1);
   uint8_t handle_length;
   memcpy(&handle_length, packet_buffer + CHAT_HEADER_LENGTH, 1);
   memcpy(handle_name, packet_buffer + CHAT_HEADER_LENGTH + 1, handle_length);
   printf("\t%s\n", handle_name);
   free(handle_name);
}

void process_incoming_handle_list(int socket_num, char *packet_buffer) {
   uint32_t number_of_incoming_handles;
   int i;
   memcpy(&number_of_incoming_handles, packet_buffer + CHAT_HEADER_LENGTH, sizeof(uint32_t));
   number_of_incoming_handles = ntohl(number_of_incoming_handles);
   printf("Number of clients: %d\n", number_of_incoming_handles);
   for (i = 0; i < number_of_incoming_handles; i++) {
      recieve_packet_from_server(socket_num);   
   }
   //recieve_packet_from_server(socket_num);
}

void process_error_packet(char *packet_buffer) {
   char *failed_handle = (char *)calloc(1, HANDLE_SIZE);
   uint8_t handle_length;
   memcpy(&handle_length, packet_buffer + CHAT_HEADER_LENGTH, 1);
   memcpy(failed_handle, packet_buffer + CHAT_HEADER_LENGTH + 1, handle_length);
   printf("Client with handle %s does not exist\n", failed_handle);
}  

void process_recieved_message(char *packet_buffer, ChatHeader *chatHead) {
   char *source_handle = (char *)calloc(HANDLE_SIZE, 1);
   uint8_t handle_length;
   int i;
   uint8_t numberOfDestinationHandles;
   int offset = CHAT_HEADER_LENGTH; 
   
   char *textBuffer = calloc(MAX_MESSAGE_LENGTH, 1);
   memcpy(&handle_length, packet_buffer + offset++, sizeof(uint8_t));
   memcpy(source_handle, packet_buffer + offset, handle_length);
   
   if (look_for_blocked_handle(source_handle) == HANDLE_NOT_FOUND) {
      printf("%s: ", source_handle);
      offset += handle_length;
      memcpy(&numberOfDestinationHandles, packet_buffer + offset, sizeof(uint8_t));
      offset++;
      //Skip past destination handles so we can get the text
      for (i = 0; i < numberOfDestinationHandles; i++) {
         memcpy(&handle_length, packet_buffer + offset++, sizeof(uint8_t));
         offset += handle_length;   
      }
      //The rest should be text now
      memcpy(textBuffer, packet_buffer + offset, ntohs(chatHead->packet_length) - offset);
      
      printf("%s", textBuffer);
      fflush(stdout);     
   }

   free(source_handle);
}


void process_user_input(int socket_index, char *my_handle_name) {
   //char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE); //This is where the packets data will go
   char *input_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE);
   char *token;
   const char delim = ' ';
   char *input_buffer_backup = (char *)calloc(1, PACKET_BUFFER_SIZE); //strtok will kill our original one 

   fgets(input_buffer, PACKET_BUFFER_SIZE, stdin);
   
   memcpy(input_buffer_backup, input_buffer, PACKET_BUFFER_SIZE);
   
   if (strlen(input_buffer) > MAX_USER_INPUT) {
      printf("STDIN should never exceed 1000 chars\n");
      exit(EXIT_FAILURE);
   }
      
   token = strtok(input_buffer, &delim);

   if (token[0] != '%') {
      usage_failure();
   }
   if (token[1] == 'm' || token[1] == 'M') {
      send_message_to_server(socket_index, my_handle_name, input_buffer_backup);
   }
   else if (token[1] == 'b' || token[1] == 'B') {
      process_block_request(socket_index, input_buffer_backup);
   }
   else if (token[1] == 'u' || token[1] == 'U') {
      process_unblock_request(socket_index, input_buffer_backup);
   }
   else if (token[1] == 'l' || token[1] == 'L') {
      request_handle_list_from_server(socket_index, input_buffer_backup);
   }
   else if (token[1] == 'e' || token[1] == 'E') {
      request_exit(socket_index);
   }
}

void request_exit(int socket_index) {
   char *packet_buffer = calloc(CHAT_HEADER_LENGTH, 1);
   addChatHeader(packet_buffer, 0, CLIENT_REQUESTING_TO_EXIT);
   if ((send(socket_index, packet_buffer, CHAT_HEADER_LENGTH, 0)) == -1) {
      perror("client send request exit packet error\n");
      exit(EXIT_FAILURE);
   }
   free(packet_buffer); 
}

void request_handle_list_from_server(int socket_index, char *input_buffer_backup) {

   char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE); //This is where the packets data will go
   uint8_t bytes_transmitted;
   
   addChatHeader(packet_buffer, 0, CLIENT_REQUEST_HANDLE_LIST);
   
   if ((bytes_transmitted = send(socket_index, packet_buffer, CHAT_HEADER_LENGTH, 0)) == -1) {
      perror("client send inital packet error\n");
      exit(EXIT_FAILURE);
   }
   waitingForFlag11 = 1;
}
     
void process_unblock_request(int socket_index, char *input_buffer_backup) {
   char *token;
   token = strtok(NULL, " \n");
   int index;
   
   if (token == NULL) {
      usage_failure();
   }
   if ((index = look_for_blocked_handle(token)) == HANDLE_NOT_FOUND) {
      printf("Unblock failed, handle %s is not blocked\n", token);
   }
   else {
      arrayOfHandles[index].flag = 0;
   }
   
}
void process_block_request(int socket_index, char *input_buffer_backup) {
   
   char *token;
   token = strtok(NULL, " \n");
   if (token == NULL) {
      usage_failure();
   }
   if (look_for_blocked_handle(token) != HANDLE_NOT_FOUND) {
      printf("Block failed, %s is already blocked\n", token);
   }
   else {
      arrayOfHandles[numberBlocked].flag = 1;
      memcpy(arrayOfHandles[numberBlocked++].handle, token, strlen(token)); 
   }
   printf("Blocked: ");
   printBlockedList();

}

void printBlockedList() {
   int i;
   
   for (i = 0; i < numberBlocked; i++) {
      printf("%s", arrayOfHandles[i].handle);
      if (i + 1 < numberBlocked) {
         printf(", ");
      }
   }
   printf("\n");
}


int listen_to_sockets(int socket_num) {
   fd_set fdSelectObject;
   int selectValue, fileDescriptorRangeToBeSelected = STD_IN_FD;
   struct timeval timeout;
   
   timeout.tv_sec = 0;
   timeout.tv_usec = 0;
   
   //Initializes the file descriptor set fdset to have zero bits for all file descriptors.
   FD_ZERO(&fdSelectObject);
   FD_SET(STD_IN_FD, &fdSelectObject);
   FD_SET(socket_num, &fdSelectObject);
   if (fileDescriptorRangeToBeSelected < socket_num) {
      fileDescriptorRangeToBeSelected = socket_num;
   }
   
   selectValue = select(fileDescriptorRangeToBeSelected + 1, &fdSelectObject, NULL, NULL, NULL);
   
   //If we timeout then try again
   if (selectValue == 0) {
      while (selectValue == 0) {
         selectValue = select(fileDescriptorRangeToBeSelected, &fdSelectObject, NULL, NULL, &timeout);
         ///@@@ If you get stuck ina  while loop it could easily be here
      }
   }
      
   if (selectValue == -1) {
      perror("Select Error\n");
      exit(EXIT_FAILURE);
   }
   if (FD_ISSET(socket_num, &fdSelectObject)) {
      return socket_num;
   }
   else if (FD_ISSET(STD_IN_FD, &fdSelectObject)) {
      return STD_IN_FD;
   }
   
   return NO_SOCKETS_TO_BE_READ; 
}

void send_message_to_server(int socket_index, char *my_handle_name, char *input_buffer) {
   
   char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE); //This is where the packets data will go
   char *token;
   char *destination_handle;
   char *text;
   uint8_t num_handles = 1;
   int offset = CHAT_HEADER_LENGTH;
   int total_packet_length, i, textLength;
   const char delim = ' ';
   
   //Start constructing packet with sender handle length
   uint8_t handle_length = strlen(my_handle_name);
   memcpy(packet_buffer + offset, &handle_length, 1); 
   offset++;
   //Then sender handle
   memcpy(packet_buffer + offset, my_handle_name, handle_length);

   offset += handle_length;
   
   token = strtok(NULL, &delim);
   //handle number was specified
   if (token == NULL) {
      usage_failure();
   }
   if (isdigit(token[0])) {
      num_handles = atoi(token);
      token = strtok(NULL, &delim);
   }
   else if (!isalpha(token[0])) {
      printf("Input after %m should be a letter or a number\n");
      exit(EXIT_FAILURE);
   }
   
   //Now number of handles
   memcpy(packet_buffer + offset, &num_handles, 1);

   offset += 1;
   
   //Put in each destination handle
   for (i = 0; i < num_handles; i++) {
      if (!(destination_handle = token)) {
         usage_failure();
      }
      //Advance token for next read of handle
      if (i + 1 < num_handles) {
         token = strtok(NULL, &delim);
      }
      
      //Destination handle length
      handle_length = strlen(destination_handle);
      memcpy(packet_buffer + offset, &handle_length, 1);
      offset += 1;
      //destination handle
      memcpy(packet_buffer + offset, destination_handle, handle_length);
      offset += handle_length;
   }
   
   //token = strtok(NULL, &delim);
   text = strtok(NULL, "\n");
   if (text == NULL) {
      textLength = 0;
   }
   else {
      textLength = strlen(text);
   }
   while (textLength > MAX_MESSAGE_LENGTH) {
      //Put in first part of text
      memcpy(packet_buffer + offset, text, MAX_MESSAGE_LENGTH);
      total_packet_length = addChatHeader(packet_buffer, offset + MAX_MESSAGE_LENGTH, MESSAGE_FLAG);
      if (send(socket_index, packet_buffer, total_packet_length, 0) == -1) {
         perror("client send message packet error\n");
         exit(EXIT_FAILURE);
      }

      textLength -= MAX_MESSAGE_LENGTH;
      text += MAX_MESSAGE_LENGTH;
   }
   memcpy(packet_buffer + offset, text, textLength);
   
   total_packet_length = addChatHeader(packet_buffer, offset + textLength, MESSAGE_FLAG);

   if (send(socket_index, packet_buffer, total_packet_length, 0) == -1) {
     perror("client send message packet error\n");
     exit(EXIT_FAILURE);
   }
}
 
 
void usage_failure() {
   printf("Error: Usage %%M, %%B, %%U, %%L %%E\n");
   printf("Messaging: %%m [num-handles] destination-handle [destination-handle] ... [text]\n");
   printf("Unblocking: %%u handle_to_be_unblocked\n");
   printf("Blocking: %%b handle_to_be_blocked\n");
   printf("Exit: %%e\n");
   printf("List Handles: %%l\n");
   exit(EXIT_FAILURE);
}    

int init_client_socket(char *server_name, char *server_port) {
   
   struct sockaddr_in6 socketAddress;
   uint8_t *ipAddress = NULL;
   
   int socket_index = socket(AF_INET6, SOCK_STREAM, 0);
   
   if (socket_index == -1) {
      perror("Creating client socket error\n");
      exit(EXIT_FAILURE);
   }
   
   socketAddress.sin6_family = AF_INET6;
   socketAddress.sin6_port = htons(atoi(server_port));

   if ((ipAddress = getIPAddress6(server_name, &socketAddress)) == NULL)
	{
      perror("getting IPV6 address error\n");
      exit(EXIT_FAILURE);
	}
   
   if (connect(socket_index, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) == -1)
	{
	   perror("connect call error");
	   exit(EXIT_FAILURE);
   }
   
   return socket_index;
}

int look_for_blocked_handle(char *handle) {
   int index;
   for (index = 0; index < numberBlocked; index++) {
      if (arrayOfHandles[index].flag && strncmp(arrayOfHandles[index].handle, handle, HANDLE_SIZE) == 0) {
         return index;
      }
   }
   return HANDLE_NOT_FOUND;
}
void updateBlockList() {
   //If list is getting too full double the size of the list
   void *temp = calloc(sizeof(Handle), size_of_handle_list * 2);
   memcpy(temp, (void *)arrayOfHandles, size_of_handle_list * sizeof(Handle));
   size_of_handle_list *= 2;
   
   //If entire list gets destroyed this is where that happened @@
   free(arrayOfHandles);
   arrayOfHandles = temp;
}