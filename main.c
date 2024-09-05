#include "main.h"

void validate_alloc(void *mem_ptr) {
	if (mem_ptr == NULL) {
        printf("Failed to allocated memory");
        exit(0);
	}
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

    while (token) {
        len_token = strlen(token);
        snprintf(&en_hostname_out[pos], sizeof(uint16_t), "%lu", len_token);
        strncpy(&en_hostname_out[pos + 1], token, len_token + 1);
        pos += len_token + 1; // move pos to start of where next token will go
        token = strtok(NULL, ".");
    }

    sprintf(&en_hostname_out[pos], "%d", 0);
}

void init_question(struct DNSQuestion *question, char *hostname, size_t *len_encoded_hostname_out) {
    *len_encoded_hostname_out = strlen(hostname) + 2; // +2 for leading/trailing token counts
    question->qname = (uint8_t *)calloc(*len_encoded_hostname_out, sizeof(uint8_t));
    validate_alloc(question->qname);

    encode_hostname(hostname, (char *)question->qname);

    question->qtype = htons(QT_A);
    question->qclass = htons(QC_IN);
}

uint8_t *pack_payload(struct DNS_msg *msg, char *hostname) {
    size_t len_encoded_hostname;
    init_header(msg->header);
    init_question(msg->question, hostname, &len_encoded_hostname);

    printf("hostname: %s\nencoded: %s\n", hostname, msg->question->qname);

    const size_t payload_size = sizeof(msg->header) + (sizeof(uint16_t) * 2) + len_encoded_hostname; // -1 for question.qname pointer
    uint8_t* payload = (uint8_t *)calloc(payload_size, sizeof(uint8_t));
    validate_alloc(payload);

    uint8_t *fill_pos = payload;
    memcpy(fill_pos, msg->header, sizeof(*(msg->header)));

    fill_pos += sizeof(msg->header);
    memcpy(fill_pos, msg->question->qname, len_encoded_hostname);

    fill_pos += len_encoded_hostname;
    memcpy(fill_pos, &(msg->question->qtype), sizeof(uint16_t));

    fill_pos += sizeof(uint16_t);
    memcpy(fill_pos, &(msg->question->qclass), sizeof(uint16_t));

    free(msg->question->qname);
    msg->question->qname = NULL;

    return payload;
}

void send_dns_msg(char *hostname) {
    struct DNS_msg msg;
    msg.header = (struct DNSHeader *)calloc(1, sizeof(struct DNSHeader));
    validate_alloc(msg.header);

    msg.question = (struct DNSQuestion *)calloc(1, sizeof(struct DNSQuestion));
    validate_alloc(msg.question);

    uint8_t *payload = pack_payload(&msg, hostname);

    free(payload);
    payload = NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments. Expected 1, recieved %d\n", argc);
        return 0;
    }

    srand(time(NULL));
    send_dns_msg(argv[1]);

    return 0;
}