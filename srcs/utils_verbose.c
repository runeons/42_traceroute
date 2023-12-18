#include "traceroute_functions.h"

void    verbose_h_dump(char *title, void *h, int len)
{
    unsigned char *bytes = (unsigned char *)h;

    if (g_loop)
    {
        printf("%-10s", title);
        for (int i = 0; i < len; i = i + 2)
            printf(" %02X%02X", bytes[i], bytes[i + 1]);    
        printf("\n");
    }
}

void    verbose_full_reply(void *packet)
{
    verbose_h_dump("   IPH",   packet,                                     sizeof(struct iphdr));
    verbose_h_dump("   ICMPH", packet + H_IP_LEN,                          sizeof(struct icmphdr));
    verbose_h_dump("   IPH",   packet + H_IP_LEN + H_ICMP_LEN,             sizeof(struct iphdr));
    verbose_h_dump("   UDPH",  packet + H_IP_LEN + H_ICMP_LEN + H_IP_LEN,  sizeof(struct udphdr));
}

void    verbose_full_send(void *packet)
{
    verbose_h_dump("   IP",    packet,             sizeof(struct iphdr));
    verbose_h_dump("   UDPH",  packet + H_IP_LEN,  sizeof(struct udphdr));
}
