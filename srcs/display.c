#include "traceroute_functions.h"

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl, PACKET_SIZE);
    if (gettimeofday(&dt->init_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
}

void    display_hop(t_data *dt, struct sockaddr_in hop_addr)
{
    int curr_probe_time = *(int *)(ft_lst_get_last_node(&dt->hop_times)->content);
    if (dt->curr_probe == 1)
        printf("%-4d  ", dt->curr_ttl);
    if (ft_lst_size(dt->hop_times) == 1)
        printf("%s  ", inet_ntoa(hop_addr.sin_addr));
    printf("%.3f ms  ", (float)curr_probe_time / 1000);
    if (dt->nb_probes > 1)
        fflush(stdout);
    if (dt->curr_probe == dt->nb_probes)
        printf("\n");
}

void    display_hop_timeout(t_data *dt)
{
    if (dt->curr_probe == 1)
        printf("%-4d  ", dt->curr_ttl);
    printf("*  ");
    if (dt->nb_probes > 1)
        fflush(stdout);
    if (dt->curr_probe == dt->nb_probes)
        printf("\n");
}