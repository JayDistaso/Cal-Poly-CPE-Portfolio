#include "clientServer.h"

//Must not be very long (20 lines?)@@@
int main(int argc, char *argv[]) {

   int port_number = 0, serversSocketFD;
   Handle *arrayOfHandles = calloc(DEFAULT_HANDLE_LIST_SIZE, sizeof(Handle));
   
   if (argc == 2) {
      port_number = atoi(argv[1]);
   }
   else if (argc != 1) {
      printf("Invalid arguments\nUsage: server [port-number]\n");
      exit(EXIT_FAILURE);
   }
   
   serversSocketFD = init_server_socket(port_number);
   arrayOfHandles[SERVER_CONNECTION].flag = serversSocketFD;
   
   if (listen(serversSocketFD, DEFAULT_QUE_SIZE) == -1) {
      perror("Server listen call error\n");
      exit(EXIT_FAILURE);
   }
   
   runServer(arrayOfHandles);
   
   return 0;
}
int check_connections(Handle *arrayOfHandles, int numberOfConnections) {
   fd_set fdSelectObject; //is their no length paramter? @@@
   int index, selectValue, fileDescriptorRangeToBeSelected = 0;
   struct timeval timeout;
   
   timeout.tv_sec = 0;
   timeout.tv_usec = 0;
   
   //Initializes the file descriptor set fdset to have zero bits for all file descriptors.
   FD_ZERO(&fdSelectObject);
   //Go through all connections
   for (index = 0; index < numberOfConnections; index++) {
      if (arrayOfHandles[index].flag) {   
         //Sets the bit for the file descriptor fd in the file descriptor set fdset.
         FD_SET(arrayOfHandles[index].flag, &fdSelectObject);
         if (arrayOfHandles[index].flag > fileDescriptorRangeToBeSelected) {
            fileDescriptorRangeToBeSelected = arrayOfHandles[index].flag;   
         }
      }
   }
   selectValue = select(fileDescriptorRangeToBeSelected + 1, &fdSelectObject, NULL, NULL, NULL);
   
   //If we timeout then try again
   if (selectValue == 0) {
      while (selectValue == 0) {
         selectValue = select(fileDescriptorRangeToBeSelected, &fdSelectObject, NULL, NULL, &timeout);
      }
   }
   
   if (selectValue == -1) {
      perror("Select Error\n");
      exit(EXIT_FAILURE);
   }
   //Go through all the connections, and return the first one ready to be read
   //This means lower (older) connections get priority. Top priority to new clients
   for (index = 0; index < numberOfConnections; index++) {
      if (arrayOfHandles[index].flag && FD_ISSET(arrayOfHandles[index].flag, &fdSelectObject)) {   
         return index;
      }
   }
   
   return NO_SOCKETS_TO_BE_READ; 
}

void runServer(Handle *arrayOfHandles) {
   int socket_index = NO_SOCKETS_TO_BE_READ; //Initial value until we check our sockets 
   int numberOfConnections = 1; //The server listening socket is only connection to start
   int size_of_handle_list = DEFAULT_HANDLE_LIST_SIZE; //We will need to grow this if too manyh connections
   int bytes_transmitted;
   int retVal;
   uint16_t messageLen;
   int total_packet_length;
   char *packet_buffer = (char *)calloc(1, PACKET_BUFFER_SIZE); //This is where the packets data will go
   ChatHeader *chatHeader = (ChatHeader *)calloc(1, sizeof(ChatHeader)); 


   while (1) {
      //Keep checking our sockets until we have one to read
      socket_index = NO_SOCKETS_TO_BE_READ;
      while (socket_index == NO_SOCKETS_TO_BE_READ) {
         socket_index = check_connections(arrayOfHandles, numberOfConnections);
      }
      //Server Listening socket wants to be read, so new client
      if (socket_index == SERVER_CONNECTION) {
         //Check if we have room for new connection first
         if (numberOfConnections == size_of_handle_list) {
            arrayOfHandles = updateHandleList(arrayOfHandles, &numberOfConnections, &size_of_handle_list);
         }
         add_new_client(arrayOfHandles, numberOfConnections++);
      }
               
      //Otherwise its something an existing connection wants, recv is like read
      else {
         if ((bytes_transmitted = recv(arrayOfHandles[socket_index].flag, packet_buffer, CHAT_HEADER_LENGTH, 0)) == -1) {
            perror("Reading socket error\n");
            exit(EXIT_FAILURE);
         }
         if (bytes_transmitted == 0) {
            close(arrayOfHandles[socket_index].flag);
            arrayOfHandles[socket_index].flag = 0;  //This handle is retired
         }
         else {
            memcpy(chatHeader, packet_buffer, CHAT_HEADER_LENGTH);
            messageLen = ntohs(chatHeader->packet_length);
            //Ony try to recv if there is more to get
            if (messageLen > CHAT_HEADER_LENGTH) {
               if ((bytes_transmitted = recv(arrayOfHandles[socket_index].flag, packet_buffer + CHAT_HEADER_LENGTH, messageLen, 0) == -1)) {
                  perror("recv call while recieve rest of packet\n");
                  exit(EXIT_FAILURE);
               }   
            }

            if (chatHeader->flag == INITIAL_CONNECTION_FLAG) {               
               //The return value is the flag (2 for good, 3 for bad)
               retVal = add_new_handle(socket_index, packet_buffer, arrayOfHandles, numberOfConnections);
               
               total_packet_length = addChatHeader(packet_buffer, 0, retVal);
               if ((bytes_transmitted = send(arrayOfHandles[socket_index].flag, packet_buffer, total_packet_length, 0)) == -1) {
                  perror("server send response to inital packet error\n");
                  exit(EXIT_FAILURE);
               }
            }
            else if (chatHeader->flag == MESSAGE_FLAG) {
               process_message_packet(ntohs(chatHeader->packet_length), socket_index, packet_buffer, arrayOfHandles, numberOfConnections);
            }
            else if (chatHeader->flag == CLIENT_REQUEST_HANDLE_LIST) {
               send_handle_list_to_client(socket_index, arrayOfHandles, numberOfConnections);
            }
            else if (chatHeader->flag == CLIENT_REQUESTING_TO_EXIT) {
               addChatHeader(packet_buffer, 0, CLIENT_ALLOWED_TO_EXIT);
               if ((send(arrayOfHandles[socket_index].flag, packet_buffer, CHAT_HEADER_LENGTH, 0)) == -1) {
                  perror("server send exit packet error\n");
                  exit(EXIT_FAILURE);
               }
            }
            else {
               printf("It looks like client index: %d sent a badly formed packet\n", socket_index); 
            } 
         }
      }
      
      
   }  
}

void send_handle_list_to_client(int socket_index, Handle *arrayOfHandles, int numberOfConnections) {
   int i = 0;
   uint32_t activeConnections = 0;
   char *packet_buffer = calloc(1, PACKET_BUFFER_SIZE);
   int offset = CHAT_HEADER_LENGTH;
   uint8_t handle_length;
   int bytes_transmitted;
   int total_packet_length;
   
   //Skip the first because its the server one
   for (i = 1; i < numberOfConnections; i++) {
      if (arrayOfHandles[i].flag) {
         activeConnections++;
      }
   }
   activeConnections = htonl(activeConnections);
   memcpy(packet_buffer + offset, &activeConnections, sizeof(uint32_t));
   
   offset += sizeof(uint32_t);
   total_packet_length = addChatHeader(packet_buffer, offset - CHAT_HEADER_LENGTH, INCOMING_HANDLE_NUM_FLAG);

   //Send the number of handles to expect
   if ((bytes_transmitted = send(arrayOfHandles[socket_index].flag, packet_buffer, total_packet_length, 0)) == -1) {
      perror("server send message packet error\n");
      exit(EXIT_FAILURE);
   }

   
   for (i = 1; i < numberOfConnections; i++) {
      offset = CHAT_HEADER_LENGTH;
      if (arrayOfHandles[i].flag) {
         handle_length = strlen(arrayOfHandles[i].handle);
         memcpy(packet_buffer + offset, &handle_length, sizeof(uint8_t));
         offset++;
         memcpy(packet_buffer + offset, arrayOfHandles[i].handle, handle_length);
         offset += handle_length;
         total_packet_length = addChatHeader(packet_buffer, offset - CHAT_HEADER_LENGTH, INCOMING_HANDLE);
         //Sednt eh handle and length
         if ((bytes_transmitted = send(arrayOfHandles[socket_index].flag, packet_buffer, total_packet_length, 0)) == -1) {
            perror("server send message packet error\n");
            exit(EXIT_FAILURE);
         }  
      }
   }
   
   total_packet_length = addChatHeader(packet_buffer, 0, END_OF_HANDLE_LIST);
   
   
   
}

int process_message_packet(uint16_t packet_length, int socket_index, char *packet_buffer, Handle *arrayOfHandles, int numberOfConnections) {
   
   //int packet_length_so_far = 0;
   int i, offset = CHAT_HEADER_LENGTH;
   uint8_t handle_length;
   memcpy(&handle_length, packet_buffer + offset++, sizeof(uint8_t)); //length of handle, at first this is the sending handle

   char *handle_name = calloc(HANDLE_SIZE, sizeof(char));
   memcpy(handle_name, packet_buffer + offset, handle_length);
   offset += handle_length;
   
   uint8_t number_of_destination_handles;
   memcpy(&number_of_destination_handles, packet_buffer + offset++, sizeof(uint8_t));
   int handle_index, bytes_transmitted;

   //Offset is past the chat header, client handle length, client handle, number of handles
   
   
   
   if (look_for_handle(arrayOfHandles, numberOfConnections, handle_name) == HANDLE_NOT_FOUND) {
      printf("A client with an unknown handle (%s) tried to send a message.\n", handle_name);
      return UNDEFINED_BEHAVIOR;
   }
   
   if (number_of_destination_handles < 1 || number_of_destination_handles > 9) {
      printf("A client tried to send to a number of destinations that is not allowed (%u)\n", number_of_destination_handles);
      return UNDEFINED_BEHAVIOR;
   }
   for (i = 0; i < number_of_destination_handles; i++) {
      //Copy in the handle length
      memcpy(&handle_length, (packet_buffer + offset), 1);
      offset++;
      //Copy in the handle
      memset(handle_name, 0, HANDLE_SIZE);
      memcpy(handle_name, (packet_buffer + offset), handle_length);
      offset += handle_length;
      //The client is trying to send a message to somebody who does not exist
      if ((handle_index = look_for_handle(arrayOfHandles, numberOfConnections, handle_name)) == HANDLE_NOT_FOUND) {
         send_error_packet(arrayOfHandles[socket_index], handle_name, handle_length); 
         //printf("Client tried to send a message to a handle that does not exist (%s)\n", handle_name);
      }
      //Send message to the client
      else {
         if ((bytes_transmitted = send(arrayOfHandles[handle_index].flag, packet_buffer, packet_length, 0)) == -1) {
            perror("server send message packet error\n");
            exit(EXIT_FAILURE);
         }
      }     
   }
   return 0;
}

void send_error_packet(Handle destination_handle, char *failed_handle_name, uint8_t handle_length) {
   char *packet_buffer = calloc(1, PACKET_BUFFER_SIZE);
   memcpy(packet_buffer + CHAT_HEADER_LENGTH, &handle_length, 1);
   memcpy(packet_buffer + CHAT_HEADER_LENGTH + 1, failed_handle_name, handle_length);
   int total_packet_length = addChatHeader(packet_buffer, handle_length + 1, ERROR_PACKET);
   int bytes_transmitted;
   //printf("The total packetl should be %d\n" 
   if ((bytes_transmitted = send(destination_handle.flag, packet_buffer, total_packet_length, 0)) == -1) {
      perror("server send message packet error\n");
      exit(EXIT_FAILURE);
   }
   free(packet_buffer); 
} 

void sendMessage(int destination_socket, char *packet_buffer) {
   
}


int add_new_handle(int socket_index, char *packet_buffer, Handle *arrayOfHandles, int numberOfConnections) {
   uint8_t handle_length;
   memcpy(&handle_length, packet_buffer + CHAT_HEADER_LENGTH, sizeof(uint8_t));
   char handle_to_be_added[handle_length];
   memcpy(handle_to_be_added, packet_buffer + CHAT_HEADER_LENGTH + 1, handle_length);
   if (look_for_handle(arrayOfHandles, numberOfConnections, handle_to_be_added) == HANDLE_NOT_FOUND) {
      memcpy(arrayOfHandles[socket_index].handle, handle_to_be_added, handle_length);
      return GOOD_HANDLE;
   }
   else {
      return BAD_HANDLE;
   }
}

int look_for_handle(Handle *arrayOfHandles, int numberOfConnections, char *handle) {
   int index;
   for (index = 0; index < numberOfConnections; index++) {
      if (arrayOfHandles[index].flag != 0 && strncmp(arrayOfHandles[index].handle, handle, HANDLE_SIZE) == 0) {
         return index;
      }
   }
   return HANDLE_NOT_FOUND;
}

Handle *updateHandleList(Handle *arrayOfHandles, int *numberOfConnections, int *size_of_handle_list) {
   //If list is getting too full double the size of the list
   void *temp = calloc(sizeof(Handle), *size_of_handle_list * 2);
   memcpy(temp, (void *)arrayOfHandles, *size_of_handle_list * sizeof(Handle));
   *size_of_handle_list *= 2;
   
   free(arrayOfHandles);
   return temp;
}


void add_new_client(Handle *arrayOfHandles, int numberOfConnections) {
   int new_socket;
   if ((new_socket = accept(arrayOfHandles[SERVER_CONNECTION].flag, NULL, NULL)) == -1) {
      perror("Accepting new socket error\n");
      exit(EXIT_FAILURE);
   }
   arrayOfHandles[numberOfConnections].flag = new_socket;
}

int init_server_socket(int port_number) {
   
   struct sockaddr_in6 socketAddress;
   socklen_t len = sizeof(socketAddress);
   
   //AF_INET = ivp4/6, SOCK_STREAM = bitstream over socket
   int serversSocketFD = socket(AF_INET6, SOCK_STREAM, 0);
   
   if (serversSocketFD == -1) {
      perror("Creating servers socket failed\n");
      exit(EXIT_FAILURE);
   }
   
   socketAddress.sin6_family = AF_INET6;
   socketAddress.sin6_port = htons(port_number);
   socketAddress.sin6_addr = in6addr_any;
   
   if (bind(serversSocketFD, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) == -1) {
      perror("Binding server socket error\n");
      exit(EXIT_FAILURE);
   }
   
   if (getsockname(serversSocketFD, (struct sockaddr*)&socketAddress, &len) == -1) {
      perror("getsockname server error\n");
      exit(EXIT_FAILURE);
   }
	printf("Server Port Number %d \n", ntohs(socketAddress.sin6_port));
   
   return serversSocketFD;
}

int addChatHeader(char *packet_buffer, int packet_length_so_far, int flag) {
    ChatHeader chatHeader;
    
    chatHeader.flag = flag;
    //Length of chat-header/handles/payload
    chatHeader.packet_length = htons(packet_length_so_far + CHAT_HEADER_LENGTH);

    memcpy(packet_buffer, &chatHeader, CHAT_HEADER_LENGTH);
    
    return packet_length_so_far + CHAT_HEADER_LENGTH;
}

void printHeaderList(Handle *arrayOfHandles, int numberOfConnections) {

   int i;
   
   for (i = 0; i < numberOfConnections; i++) {
      printf("Index %d | flag = %d | name = %s\n", i, arrayOfHandles[i].flag, arrayOfHandles[i].handle);
   }

}