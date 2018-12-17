#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void die(char* s) { printf("%s\n", s); exit(EXIT_FAILURE); }

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("usage: %s host port filename packet-len sleep-ms\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in dest_addr;
    socklen_t dest_len = sizeof(dest_addr);
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    int status = inet_pton(AF_INET, argv[1], &(dest_addr.sin_addr));
    if (status != 1) die("invalid IP");
    if (isdigit(*argv[2])) dest_addr.sin_port = htons(atoi(argv[2]));
    else die("invalid port");

    size_t length, maxlength = atoi(argv[4]);
    if (maxlength < 32 || maxlength > 60000) die("packet length must be between 32 and 60000");
    char message[maxlength];

    int sleeptime = atoi(argv[5]);
    if (sleeptime < 0 || sleeptime > 5000) die("sleep time must be between 0 and 5000");

    FILE *file = fopen(argv[3], "r");
    if (file == NULL) die("file read failed");
    fseek(file, 0L, SEEK_END);
    long int filelen = ftell(file);
    fseek(file, 0L, SEEK_SET);

    int s_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s_fd < 0) die("create socket failed");

    size_t total = 0;
    while ((length = fread(message, 1, maxlength, file)) > 0) {
        ssize_t status = sendto(s_fd, message, length, 0, (struct sockaddr*)&dest_addr, dest_len);
        if (status < 0) die("send failed");

        nanosleep(&(struct timespec){ .tv_sec = sleeptime / 1000, .tv_nsec = 1000000L * (sleeptime % 1000) }, NULL);
        
        printf("sent %d/%d\n", total += length, filelen);
    }
    close(s_fd);
}