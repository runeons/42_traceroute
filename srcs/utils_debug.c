#include "traceroute_functions.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG == 1 && ai != NULL)
    {
        printf(C_B_RED"[DEBUG] addrinfo"C_RES"\n");
        printf("        ai_family: %d\n", ai->ai_family);
        printf("        ai_socktype: %d\n", ai->ai_socktype);
        printf("        ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
        printf("\n");
    }
}

void    debug_sockaddr_in(struct sockaddr_in *addr)
{
    if (DEBUG == 1 && addr != NULL)
    {
        printf(C_B_RED"[DEBUG] sockaddr_in"C_RES"\n");
        printf("        sin_family: %d\n", addr->sin_family);
        printf("        sin_port: %d\n", addr->sin_port);
        printf("        sin_addr.s_addr: %s (%d)\n", addr_to_str(addr->sin_addr.s_addr), addr->sin_addr.s_addr);
        printf("\n");
    }
}

void    debug_icmp(t_icmp *icmp_packet)
{
    if (DEBUG == 1 && icmp_packet != NULL)
    {
        printf("[DEBUG] t_icmp\n");
        printf("sizeof(icmp_packet): %lu\n", sizeof(icmp_packet));
        printf("sizeof(icmp_packet->payload): %lu\n", sizeof(icmp_packet->payload));
        printf("payload: %s\n", icmp_packet->payload);
        printf("h.type: %d (0x%x)\n", icmp_packet->h.type, icmp_packet->h.type);
        printf("h.code: %d (0x%x)\n", icmp_packet->h.code, icmp_packet->h.code);
        printf("h.checksum: %d (0x%x)\n", icmp_packet->h.checksum, icmp_packet->h.checksum);
        printf("h.un.echo.id: %d (0x%x)\n", icmp_packet->h.un.echo.id, icmp_packet->h.un.echo.id);
        printf("h.un.echo.sequence: %d (0x%x)\n", icmp_packet->h.un.echo.sequence, icmp_packet->h.un.echo.sequence);
        printf("\n");
    }
}

void    debug_udp(t_udp *udp_packet)
{
    if (DEBUG == 1 && udp_packet != NULL)
    {
        printf(C_G_RED"[DEBUG] t_udp"C_RES"\n");
        printf("sizeof(udp_packet): %lu\n", sizeof(udp_packet));
        printf("sizeof(udp_packet->payload): %lu\n", sizeof(udp_packet->payload));
        printf("payload: %s\n", udp_packet->payload);
        printf("h.uh_sport: %d\n", udp_packet->h.uh_sport);
        printf("h.uh_dport: %d\n", udp_packet->h.uh_dport);
        printf("h.uh_ulen: %d\n", udp_packet->h.uh_ulen);
        printf("h.uh_sum: %d\n", udp_packet->h.uh_sum);
        printf("\n");
    }
}
