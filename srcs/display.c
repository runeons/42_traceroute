#include "traceroute_functions.h"

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl, PACKET_SIZE);
    if (gettimeofday(&dt->init_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
}

static char    *hop_information(t_data *dt, char *buf, t_probe *curr_probe)
{
    ft_bzero(buf, 512);
    if (is_activated_option(dt->act_options, 'n'))
        sprintf(buf, "%s", inet_ntoa(curr_probe->address.sin_addr));
    else
        sprintf(buf, "%s (%s)", curr_probe->name, inet_ntoa(curr_probe->address.sin_addr));
    return (buf);
}

int     is_new_address(t_data *dt, t_probe *curr_probe) // TO DO CLEAN
{
    if (curr_probe->nb == 1)
        return (1);
    t_probe *prev_probe = get_probe(dt->hop_probes, curr_probe->nb - 1);
    char prev[MAX_IP_LEN];
    char curr[MAX_IP_LEN];
    ft_strcpy(prev, inet_ntoa(curr_probe->address.sin_addr));
    ft_strcpy(curr, inet_ntoa(prev_probe->address.sin_addr));
    if (ft_strcmp(prev, curr) == 0)
        return (0);
    return (1);
}

void    display_hop(t_data *dt)
{
    char    buf[512];

    t_probe *curr_probe = (t_probe *)ft_lst_get_last_node(&dt->hop_probes)->content; // TO DO PROTECT BY DESIGN
    if (dt->curr_probe == 1)
        printf("%-4d  ", dt->curr_ttl);
    if (is_new_address(dt, curr_probe))
        printf("%s  ", hop_information(dt, buf, curr_probe));
    printf("%.3f ms  ", (float)curr_probe->time / 1000);
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
