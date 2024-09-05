#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "enum.h"

#define DNS_SERVICE_PORT 53
#define GOOGLE_DNS_SERVER_IP "8.8.8.8"

/*
DNSHeader.flags
                               1  1  1  1  1  1
 0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
|QR|   OPCODE  |AA|TC|RD|RA|   Z    |   RCODE   |
*/
struct DNSHeader {
    uint16_t id;
    // unsigned int qr: 1; // query (0) or response (1)?
    // unsigned int opcode: 4;
    // unsigned int aa: 1; // Authoritative Answer
    // unsigned int tc: 1; // Truncated - response was truncated
    // unsigned int rd: 1; // Recursion Desired
    // unsigned int ra: 1; // Recursion Available
    // unsigned int z: 3; // Not used, must be 0.
    // unsigned int rcode: 4; // Response Code
    uint16_t flags;
    uint16_t qdcount; // num entries in the questions section
    uint16_t ancount; // num resource records in the answer section
    uint16_t nscount; // num name server resource records in the authority records section
    uint16_t arcount; // num resource records in the additional records section
} __attribute__((packed));

struct DNSQuestion {
    uint8_t* qname;
    uint16_t qtype;
    uint16_t qclass;
} __attribute__((packed));

struct DNSAnswer {

};
struct DNSAuthority {

};
struct DNSAdditional {

};

struct DNS_msg {
    struct DNSHeader *header;
    struct DNSQuestion *question;
} __attribute__((packed));

void validate_alloc(void *mem_ptr);
void init_dns_msg(struct DNS_msg *msg);
void init_header(struct DNSHeader *header);
void init_question(struct DNSQuestion *question, char *hostname, size_t *len_encoded_hostname);
void encode_hostname(char *hostname, char *en_hostname_out);
uint8_t *pack_payload(struct DNS_msg *msg, char *hostname, size_t *payload_size_out);
int send_dns_msg(uint8_t *payload, size_t payload_size);
int main(int argc, char *argv[]);