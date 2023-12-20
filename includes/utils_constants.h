
#ifndef UTILS_CONSTANTS_H
# define UTILS_CONSTANTS_H

# define DEBUG                  0
# define VERBOSE                1
# define PRINT_BUF_SIZE         512
# define MAX_IP_LEN             16
# define MAX_HOSTNAME_LEN       128
# define MAX_PORT               65535
# define DST_PORT               33434

# define IP_H_LEN               20  // sizeof(struct iphdr)
# define ICMP_H_LEN             8   // sizeof(struct icmphdr)

# define UDP_H_LEN              8   // sizeof(struct udphdr)
# define UDP_D_LEN              32
# define UDP_DATA               "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\^_]"

# define IP_HL                  5
# define IP_VERSION             4

# define SEND_PACKET_SIZE       60
# define RECV_PACKET_SIZE       120

# define MAX_TTL                30
# define FIRST_TTL              1
# define NB_PROBES              3
# define REPLY_TIMEOUT          2
# define PROBES_INTERVAL_S      0


#endif
