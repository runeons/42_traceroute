#include "traceroute_functions.h"

static void    save_time(t_data *dt)
{
    t_probe *curr = NULL;

    if (gettimeofday(&dt->recv_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
    curr = get_probe(dt->hop_probes, dt->curr_probe);
    if (curr)
        curr->time = (dt->recv_tv.tv_sec - dt->send_tv.tv_sec) * 1000000 + dt->recv_tv.tv_usec - dt->send_tv.tv_usec;
}

static void     save_name(t_data *dt, char *host)
{
    t_probe *curr = NULL;

    curr = get_probe(dt->hop_probes, dt->curr_probe);
    if (curr)
    {
        curr->name = ft_strdup(host);
        if (curr->name == NULL)
            exit_error("traceroute: malloc failure.\n");
    }
}

static void     resolve_name(t_data *dt, struct sockaddr_in hop_addr)
{
    char    host[MAX_HOSTNAME_LEN];

    ft_bzero(host, MAX_HOSTNAME_LEN);
    if (getnameinfo((struct sockaddr*)&(hop_addr), sizeof(hop_addr), host, sizeof(host), NULL, 0, 0) != 0)
        exit_error("traceroute: address error: The hop name could not be resolved.\n");
    else
        save_name(dt, host);
}

static void    handle_reply(t_data *dt, char recv_packet[], struct sockaddr_in hop_addr)
{
    struct icmphdr *h = (struct icmphdr *)(recv_packet + H_IP_LEN);

    save_time(dt);
    resolve_name(dt, hop_addr);
    if (h->type == ICMP_TIME_EXCEEDED || h->type == ICMP_UNREACH)
        display_hop(dt);
    if (h->type == ICMP_UNREACH)
        g_loop = 0;
    // else
    //     if (g_loop)
    //         printf(C_B_RED"UNHANDLED %-4d %s"C_RES"\n", dt->curr_ttl, inet_ntoa(hop_addr.sin_addr)); // TO DO check 127 // WHEN do I go there multi stress test
}

int     is_same_port(t_data *dt, char *recv_packet)
{
    int  reply_port = 0;
    int  send_port  = 0;

    reply_port = get_reply_port(recv_packet);
    send_port = get_curr_send_port(dt);
    if (reply_port == send_port)
        return (1);
    return(0);
}

void    receive_reply(t_data *dt)
{
    char                recv_packet[PACKET_SIZE];
    socklen_t           hop_addr_len = sizeof(struct sockaddr_in);

    struct sockaddr_in  *addr = &((t_probe *)(ft_lst_get_last_node(&dt->hop_probes)->content))->address; // TO DO PROTECT
    if (recvfrom(dt->socket, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)addr, &hop_addr_len) == -1)
        exit_error_clear(dt, "Error receiving recv_packet %s %s\n", strerror(errno));
    verbose_full_reply(recv_packet);
    // printf(C_G_RED"[QUICK DEBUG] reply_port == send_port: %d"C_RES"\n", is_same_port(dt, recv_packet));
    handle_reply(dt, recv_packet, *addr);
}
