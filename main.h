#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

struct DNSHeader {
    uint16_t id;
    unsigned int qr: 1; // query (0) or response (1)?
    unsigned int opcode: 4;
    unsigned int aa: 1; // Authoritative Answer
    unsigned int tc: 1; // Truncated - response was truncated
    unsigned int rd: 1; // Recursion Desired
    unsigned int ra: 1; // Recursion Available
    unsigned int z: 3; // Not used, must be 0.
    unsigned int rcode: 3; // Response Code
    uint16_t qdcount; // num entries in the questions section
    uint16_t ancount; // num resource records in the answer section
    uint16_t nscount; // num name server resource records in the authority records section
    uint16_t arcount; // num resource records in the additional records section
};

struct DNSQuestion {
    uint8_t* qname;
    uint16_t qtype;
    uint16_t qclass;
};

struct DNSAnswer {

};
struct DNSAuthority {

};
struct DNSAdditional {

};


struct DNS_msg {
    struct DNSHeader header;
    struct DNSQuestion question;
};

void validate_alloc(void *mem_ptr);
void init_header(struct DNSHeader *header);
void init_question(struct DNSQuestion *question, char *hostname);
char *encode_hostname(char *hostname);
int main(int argc, char *argv[]);