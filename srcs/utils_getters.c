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

    port = *(int *)(packet + H_IP_LEN + 2);
    printf(C_G_RED"[QUICK DEBUG] SEND  port: %d"C_RES"\n", ntohs(port));
    return (ntohs(port)); // TO DO make my own ?
}

int     get_reply_port(void *packet)
{
    int port = 0;

    port = *(int *)(packet + H_IP_LEN + H_ICMP_LEN + H_IP_LEN + 2);
    printf(C_G_RED"[QUICK DEBUG] REPLY port: %d"C_RES"\n", ntohs(port));
    return (ntohs(port));
}
