// necessary inclusions for the program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// define the necessary constants
#define NTP_SERVER "44.31.46.123"
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_TIMESTAMP_DELTA 2208988800ull

// define the structure for the NTP packet
typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint32_t ref_ts_sec;
    uint32_t ref_ts_frac;
    uint32_t orig_ts_sec;
    uint32_t orig_ts_frac;
    uint32_t recv_ts_sec;
    uint32_t recv_ts_frac;
    uint32_t trans_ts_sec;
    uint32_t trans_ts_frac;
} ntp_packet;

// function to handle errors
void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// main function
int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    ntp_packet packet;
    time_t current_time;
    struct tm *local_time;

    // get local time
    current_time = time(NULL);
    local_time = localtime(&current_time);

    // print local time
    printf("Local time: %s", asctime(local_time));

    // get NTP time
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
        error("ERROR opening socket");

    // set NTP server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(NTP_PORT);
    if (inet_pton(AF_INET, NTP_SERVER, &serv_addr.sin_addr) <= 0)
        error("ERROR invalid address");

    // set NTP packet
    memset(&packet, 0, sizeof(packet));
    packet.li_vn_mode = (0x03 << 6) | (0x03 << 3) | 0x03;

    // send NTP packet
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR sending packet");

    // receive NTP packet
    n = recv(sockfd, &packet, sizeof(packet), 0);
    if (n < 0)
        error("ERROR receiving response");

    // close socket
    close(sockfd);

    // calculate NTP time
    current_time = (ntohl(packet.trans_ts_sec) - NTP_TIMESTAMP_DELTA);
    printf("NTP time: %s", ctime(&current_time));

    // calculate time difference
    time_t local_time_sec = mktime(local_time);
    double time_diff = difftime(current_time, local_time_sec);
    printf("Time difference: %.2f seconds\n", time_diff);

    return 0;
}
