#include "traceroute_functions.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG && ai != NULL)
    {
        printf(C_B_RED"[DEBUG] addrinfo"C_RES"\n");
        printf("        ai_family: %d\n",   ai->ai_family);
        printf("        ai_socktype: %d\n", ai->ai_socktype);
        printf("        ai_addr: %s\n",     inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
        printf("\n");
    }
}

void    debug_sockaddr_in(struct sockaddr_in *addr)
{
    if (DEBUG && addr != NULL)
    {
        printf(C_B_RED"[DEBUG] sockaddr_in"C_RES"\n");
        printf("        sin_family: %d\n",              addr->sin_family);
        printf("        sin_port: %d\n",                addr->sin_port);
        printf("        sin_addr.s_addr: %s (%d)\n",    inet_ntoa(addr->sin_addr), addr->sin_addr.s_addr);
        printf("\n");
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

void    debug_one_probe(void *content)
{
    if (content)
    {
        t_probe *p = (t_probe *)content;
        printf("\n"C_G_RED"[PROBE %d] (%d) [%s] [%s]"C_RES"\n", p->nb, p->time, inet_ntoa(p->address.sin_addr), p->name);
    }
}

void    debug_probes(t_data *dt)
{
    printf("\n_________________debug_probes_________________\n");
    ft_lst_iter_content(dt->hop_probes, debug_one_probe);
}
