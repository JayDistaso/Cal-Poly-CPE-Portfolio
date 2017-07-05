//Justin Distaso 
//CPE464 
//Program 1: Trace
//Due 4/17/17

#include "trace.h"

int main(int argc, char *argv[]) {
  
  char *filename = argv[FILENAME_ARGUMENT];
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *pcap_object;
  struct pcap_pkthdr *pkt_header;
  const u_char *pkt_data;
  int retVal;
  int packets_read = 0;
  int offset;
  
  Ethernet_Header *ethernetHeader;
  
  //On windows the stream should be opened in binary mode
  pcap_object = pcap_open_offline(filename, errbuf);
  if (!pcap_object) {
      printf("Opening the pcap file failed\n");
      printf("%s\n", errbuf); 
      exit(PCAP_ERROR);
  }
  
  //Go through each packet until no more packets
  while(1) {
      retVal = pcap_next_ex(pcap_object, &pkt_header, &pkt_data);    
      
      if (retVal == NO_MORE_PACKETS_FROM_SAVEFILE) {
         pcap_close(pcap_object);
         break;
      }
      else if(retVal == PCAP_ERROR) {
         printf("There was an error while reading pcap object\n");
         printf("%s\n", pcap_geterr(pcap_object));
         exit(PCAP_ERROR);
      }
      
      printf("\nPacket number: %d  Packet Len: %d\n", ++packets_read, pkt_header->len);
      ethernetHeader = parse_ethernet_header(pkt_data);
      printf("\t\tType: ");   

      //Branch off from ethernet depending on type
      if (ethernetHeader->EtherType[0] == IP_BYTE_ONE && ethernetHeader->EtherType[1] == IP_BYTE_TWO) {
         printf("IP\n");
         parse_ip(pkt_data);
      }
      else if (ethernetHeader->EtherType[0] == ARP_BYTE_ONE && ethernetHeader->EtherType[1] == ARP_BYTE_TWO) {
         printf("ARP\n");
         parse_arp(pkt_data);
      }
      else {
         printf("Unknown");
         printf("Unknown PDU\n");
      }
  }
}

void parse_icmp(const u_char *pkt_data, int ihl) {
   struct icmp_header *icmpHeader = calloc(sizeof(icmp_header), 1);
   char offset = SIZE_OF_ETHERNET_PACKET + ihl * 4;
   
   //copy in struct from packet
   memcpy(&(icmpHeader->type), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(icmpHeader->code), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(icmpHeader->checksum), pkt_data + offset, icmpChecksumLength);
   offset += icmpChecksumLength;
   memcpy(&(icmpHeader->icmp_end), pkt_data + offset, icmpRestOfHeaderLength);
   offset += icmpRestOfHeaderLength;
   
   printf("\tICMP Header\n");
   printf("\t\tType: ");
   if (icmpHeader->type == ICMP_REQUEST) {
      printf("Request\n");
   }
   else if (icmpHeader->type == ICMP_REPLY) {
      printf("Reply\n");
   }
   else {
      printf("%d\n", icmpHeader->type);
   }
   free(icmpHeader);
}

void parse_tcp(const u_char *pkt_data, int ihl, IP_Header *ipHeader) {
   struct tcp_header *tcpHeader = calloc(sizeof(tcp_header), 1);
   char offset = SIZE_OF_ETHERNET_PACKET + ihl * 4;
   int checksum = 0;
   int checksum1 = 0;
   
   //Copy over tcp struct
   memcpy(&(tcpHeader->source_port), pkt_data + offset, sizeof(uint16_t));
   offset += sizeof(uint16_t);
   memcpy(&(tcpHeader->destination_port), pkt_data + offset, sizeof(uint16_t));
   offset += sizeof(uint16_t);
   memcpy(&(tcpHeader->sequence_number), pkt_data + offset, sizeof(uint32_t));
   offset += sizeof(uint32_t);
   memcpy(&(tcpHeader->acknowledgment_number), pkt_data + offset, sizeof(uint32_t));
   offset += sizeof(uint32_t);
   memcpy(&(tcpHeader->data_offset), pkt_data + offset, sizeof(uint8_t));
   offset += sizeof(uint8_t);
   memcpy(&(tcpHeader->flags), pkt_data + offset, sizeof(uint8_t));
   offset += sizeof(uint8_t);
   memcpy(&(tcpHeader->window_size), pkt_data + offset, sizeof(uint16_t));
   offset += sizeof(uint16_t);
   memcpy(&(tcpHeader->checksum), pkt_data + offset, sizeof(uint16_t));
   offset += sizeof(uint16_t);
   memcpy(&(tcpHeader->urgent_pointer), pkt_data + offset, sizeof(uint16_t));
   offset += sizeof(uint16_t);
   
   print_tcpInfo(tcpHeader);
   
   //Construct our psuedo header
   u_char *buffer = calloc(1, BUFFER_SIZE);
   psudoAndRealHeader tempHeader;
   tempHeader.sourceAddress = ipHeader->sourceIPAddress;
   tempHeader.destinationAddress = ipHeader->destinationIPAddress;
   tempHeader.reserved = 0;
   tempHeader.protocol = TCP_PROTOCOL_CODE;
   tempHeader.tcpLength = ntohs(ipHeader->totalLength) - (ihl * 4);
   tempHeader.tcpLength = htons(tempHeader.tcpLength);
   
   //Sadly this does clobber the data packet, but my tcp structure was just not cutting it
   //likely because I forgot to properly copy over the tcp data
   //Puts the psuedo header directly before the tcp so it can be passed in.
   memcpy((void *)pkt_data + SIZE_OF_ETHERNET_PACKET + (ihl * 4) - 12, &tempHeader, sizeof(struct psudoAndRealHeader));
   checksum = ntohs(tcpHeader->checksum);
   if (!(in_cksum((unsigned short *)(pkt_data + SIZE_OF_ETHERNET_PACKET + (ihl * 4) - 12), (int)(ntohs(tempHeader.tcpLength) + sizeof(psudoAndRealHeader))))) {

   //if (!(in_cksum((unsigned short *)buffer, ntohs(tempHeader.tcpLength) + sizeof(psudoAndRealHeader)))) {
      printf("\t\tChecksum: Correct (0x%04x)\n", checksum);
   }
   else {
      printf("\t\tChecksum: Incorrect (0x%04x)\n", checksum);
   }

   free(tcpHeader);
}

void print_tcpInfo(struct tcp_header *tcpHeader) {
   printf("\n\tTCP Header\n");
   printf("\t\tSource Port:  ");
   
   int port = ntohs(tcpHeader->source_port);
   print_ports(port);   
   printf("\t\tDest Port:  ");
   
   port = ntohs(tcpHeader->destination_port);
   
   print_ports(port);
   
   printf("\t\tSequence Number: %zu\n", ntohl(tcpHeader->sequence_number));
   printf("\t\tACK Number: %u\n", ntohl(tcpHeader->acknowledgment_number));
   printf("\t\tData Offset (bytes): %d\n", (tcpHeader->data_offset & 0xF0) >> 2);
   int flags = tcpHeader->flags;
   printf("\t\tSYN Flag: ");
   if (flags & (1 << 1)) {
      printf("Yes\n");
   }
   else {
      printf("No\n");
   }
   printf("\t\tRST Flag: ");
   if (flags & (1 << 2)) {
      printf("Yes\n");
   }
   else {
      printf("No\n");
   }
   printf("\t\tFIN Flag: ");
   if (flags & 1) {
      printf("Yes\n");
   }
   else {
      printf("No\n");
   }
   printf("\t\tACK Flag: ");
   if (flags & (1 << 4)) {
      printf("Yes\n");
   }
   else {
      printf("No\n");
   }
   
   printf("\t\tWindow Size: %d\n", ntohs(tcpHeader->window_size));
   
}

ARP_Header *parse_arp(const u_char *pkt_data) {
   struct ARP_Header *arpHeader = calloc(sizeof(ARP_Header), 1);
   char offset = SIZE_OF_ETHERNET_PACKET;
   
   memcpy(&(arpHeader->hardware_type), pkt_data + offset, ARPTypeLength);
   offset += ARPTypeLength;
   memcpy(&(arpHeader->protocol_type), pkt_data + offset, ARPTypeLength);
   offset += ARPTypeLength;
   memcpy(&(arpHeader->hardware_address_length), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(arpHeader->protocol_address_length), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(arpHeader->operation), pkt_data + offset, SIZE_OF_OPER);   
   offset += SIZE_OF_OPER;
   memcpy(&(arpHeader->sender_hardware_address), pkt_data + offset, sizeof(arpHeader->sender_hardware_address));   
   offset += sizeof(arpHeader->sender_hardware_address);
   memcpy(&(arpHeader->sender_protocol_address), pkt_data + offset, sizeof(arpHeader->sender_protocol_address));   
   offset += sizeof(arpHeader->sender_protocol_address);
   memcpy(&(arpHeader->target_hardware_address), pkt_data + offset, ARPAddressLength);   
   offset += ARPAddressLength;
   memcpy(&(arpHeader->target_protocol_address), pkt_data + offset, sizeof(arpHeader->sender_protocol_address));   
   
   if (!arpHeader) {
      exit(-1);
   }
   
   printf("\n\tARP header\n");
   printf("\t\tOpcode: ");
   
   if (arpHeader->operation[1] == ARP_REPLY) {
      printf("Reply\n");
   }
   else if (arpHeader->operation[1] == ARP_REQUEST) {
      printf("Request\n");
   }
   else {
      printf("Unknown arp opcode\n");
   }

   printf("\t\tSender MAC: %s\n", ether_ntoa((struct ether_addr *)arpHeader->sender_hardware_address));
   printf("\t\tSender IP: %s\n", inet_ntoa(arpHeader->sender_protocol_address));
   printf("\t\tTarget MAC: %s\n", ether_ntoa((struct ether_addr *)arpHeader->target_hardware_address));
   printf("\t\tTarget IP: %s\n", inet_ntoa(arpHeader->target_protocol_address));

   return arpHeader;
}

IP_Header *parse_ip(const u_char *pkt_data) {
   struct IP_Header *ipHeader = calloc(sizeof(IP_Header), 1);
   char offset = SIZE_OF_ETHERNET_PACKET;
   int ihl;
   int checksum = 0;
   int checksum1 = 0;

   
   memcpy(&(ipHeader->version), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(ipHeader->dscp), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(ipHeader->totalLength), pkt_data + offset, TWO_BYTES);
   offset += TWO_BYTES;
   memcpy(&(ipHeader->id), pkt_data + offset, IP_ID_LENGTH);
   offset += IP_ID_LENGTH;
   memcpy(&(ipHeader->flags), pkt_data + offset, TWO_BYTES);
   offset += TWO_BYTES;
   memcpy(&(ipHeader->timeToLive), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(ipHeader->protocol), pkt_data + offset, sizeof(char));
   offset += sizeof(char);
   memcpy(&(ipHeader->headerChecksum), pkt_data + offset, IP_HEADER_CHECKSUM_LENGTH);
   offset += IP_HEADER_CHECKSUM_LENGTH;
   memcpy(&(ipHeader->sourceIPAddress), pkt_data + offset, IP_ADDRESS_LENGTH);
   offset += IP_ADDRESS_LENGTH;
   memcpy(&(ipHeader->destinationIPAddress), pkt_data + offset, IP_ADDRESS_LENGTH);
   
   if (!ipHeader) {
      exit(-1);
   }
   ihl = ipHeader->version & IHL_MASK;

   printf("\n\tIP Header\n");
   printf("\t\tIP Version: %d\n", (ipHeader->version & 0xF0) >> 4);
   printf("\t\tHeader Len (bytes): %d\n", ihl * 4);
   printf("\t\tTOS subfields:\n");
   printf("\t\t   Diffserv bits: %d\n", (ipHeader->dscp & 0xFC) >> 2);
   printf("\t\t   ECN bits: %d\n", (ipHeader->dscp) & 0x03);    
   printf("\t\tTTL: %d\n", (ipHeader->timeToLive));
   
   printf("\t\tProtocol: ");
   
   if (ipHeader->protocol == UDP_PROTOCOL_CODE) {
      printf("UDP\n");
   }
   else if (ipHeader->protocol == TCP_PROTOCOL_CODE) {
      printf("TCP\n");
   }
   else if (ipHeader->protocol == ICMP_PROTOCOL_CODE) {
      printf("ICMP\n");
   }
   else {
      printf("Unknown\n");
   }
      
   checksum |= ipHeader->headerChecksum[1] & 0xFF;
   checksum1 |= (ipHeader->headerChecksum[0] & 0xFF);

   if (!in_cksum((unsigned short *)ipHeader, ihl * 4)) {
      printf("\t\tChecksum: Correct (0x%02x%02x)\n", checksum1, checksum);
   }
   else {
      printf("\t\tChecksum: Incorrect (0x%02x%02x)\n", checksum1, checksum);
   }
   printf("\t\tSender IP: %s\n", inet_ntoa(ipHeader->sourceIPAddress));
   printf("\t\tDest IP: %s\n", inet_ntoa(ipHeader->destinationIPAddress));
   if (ipHeader->protocol == UDP_PROTOCOL_CODE) {
      parse_udp(pkt_data, ihl);
   }
   else if (ipHeader->protocol == TCP_PROTOCOL_CODE) {
      parse_tcp(pkt_data, ihl, ipHeader);
   }
   else if (ipHeader->protocol == ICMP_PROTOCOL_CODE) {
      parse_icmp(pkt_data, ihl);
   }
   
   return ipHeader;
}

void parse_udp(const u_char *pkt_data, int ihl) {
   struct udp_header *udpHeader = calloc(sizeof(udpHeader), 1);
   char offset = SIZE_OF_ETHERNET_PACKET + ihl * 4;
   
   memcpy(&(udpHeader->source_port), pkt_data + offset, udpPortLength);
   offset += udpPortLength;
   memcpy(&(udpHeader->destination_port), pkt_data + offset, udpPortLength);
   offset += udpPortLength;
   memcpy(&(udpHeader->length), pkt_data + offset, udpLength);
   offset += udpLength;
   memcpy(&(udpHeader->checksum), pkt_data + offset, udpChecksumLength);
   offset += udpChecksumLength;

   printf("\tUDP Header\n");
   int port = ntohs(udpHeader->source_port);
   printf("\t\tSource Port:  ");
   print_ports(port);
   printf("\t\tDest Port:  ");
   
   port = ntohs(udpHeader->destination_port);
   print_ports(port);
   free(udpHeader);
}

void print_ports(int port) {
   
   if (port == HTTP_PORT) {
      printf("HTTP\n");
   }
   else if (port == TELNET_PORT) {
      printf("Telnet\n");   
   }
   else if (port == FTP_PORT) {
      printf("FTP\n");   
   }
   else if (port == POP3_PORT) {
      printf("POP3\n");   
   }
   else if (port == SMTP_PORT) {
      printf("SMTP\n");
   }
   else if (port == DNS_PORT) {
      printf("DNS\n");
   }
   else {
      printf("%d\n", port);
   }
}


Ethernet_Header *parse_ethernet_header(const u_char *pkt_data) {
   Ethernet_Header *ethernetHeader = (Ethernet_Header*)calloc(sizeof(Ethernet_Header), 1);
   memcpy(&(ethernetHeader->destination), pkt_data, ETHERNET_ADDRESS_LENGTH);
   memcpy(&(ethernetHeader->source), pkt_data + ETHERNET_ADDRESS_LENGTH, ETHERNET_ADDRESS_LENGTH);
   memcpy(&(ethernetHeader->EtherType), pkt_data + 2*ETHERNET_ADDRESS_LENGTH, EtherTypeLength);
   if (!ethernetHeader) {
      fprintf(stderr, "Error: Ethernet Packet\n");
      exit(ETHERNET_ERROR);
   }
   
   printf("\n\tEthernet Header\n");
   printf("\t\tDest MAC: %s\n", ether_ntoa((const struct ether_addr*)ethernetHeader->destination));
   printf("\t\tSource MAC: %s\n", ether_ntoa((const struct ether_addr*)ethernetHeader->source));
   return ethernetHeader;  
}
