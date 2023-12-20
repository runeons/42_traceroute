#include "traceroute_functions.h"

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl, SEND_PACKET_SIZE);
    if (gettimeofday(&dt->init_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
}

static char    *hop_information(t_data *dt, char *buf, t_probe *curr_probe)
{
    ft_bzero(buf, PRINT_BUF_SIZE);
    if (is_activated_option(dt->act_options, 'n'))
        sprintf(buf, "%s", inet_ntoa(curr_probe->address.sin_addr));
    else
        sprintf(buf, "%s (%s)", curr_probe->name, inet_ntoa(curr_probe->address.sin_addr));
    return (buf);
}

static int     is_new_address(t_data *dt, t_probe *curr_probe)
{
    t_probe *prev_probe = NULL;
    char    prev[MAX_IP_LEN];
    char    curr[MAX_IP_LEN];
    
    if (curr_probe->nb == 1)
        return (1);
    prev_probe = get_probe(dt->hop_probes, curr_probe->nb - 1);
    ft_strcpy(prev, inet_ntoa(curr_probe->address.sin_addr));
    ft_strcpy(curr, inet_ntoa(prev_probe->address.sin_addr));
    if (ft_strcmp(prev, curr) == 0)
        return (0);
    return (1);
}

void    display_hop(t_data *dt)
{
    char    buf[PRINT_BUF_SIZE];

    t_probe *curr = get_probe(dt->hop_probes, dt->curr_probe);
    if (curr)
    {
        if (dt->curr_probe == 1)
            printf("%-4d  ", dt->curr_ttl);
        if (is_new_address(dt, curr))
            printf("%s  ", hop_information(dt, buf, curr));
        printf("%.3f ms  ", (float)curr->time / 1000);
        if (dt->nb_probes > 1)
            fflush(stdout);
        if (dt->curr_probe == dt->nb_probes)
            printf("\n");
    }
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
