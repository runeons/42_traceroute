
#ifndef UTILS_CONSTANTS_H
# define UTILS_CONSTANTS_H

# define DEBUG 1
# define VERBOSE 0

# define MAX_INT                2147483647
# define MIN_INT                -2147483648
# define HEX_BASE_U             "0123456789ABCDEF"
# define HEX_BASE_L             "0123456789abcdef"
# define MAX_IP_LEN             16
# define MAX_HOSTNAME_LEN       128

# define MAX_TTL               16
# define NB_PROBES             3
# define REPLY_TIMEOUT         5

# define UDP_H_LEN             8
# define UDP_D_LEN             32
# define UDP_PAYLOAD           "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\^_]"
# define ICMP_HEADER_LEN        8
# define ICMP_TIMESTAMP_LEN     8
# define ICMP_DATA_LEN          40
# define ICMP_PAYLOAD_LEN       48

// ECHO QUERY MESSAGES
# define ICMP_ECHO_REQUEST      8
# define ICMP_ECHO_REPLY        0

// ERROR-REPORTING MESSAGES
# define ICMP_ERR_UNREACHABLE   3
# define ICMP_ERR_TIME_EXCEEDED 11

#endif
