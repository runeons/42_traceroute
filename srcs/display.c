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

t_probe *get_probe(t_lst *hop_probes, int nb)
{
    while (hop_probes != NULL)
    {
        t_probe *tmp = (t_probe *)hop_probes->content;
        if (tmp->nb == nb)
            return (tmp);
        hop_probes = hop_probes->next;
    }
    return (NULL);
}

static char    *hop_information(t_data *dt, char *buf, t_probe *curr_probe)
{
    ft_bzero(buf, 512);
    if (is_activated_option(dt->act_options, 'n'))
        sprintf(buf, "%s", inet_ntoa(curr_probe->address.sin_addr));
    else
    {
        resolve_hop_name(dt, curr_probe->address);
        sprintf(buf, "%s (%s)", dt->hop_name, inet_ntoa(curr_probe->address.sin_addr));
    }
    return (buf);
}

int     is_new_address(t_data *dt, t_probe *curr_probe)
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

void    display_hop(t_data *dt, struct sockaddr_in hop_addr)
{
    char    buf[512];

    (void)hop_addr;
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
