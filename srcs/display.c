#include "traceroute_functions.h"

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl, PACKET_SIZE);
    if (gettimeofday(&dt->init_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "traceroute: cannot retrieve time\n");
}

static void resolve_hop_name(t_data *dt, struct sockaddr_in hop_addr)
{
    char    host[MAX_HOSTNAME_LEN];
    int r = 0;

    ft_bzero(host, MAX_HOSTNAME_LEN);
    r = getnameinfo((struct sockaddr*)&(hop_addr), sizeof(hop_addr), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("traceroute: address error: The hop name could not be resolved. %d\n", r);
    else
    {
        dt->hop_name = ft_strdup(host);
        if (dt->hop_name == NULL)
            exit_error("traceroute: malloc failure.\n");
    }
}

static char    *hop_information(t_data *dt, char *buf, struct sockaddr_in hop_addr)
{
    ft_bzero(buf, 512);
    if (is_activated_option(dt->act_options, 'n'))
        sprintf(buf, "%s", inet_ntoa(hop_addr.sin_addr));
    else
    {
        resolve_hop_name(dt, hop_addr);
        sprintf(buf, "%s (%s)", dt->hop_name, inet_ntoa(hop_addr.sin_addr));
    }
    return (buf);
}

static int     curr_prob_time(t_data *dt)
{
    t_lst *last = NULL;

    last = ft_lst_get_last_node(&dt->hop_times);
    if (last && last->content)
        return (*(int *)last->content);
    else
        return (0);
}

void    display_hop(t_data *dt, struct sockaddr_in hop_addr)
{
    char    buf[512];
    int     curr_probe_time = 0;

    curr_probe_time = curr_prob_time(dt);
    if (dt->curr_probe == 1)
        printf("%-4d  ", dt->curr_ttl);
    if (ft_lst_size(dt->hop_times) == 1)
        printf("%s  ", hop_information(dt, buf, hop_addr));
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
