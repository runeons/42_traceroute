#include "traceroute_functions.h"

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

static void resolve_hop_name(t_data *dt, struct sockaddr_in hop_addr)
{
    char    host[MAX_HOSTNAME_LEN];
    int r = 0;

    ft_bzero(host, MAX_HOSTNAME_LEN);
    // if (inet_pton(AF_INET, dt->resolved_address, &(hop_addr.sin_addr)) <= 0)
    //     exit_error("traceroute: address error: Invalid IPv4 address.\n");
    r = getnameinfo((struct sockaddr*)&(hop_addr), sizeof(hop_addr), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("traceroute: address error: The hop name could not be resolved. %d\n", r);
    else
    {
        dt->hop_name = ft_strdup(host);
        // printf(C_G_RED"[QUICK DEBUG] dt->hop_name: %s"C_RES"\n", dt->hop_name);
        if (dt->hop_name == NULL)
            exit_error("traceroute: malloc failure.\n");
    }
    // if (DEBUG)
    //     printf(C_B_RED"dt->hop_name %s"C_RES"\n", dt->hop_name);
}

static void    handle_reply(t_data *dt, char recv_packet[], struct sockaddr_in hop_addr)
{
    struct icmphdr *h = (struct icmphdr *)(recv_packet + H_IP_LEN);

    save_time(dt);
    resolve_hop_name(dt, hop_addr);
    if (h->type == ICMP_TIME_EXCEEDED || h->type == ICMP_UNREACH)
        display_hop(dt, hop_addr);
    if (h->type == ICMP_UNREACH)
        g_loop = 0;
    // else
    //     if (g_loop)
    //         printf(C_B_RED"UNHANDLED %-4d %s"C_RES"\n", dt->curr_ttl, inet_ntoa(hop_addr.sin_addr)); // TO DO check 127 // WHEN do I go there multi stress test
}

void    receive_reply(t_data *dt)
{
    struct sockaddr_in  hop_addr;
    socklen_t           hop_addr_len = sizeof(hop_addr);
    char                recv_packet[PACKET_SIZE];
    int                 r = 0;

    r = recvfrom(dt->socket, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *) &hop_addr, &hop_addr_len);
    if (r == -1)
        exit_error_clear(dt, "Error receiving recv_packet %s %s\n", strerror(errno));
    verbose_full_reply(recv_packet);
    handle_reply(dt, recv_packet, hop_addr);
}
