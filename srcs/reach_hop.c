#include "traceroute_functions.h"


void    craft_ip_header(t_data *dt, struct ip *ip_h)
{
    ip_h->ip_hl         = IP_HL;
    ip_h->ip_v          = IP_VERSION;
    ip_h->ip_ttl        = dt->curr_ttl;
    ip_h->ip_p          = IPPROTO_UDP;
    ip_h->ip_src.s_addr = INADDR_ANY;
    ip_h->ip_dst        = dt->target_address.sin_addr;
}

void    craft_udp_header(t_data *dt, struct udphdr *udp_h)
{
    dt->dst_port = (dt->dst_port + 1) % MAX_PORT;
    if (dt->dst_port == 0)
        dt->dst_port++;
    udp_h->uh_sport = htons(dt->src_port);
    udp_h->uh_dport = htons(dt->dst_port);
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
    if (gettimeofday(&dt->send_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
    verbose_full_send(packet);
}

void    init_fd_set(t_data *dt)
{
    FD_ZERO(&dt->read_set);
    FD_SET(dt->socket, &dt->read_set);
}

static void    save_time(t_data *dt)
{
    int *time;

    if (gettimeofday(&dt->recv_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
    if (!(time = mmalloc(sizeof(int))))
        exit_error_close(dt->socket, "traceroute: malloc failure.");
    *time = (dt->recv_tv.tv_sec - dt->send_tv.tv_sec) * 1000000 + dt->recv_tv.tv_usec - dt->send_tv.tv_usec;
    ft_lst_add_node_back(&dt->hop_times, ft_lst_create_node(time));
}

void    handle_reply(t_data *dt, char recv_packet[], struct sockaddr_in hop_addr)
{
    struct icmphdr *h = (struct icmphdr *)(recv_packet + H_IP_LEN);

    save_time(dt);
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

void    receive_response(t_data *dt)
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

void    reach_hop(t_data *dt)
{
    char            sent_packet[PACKET_SIZE];
    struct ip       *ip_h  = (struct ip *)sent_packet;
    struct udphdr   *udp_h = (struct udphdr *)(sent_packet + sizeof(struct ip));

    ft_del((void **)&dt->hop_times);
    for (int i = 1; i <= dt->nb_probes; i++)
    {
        dt->curr_probe = i;
        craft_ip_header(dt, ip_h);
        craft_udp_header(dt, udp_h);
        send_packet(dt, sent_packet);
        receive_response(dt);
        usleep(dt->probes_interval_us);
    }
    dt->curr_ttl++;
}