#include "traceroute_functions.h"

void    display_hop(t_data *dt, struct sockaddr_in hop_addr)
{
    if (g_loop)
        printf("%-4d %s\n", dt->curr_ttl, inet_ntoa(hop_addr.sin_addr));
}

void    display_hop_timeout(t_data *dt)
{
    if (g_loop)
        printf("%-4d *\n", dt->curr_ttl);
}

void    craft_ip_header(t_data *dt, struct ip *ip_h)
{
    ip_h->ip_hl         = 5;
    ip_h->ip_v          = 4;
    ip_h->ip_ttl        = dt->curr_ttl;
    ip_h->ip_p          = IPPROTO_UDP;
    ip_h->ip_src.s_addr = INADDR_ANY;
    ip_h->ip_dst        = dt->target_address.sin_addr;
}

void    craft_udp_header(t_data *dt, struct udphdr *udp_h)
{
    udp_h->uh_sport = htons(dt->src_port);
    udp_h->uh_dport = htons(dt->dst_port + dt->curr_ttl);
    udp_h->uh_ulen = htons(sizeof(struct udphdr));
}

void    send_packet(t_data *dt, void *packet)
{
    int r = 0;

    r = sendto(dt->socket, packet, sizeof(struct ip) + sizeof(struct udphdr), 0, (struct sockaddr *) &dt->target_address, sizeof(struct sockaddr_in));
    if (r == -1)
        exit_error_clear(dt, "Error sending packet %s\n", strerror(errno));
    if (r == 0)
        printf(C_B_RED"Not entirely sent"C_RES"\n");
    verbose_full_send(packet);
}

void    init_fd_set(t_data *dt)
{
    FD_ZERO(&dt->read_set);
    FD_SET(dt->socket, &dt->read_set);
}

void    handle_reply(t_data *dt, char recv_packet[], struct sockaddr_in hop_addr)
{
    struct icmphdr *h = (struct icmphdr *)(recv_packet + H_IP_LEN);

    if (h->type == ICMP_TIME_EXCEEDED)
        display_hop(dt, hop_addr);
    else if (h->type == ICMP_UNREACH)
    {
        display_hop(dt, hop_addr);
        g_loop = 0;
    }
    else
        if (g_loop)
            printf(C_B_RED"UNHANDLED %-4d %s"C_RES"\n", dt->curr_ttl, inet_ntoa(hop_addr.sin_addr)); // TO DO check 127
}

void    init_timeout(t_data *dt, struct timeval *timeout)
{
    timeout->tv_sec  = dt->reply_timeout;
    timeout->tv_usec = 0;
}

void    receive_responses(t_data *dt)
{
    struct timeval      timeout;
    struct sockaddr_in  hop_addr;
    socklen_t           hop_addr_len = sizeof(hop_addr);
    char                recv_packet[PACKET_SIZE];
    int                 fds = 0;
    int                 r   = 0;

    init_fd_set(dt);
    init_timeout(dt, &timeout);
    fds = select(dt->socket + 1, &dt->read_set, NULL, NULL, &timeout);
    if (fds > 0)
    {
        r = recvfrom(dt->socket, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *) &hop_addr, &hop_addr_len);
        if (r == -1)
            exit_error_clear(dt, "Error receiving recv_packet %s %s\n", strerror(errno));
        verbose_full_reply(recv_packet);
        handle_reply(dt, recv_packet, hop_addr);
    }
    else
        display_hop_timeout(dt);
}

void    reach_hop(t_data *dt, int ttl)
{
    char            sent_packet[PACKET_SIZE];
    struct ip       *ip_h  = (struct ip *)sent_packet;
    struct udphdr   *udp_h = (struct udphdr *)(sent_packet + sizeof(struct ip));

    dt->curr_ttl = ttl;
    craft_ip_header(dt, ip_h);
    craft_udp_header(dt, udp_h);
    send_packet(dt, sent_packet);
    receive_responses(dt);

}