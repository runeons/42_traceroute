#include "traceroute_functions.h"

void    reinit_hop(t_data *dt)
{
    ft_del((void **)&dt->hop_times);
    dt->curr_probe = 1;
}

void    init_fd_set(t_data *dt)
{
    FD_ZERO(&dt->read_set);
    FD_SET(dt->socket, &dt->read_set);
}

void    init_timeout(t_data *dt, struct timeval *timeout)
{
    timeout->tv_sec  = dt->reply_timeout;
    timeout->tv_usec = 0;
}

void    monitor_reply(t_data *dt)
{
    struct timeval      timeout;
    int                 fds = 0;

    init_fd_set(dt);
    init_timeout(dt, &timeout);
    fds = select(dt->socket + 1, &dt->read_set, NULL, NULL, &timeout);
    if (fds > 0)
        receive_reply(dt);
    else
        display_hop_timeout(dt);
}

void    send_packet(t_data *dt, void *packet)
{
    int r = 0;

    r = sendto(dt->socket, packet, sizeof(struct ip) + sizeof(struct udphdr), 0, (struct sockaddr *) &dt->target_address, sizeof(struct sockaddr_in));
    if (r == -1)
        exit_error_clear(dt, "Error sending packet %s\n", strerror(errno));
    else if (r == 0)
        warning_error("Not entirely sent\n");
    if (gettimeofday(&dt->send_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
    verbose_full_send(packet);
}

void    reach_hop(t_data *dt)
{
    char    udp_packet[PACKET_SIZE];

    reinit_hop(dt);
    while (dt->curr_probe <= dt->nb_probes)
    {
        craft_packet(dt, udp_packet);
        send_packet(dt, udp_packet);
        monitor_reply(dt);
        usleep(dt->probes_interval_us);
        dt->curr_probe++;
    }
    dt->curr_ttl++;
}