#ifndef TRACEROUTE_DATA_H
# define TRACEROUTE_DATA_H

# include "utils_constants.h"

// struct icmphdr
// {
//     unsigned char type;                /* message type */
//     unsigned char code;                /* type sub-code */
//     unsigned short int checksum;
//     union
//     {
//         struct
//         {
//             unsigned short int        id;
//             unsigned short int        sequence;
//         } echo;                        /* echo datagram */
//         unsigned int        gateway;        /* gateway address */
//         struct
//         {
//             unsigned short int        __unused;
//             unsigned short int        mtu;
//         } frag;                        /* path mtu discovery */
//     } un;
// };

// struct udphdr
// {
//   u_int16_t uh_sport;                /* source port */
//   u_int16_t uh_dport;                /* destination port */
//   u_int16_t uh_ulen;                /* udp length */
//   u_int16_t uh_sum;                /* udp checksum */
// };

extern int g_loop;

typedef struct  s_packet
{
    struct iphdr    *ip;
    struct icmphdr  *icmp;
    char            *payload;
}               t_packet;

typedef struct  s_icmp
{
	struct icmphdr  h;
	char            timestamp[ICMP_TIMESTAMP_LEN];
	char            payload[ICMP_PAYLOAD_LEN];
}               t_icmp;

typedef struct  s_udp
{
	struct udphdr   h;
	char            payload[UDP_D_LEN];
}               t_udp;

typedef struct  s_data
{
    // general
    char                *input_dest;
    t_lst               *act_options;
    char                *resolved_address;
    char                *resolved_hostname;
    int                 socket_udp;
    int                 socket_raw;
    struct sockaddr_in  address;
    int                 max_ttl;
    int                 nb_probes;
    int                 reply_timeout;

    // each hop
    int                 curr_ttl;
    t_udp               udp_packet;
    t_icmp              icmp_packet;
    int                 dest_port;
    int                 src_port;
}					t_data;

#endif

