#include "main.h"

void validate_alloc(void *mem_ptr) {
	if (mem_ptr == NULL) {
        printf("Failed to allocated memory");
        exit(0);
	}
}

void init_header(struct DNSHeader *header) {
    bzero((void *)header, sizeof(*header));
    header->id = (uint16_t)(rand() % 100); // random int b/w 0 and 99, inclusive
    header->rd = 1;
    header->qdcount = 1;
}

char *encode_hostname(char *hostname) {
    const size_t len_hostname = strlen(hostname);
    char *res = (char *)calloc(len_hostname + 2, sizeof(char));
    char *token = strtok(hostname, ".");
    size_t len_token;
    int pos = 0;
    while (token) {
        len_token = strlen(token);
        sprintf(&res[pos], "%d", (int)len_token);
        strncpy(&res[pos + 1], token, len_token);
        pos += len_token + 1; // move pos to start of where next token will go
        token = strtok(NULL, ".");
    }

    sprintf(&res[pos - 1], "%d", 0);

    return res;
}

void init_question(struct DNSQuestion *question, char *hostname) {
    question->qname = (uint8_t *)encode_hostname(hostname);
    question->qtype = 1;
    question->qclass = 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments. Expected 1, recieved %d", argc);
        return 0;
    }

    srand(time(NULL));
    return 0;
}