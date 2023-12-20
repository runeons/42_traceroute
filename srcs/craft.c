#include "traceroute_functions.h"

static void    craft_ip_header(t_data *dt, struct ip *ip_h)
{
    ip_h->ip_hl         = IP_HL;
    ip_h->ip_v          = IP_VERSION;
    ip_h->ip_ttl        = dt->curr_ttl;
    ip_h->ip_p          = IPPROTO_UDP;
    ip_h->ip_src.s_addr = INADDR_ANY;
    ip_h->ip_dst        = dt->target_address.sin_addr;
}

static void    craft_udp_header(t_data *dt, struct udphdr *udp_h)
{
    dt->dst_port = (dt->dst_port + 1) % MAX_PORT;
    if (dt->dst_port == 0)
        dt->dst_port++;
    udp_h->uh_sport = htons(dt->src_port);
    udp_h->uh_dport = htons(dt->dst_port);
    udp_h->uh_ulen  = htons(UDP_H_LEN + UDP_D_LEN);
}

static void    craft_udp_data(t_data *dt, char **udp_data)
{
    (void)dt;
    for (int i = 0; i < UDP_D_LEN; i++)
        (*udp_data)[i] = UDP_DATA[i];
}

void craft_packet(t_data *dt, char *udp_packet)
{
    struct ip       *ip_h  = (struct ip *)udp_packet;
    struct udphdr   *udp_h = (struct udphdr *)(udp_packet + IP_H_LEN);
    char            *udp_d = (char *)(udp_packet + IP_H_LEN + UDP_H_LEN);

    craft_ip_header(dt, ip_h);
    craft_udp_header(dt, udp_h);
    craft_udp_data(dt, &udp_d);
}
