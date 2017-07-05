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

//#include "checksum.c"

//Ethernet header constants
#define ETHERNET_ADDRESS_LENGTH 6
#define EtherTypeLength 2

//ARP header constants
#define ARPAddressLength 6
#define ARPTypeLength 2
#define ProtocolLength 2
#define SIZE_OF_OPER 2

//ICMP header constants
#define icmpChecksumLength 2
#define icmpRestOfHeaderLength 4

//TCP header constants
#define tcpPortLength 2
#define tcpSequenceNumberLength 4

//UDP header contants
#define udpPortLength 2
#define udpLength 2
#define udpChecksumLength 2

//IP header constants
#define DSCP_LENGTH 2
#define TWO_BYTES 2
#define IP_ID_LENGTH 2
#define IP_HEADER_CHECKSUM_LENGTH 2
#define IP_ADDRESS_LENGTH 4

//main constants
#define FILENAME_ARGUMENT 1
#define NO_MORE_PACKETS_FROM_SAVEFILE -2
#define PCAP_ERROR -1
#define ETHERNET_ERROR -2
#define ARP_BYTE_ONE 8
#define ARP_BYTE_TWO 6
#define IP_BYTE_ONE 8
#define IP_BYTE_TWO 0
#define IP_TYPE 8
#define ARP_REPLY 2
#define ARP_REQUEST 1
#define UDP_PROTOCOL_CODE 17
#define TCP_PROTOCOL_CODE 6
#define ICMP_PROTOCOL_CODE 1
#define IHL_MASK 0x0F
#define SIZE_OF_ETHERNET_PACKET 14
#define HTTP_PORT 80
#define TELNET_PORT 23 
#define FTP_PORT 20
#define POP3_PORT 110
#define SMTP_PORT 25
#define DNS_PORT 53
#define ICMP_REQUEST 8
#define ICMP_REPLY 0
#define BUFFER_SIZE 4096

  
//Ethernet Header
typedef struct Ethernet_Header {
   //dest 6 octets
   char destination[ETHERNET_ADDRESS_LENGTH];
   //source 6 octets
   char source[ETHERNET_ADDRESS_LENGTH];
   //EtherType 2 octets
   char EtherType[EtherTypeLength];
   //Optional IEE 802.1Q tag
   //?
} Ethernet_Header;


//ARP Header Request
typedef struct ARP_Header {
   char hardware_type[ARPTypeLength];
   char protocol_type[ARPTypeLength];
   char hardware_address_length;
   char protocol_address_length;
   
   //1 for request, 2 for reply
   char operation[SIZE_OF_OPER];

   char sender_hardware_address[ARPAddressLength];
   struct in_addr sender_protocol_address;
   char target_hardware_address[ARPAddressLength];
   struct in_addr target_protocol_address;
} ARP_Header;

//ICMP Header Echo request
typedef struct icmp_header {
   //0 is echo reply, 8 is echo request
   char type;
   char code;
   uint16_t checksum;
   uint32_t icmp_end;
} icmp_header;

typedef struct tcp_header {
   uint16_t source_port;
   uint16_t destination_port;
   uint32_t sequence_number;
   uint32_t acknowledgment_number;
   uint8_t data_offset;
   uint8_t flags;
   uint16_t window_size;
   uint16_t checksum;
   uint16_t urgent_pointer;   
} __attribute__((packed)) tcp_header;

//UDP Header
typedef struct udp_header {
   uint16_t source_port;
   uint16_t destination_port;
   char length[udpLength];
   char checksum[udpChecksumLength];
} udp_header; 

typedef struct IP_Header {
   char version;
   char dscp;
   uint16_t totalLength;
   char id[IP_ID_LENGTH];
   char flags[TWO_BYTES];
   uint8_t timeToLive;
   char protocol;
   char headerChecksum[IP_HEADER_CHECKSUM_LENGTH];
   struct in_addr sourceIPAddress;
   struct in_addr destinationIPAddress;
} IP_Header;

typedef struct psudoAndRealHeader {
   struct in_addr sourceAddress;
   struct in_addr destinationAddress;
   uint8_t reserved;
   uint8_t protocol;
   uint16_t tcpLength;
   //struct tcp_header realHeader;
} __attribute__((packed)) psudoAndRealHeader;

//Function declerations
ARP_Header *parse_arp(const u_char *pkt_data);
IP_Header *parse_ip(const u_char *pkt_data);
Ethernet_Header *parse_ethernet_header(const u_char *pkt_data);  
void parse_udp(const u_char *pkt_data, int ihl);
void parse_tcp(const u_char *pkt_data, int ihl, IP_Header *ipHeader);
void parse_icmp(const u_char *pkt_data, int ihl);
void print_ports(int port);
unsigned short in_cksum(unsigned short *addr, int len);
void print_tcpInfo(struct tcp_header *tcpHeader);