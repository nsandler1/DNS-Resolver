#include "main.h"

void print_bytes(uint8_t* bytes, size_t size) {
    for (uint8_t i=0; i < size; i++) {
        printf("%02x", bytes[i]);
    }

    printf("\n");
}

void validate_alloc(void *mem_ptr) {
	if (mem_ptr == NULL) {
        printf("Failed to allocated memory");
        exit(0);
	}
}

void init_dns_msg(struct DNS_msg *msg) {
    msg->header = (struct DNSHeader *)calloc(1, sizeof(struct DNSHeader));
    validate_alloc(msg->header);

    msg->question = (struct DNSQuestion *)calloc(1, sizeof(struct DNSQuestion));
    validate_alloc(msg->question);
}

void init_header(struct DNSHeader *header) {
    header->id = htons(22); //(uint16_t)(rand() % 100); // random number b/w 0 and 99, inclusive
    header->flags = htons(0b0000000100000000);
    header->qdcount = htons(1);
}

void encode_hostname(char *hostname, char *en_hostname_out) {
    // Make deep copy of hostname to use with strtok()
    const size_t len_hostname = strlen(hostname) + 1; // +1 for null byte
    char hostname_cpy[len_hostname];
    strncpy(hostname_cpy, hostname, len_hostname);

    char *token = strtok(hostname_cpy, ".");
    size_t len_token;
    int pos = 0;

    printf("hostname: %s\nencoded: ", hostname);

    while (token) {
        len_token = strlen(token);
        snprintf(&en_hostname_out[pos], sizeof(uint16_t), "%c", (int)len_token);
        strncpy(&en_hostname_out[pos + 1], token, len_token);
        printf("%lu%s", len_token, &en_hostname_out[pos + 1]);
        pos += len_token + 1; // move pos to start of where next token will go
        token = strtok(NULL, ".");
    }

    printf("0\n");
    en_hostname_out[pos] = 0;
}

void init_question(struct DNSQuestion *question, char *hostname, size_t *len_encoded_hostname_out) {
    *len_encoded_hostname_out = strlen(hostname) + 2; // +2 for leading/trailing token counts
    question->qname = (uint8_t *)calloc(*len_encoded_hostname_out, sizeof(uint8_t));
    validate_alloc(question->qname);

    encode_hostname(hostname, (char *)question->qname);

    question->qtype = htons(QT_A);
    question->qclass = htons(QC_IN);
}

uint8_t *pack_payload(struct DNS_msg *msg, char *hostname, size_t *payload_size_out) {
    size_t len_encoded_hostname;
    const size_t sizeof_header = sizeof(struct DNSHeader);

    init_header(msg->header);
    init_question(msg->question, hostname, &len_encoded_hostname);

    const size_t sizeof_question = (sizeof(uint16_t) * 2) + len_encoded_hostname;
    const size_t payload_size = sizeof_header + sizeof_question;
    uint8_t* payload = (uint8_t *)calloc(payload_size, sizeof(uint8_t));
    validate_alloc(payload);

    uint8_t *fill_pos = payload;
    memcpy(fill_pos, msg->header, sizeof_header);

    fill_pos += sizeof_header;
    memcpy(fill_pos, msg->question->qname, len_encoded_hostname);

    fill_pos += len_encoded_hostname;
    memcpy(fill_pos, &(msg->question->qtype), sizeof(uint16_t));

    fill_pos += sizeof(uint16_t);
    memcpy(fill_pos, &(msg->question->qclass), sizeof(uint16_t));

    *payload_size_out = payload_size;
    return payload;
}

int send_dns_msg(uint8_t *payload, size_t payload_size) {
    struct sockaddr_in serv_addr;
    int status, sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DNS_SERVICE_PORT);

     if (inet_pton(AF_INET, GOOGLE_DNS_SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        exit(1);
    }

    if ((status = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed\n");
        exit(1);
    }

    send(sockfd, payload, payload_size, 0);

    free(payload);
    payload = NULL;

    return sockfd;
}

void recv_dns_msg(int sockfd, struct DNS_msg *msg) {
    const size_t len = 12;
    uint8_t temp[len] = {0};
    recvfrom(sockfd, temp, len, MSG_PEEK, NULL, NULL);
    for (int i=0; i < len; i++) {
        printf("%02x ", temp[i]);
    }
    printf("\n");

    recvfrom(sockfd, msg->header, sizeof(struct DNSHeader), MSG_WAITALL, NULL, NULL);
    printf("ID: %d\nFLAGS: %d\n#q: %d\n#a: %d\n#servers: %d\n#additional: %d\n", ntohs(msg->header->id), ntohs(msg->header->flags), ntohs(msg->header->qdcount), ntohs(msg->header->ancount), ntohs(msg->header->nscount), ntohs(msg->header->arcount));

    uint8_t sizeof_domain_0[2] = {0};
    size_t len2 = recvfrom(sockfd, sizeof_domain_0, sizeof(uint8_t) * 2, MSG_WAITALL, NULL, NULL);
    printf("len: %lu\n", len2);
    for (int i=0; i < len2; i++) {
        printf("%02x ", sizeof_domain_0[i]);
    }

    close(sockfd);

    // const size_t size = 32;
    // uint8_t buff[size] = {0};
    // recvfrom(sockfd, buff, size, MSG_WAITALL, NULL, NULL);

    // print_bytes(buff, size);
}

void cleanup(struct DNS_msg *msg) {
    free(msg->question->qname);
    msg->question->qname = NULL;

    free(msg->header);
    msg->header = NULL;

    free(msg->question);
    msg->question = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments. Expected 1, recieved %d\n", argc);
        return 0;
    }

    srand(time(NULL));
    struct DNS_msg msg;
    size_t payload_size;
    uint8_t *payload;
    int sockfd;

    init_dns_msg(&msg);
    payload = pack_payload(&msg, argv[1], &payload_size);
    sockfd = send_dns_msg(payload, payload_size);
    bzero(msg.header, sizeof(struct DNSHeader));
    bzero(msg.question, sizeof(struct DNSQuestion));
    recv_dns_msg(sockfd, &msg);
    close(sockfd);
    //cleanup(&msg);

    return 0;
}