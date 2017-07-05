//Justin Distaso 
//CPE464 
//Program 2: chat client and server
//Due 5/1/17

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h> 
#include <ctype.h>
#include <unistd.h>

#include "networks.h"
#include "gethostbyname6.h"

#define HANDLE_SIZE 255
#define DEFAULT_HANDLE_LIST_SIZE 2
#define DEFAULT_QUE_SIZE 10 
#define NO_SOCKETS_TO_BE_READ -2
#define SERVER_CONNECTION 0
#define PACKET_BUFFER_SIZE 1024 //max data should be 1003 (except in special case covered in class), but nice powers of two
#define CHAT_HEADER_LENGTH 3
#define INITIAL_CONNECTION_FLAG 1
#define HANDLE_NOT_FOUND -3
#define GOOD_HANDLE 2
#define BAD_HANDLE 3
#define UNDEFINED_BEHAVIOR -4
#define MAX_MESSAGE_LENGTH 200
#define ERROR_PACKET 7
#define MAX_USER_INPUT 1000
#define MESSAGE_FLAG 5
#define STD_IN_FD 1
#define CLIENT_REQUEST_HANDLE_LIST 10
#define INCOMING_HANDLE_NUM_FLAG 11
#define INCOMING_HANDLE 12
#define END_OF_HANDLE_LIST 13
#define CLIENT_ALLOWED_TO_EXIT 9
#define CLIENT_REQUESTING_TO_EXIT 8

typedef struct Handle {
   int flag;
   char handle[HANDLE_SIZE];
}__attribute__((packed)) Handle;

typedef struct ChatHeader {
   uint16_t packet_length;
   uint8_t flag;
}__attribute__((packed)) ChatHeader;


//Fucntion prototypes for client.c
int addChatHeader(char *packet_buffer, int packet_length_so_far, int flag);
void constructInitialPacket (char *packet_buffer, char *handle_name);
int init_client_socket(char *server_name, char *server_port);
void runClient(int socket_index, char *my_handle_name);
void send_message_to_server(int socket_index, char *my_handle_name, char *input_buffer);
void usage_failure();
int recieve_packet_from_server(int socket_num);
void process_user_input(int socket_index, char *my_handle_name);
int listen_to_sockets(int socket_num);
void process_recieved_message(char *packet_buffer, ChatHeader *chatHead);
void process_error_packet(char *packet_buffer);
void printBlockedList();
void process_block_request(int socket_index, char *input_buffer_backup);
int look_for_blocked_handle(char *handle);
void updateBlockList();
void process_unblock_request(int socket_index, char *input_buffer_backup);
void request_handle_list_from_server(int socket_index, char *input_buffer_backup);
void process_incoming_handle_list(int socket_num, char *packet_buffer);
void print_incoming_handle(int socket_num, char *packet_buffer);
void request_exit(int socket_index);
void client_init(char *argv[]);

//Function prototypes for server.c
int init_server_socket(int port_number);
int check_connections(Handle *arrayOfHandles, int numberOfConnections);
void add_new_client(Handle *arrayOfHandles, int numberOfConnections);
void runServer(Handle *arrayOfHandles);
Handle *updateHandleList(Handle *arrayOfHandles, int *numberOfConnections, int *size_of_handle_list);
char *construct_packet(char *data, int flag);
int process_message_packet(uint16_t packet_length, int socket_index, char *packet_buffer, Handle *arrayOfHandles, int numberOfConnections);
void send_error_packet(Handle destination_handle, char *failed_handle_name, uint8_t handle_length);
int add_new_handle(int socket_index, char *packet_buffer, Handle *arrayOfHandles, int numberOfConnections);
int look_for_handle(Handle *arrayOfHandles, int numberOfConnections, char *handle);
void printHeaderList(Handle *arrayOfHandles, int numberOfConnections);
void send_handle_list_to_client(int socket_index, Handle *arrayOfHandles, int numberOfConnections);


//Good info on select functions
//http://www.mkssoftware.com/docs/man3/select.3.asp
