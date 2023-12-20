#include "traceroute_functions.h"

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

int     get_send_port(void *packet)
{
    int port = 0;

    port = *(int *)(packet + IP_H_LEN + 2);
    return (ntohs(port)); // TO DO make my own ?
}

int     get_curr_send_port(t_data *dt)
{
    int port = 0;

    t_probe *curr = get_probe(dt->hop_probes, dt->curr_probe);
    if (curr)
        port = curr->send_port;
    return (port);
}

int     get_reply_port(void *packet)
{
    int port = 0;

    port = *(int *)(packet + IP_H_LEN + ICMP_H_LEN + IP_H_LEN + 2);
    return (ntohs(port));
}
