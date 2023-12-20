
#ifndef UTILS_CONSTANTS_H
# define UTILS_CONSTANTS_H

# define DEBUG                  0
# define VERBOSE                0
# define MAX_IP_LEN             16
# define MAX_HOSTNAME_LEN       128
# define PRINT_BUF_SIZE         512
# define MAX_PORT               65535

# define IP_H_LEN               20  // sizeof(struct iphdr)
# define ICMP_H_LEN             8   // sizeof(struct icmphdr)
# define UDP_H_LEN              8   // sizeof(struct udphdr)
# define UDP_D_LEN              32
# define UDP_DATA               "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\^_]"

# define IP_HL                  5
# define IP_VERSION             4

# define SEND_PACKET_SIZE       60
# define RECV_PACKET_SIZE       120

# define MAX_TTL                30      // option m
# define FIRST_TTL              1       // option f
# define NB_PROBES              1       // option q
# define REPLY_TIMEOUT          1       // option w
# define PROBES_INTERVAL_S      0       // option z
# define DST_PORT               33434   // option p


#endif
