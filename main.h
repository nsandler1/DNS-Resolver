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
    uint16_t flags;
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

//                                 1  1  1  1  1  1
//   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                                               |
// /                                               /
// /                      NAME                     /
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      TYPE                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                     CLASS                     |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      TTL                      |
// |                                               |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                   RDLENGTH                    |
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
// /                     RDATA                     /
// /                                               /
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
struct DNSResourceRecord {
    uint8_t *domainName; // The domain name to which this resource record pertains.
    uint16_t type; // Contains one of the RR type codes. This field specifies the meaning of the data in the RDATA field.
    uint16_t class; //Specifies the class of the data in the RDATA field.
    uint32_t ttl; /* specifies the time interval (in seconds) that the
                     resource record may be cached before it should be
                     discarded.  Zero values are interpreted to mean that the
                     RR can only be used for the transaction in progress, and
                     should not be cached. */
    uint16_t rdlength; // Specifies the length in octets of the RDATA field.
    uint8_t *rdata; /* Description of the resource. The format of this
                       information varies according to the TYPE and CLASS of
                       the resource record. For example, the if the TYPE is A
                       and the CLASS is IN, the RDATA field is a 4 octet ARPA
                       Internet address. */

};

struct DNS_msg {
    struct DNSHeader *header;
    struct DNSQuestion *question;
};

void print_bytes(uint8_t* bytes, size_t size);
void validate_alloc(void *mem_ptr);
void init_dns_msg(struct DNS_msg *msg);
void init_header(struct DNSHeader *header);
void init_question(struct DNSQuestion *question, char *hostname, size_t *len_encoded_hostname);
void encode_hostname(char *hostname, char *en_hostname_out);
uint8_t *pack_payload(struct DNS_msg *msg, char *hostname, size_t *payload_size_out);
int send_dns_msg(uint8_t *payload, size_t payload_size);
void recv_dns_msg(int sockfd, struct DNS_msg *msg);
void cleanup(struct DNS_msg *msg);
int main(int argc, char *argv[]);