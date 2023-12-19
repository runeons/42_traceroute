#include "traceroute_functions.h"

static void    save_time(t_data *dt)
{
    int     *time;
    t_lst   *last = NULL;

    if (gettimeofday(&dt->recv_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
    if (!(time = mmalloc(sizeof(int))))
        exit_error_close(dt->socket, "traceroute: malloc failure.");
    *time = (dt->recv_tv.tv_sec - dt->send_tv.tv_sec) * 1000000 + dt->recv_tv.tv_usec - dt->send_tv.tv_usec;
    ft_lst_add_node_back(&dt->hop_times, ft_lst_create_node(time));
    last = ft_lst_get_last_node(&dt->hop_probes);
    if (last && last->content)
        ((t_probe *)last->content)->time = *time;
}

static void     save_name(t_data *dt, struct sockaddr_in hop_addr)
{
    char    host[MAX_HOSTNAME_LEN];
    int     r = 0;
    // char    *name = NULL;
    t_lst   *last = NULL;


    ft_bzero(host, MAX_HOSTNAME_LEN);
    r = getnameinfo((struct sockaddr*)&(hop_addr), sizeof(hop_addr), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("traceroute: address error: The hop name could not be resolved. %d\n", r);
    else
    {
        last = ft_lst_get_last_node(&dt->hop_probes);
        if (last && last->content)
        {
            ((t_probe *)last->content)->name = ft_strdup(host);
            if (((t_probe *)last->content)->name == NULL)
                exit_error("traceroute: malloc failure.\n");
        }
    }
}
static void    handle_reply(t_data *dt, char recv_packet[], struct sockaddr_in hop_addr)
{
    struct icmphdr *h = (struct icmphdr *)(recv_packet + H_IP_LEN);

    save_time(dt);
    save_name(dt, hop_addr);
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
    socklen_t           hop_addr_len = sizeof(struct sockaddr_in);
    char                recv_packet[PACKET_SIZE];
    int                 r = 0;

    struct sockaddr_in  *addr = &((t_probe *)(ft_lst_get_last_node(&dt->hop_probes)->content))->address; // TO DO PROTECT
    r = recvfrom(dt->socket, recv_packet, sizeof(recv_packet), 0, (struct sockaddr *)addr, &hop_addr_len);
    if (r == -1)
        exit_error_clear(dt, "Error receiving recv_packet %s %s\n", strerror(errno));
    verbose_full_reply(recv_packet);
    handle_reply(dt, recv_packet, *addr);
}
