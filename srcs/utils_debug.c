#include "traceroute_functions.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG == 1)
    {
        printf(C_B_RED"[DEBUG] addrinfo"C_RES"\n");
        printf("        ai_family: %d\n", ai->ai_family);
        printf("        ai_socktype: %d\n", ai->ai_socktype);
        printf("        ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
        printf("\n");
    }
}

static char    *int_addr_to_str(int addr)
{
    char res[MAX_IP_LEN];
    
    sprintf(res, "%u.%u.%u.%u", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
    return (ft_strdup(res));
}

void    debug_sockaddr_in(struct sockaddr_in *addr)
{
    if (DEBUG == 1)
    {
        printf(C_B_RED"[DEBUG] sockaddr_in"C_RES"\n");
        printf("        sin_family: %d\n", addr->sin_family);
        printf("        sin_port: %d\n", addr->sin_port);
        printf("        sin_addr.s_addr: %s (%d)\n", int_addr_to_str(addr->sin_addr.s_addr), addr->sin_addr.s_addr);
        printf("\n");
    }
}

void    debug_crafted_icmp(t_icmp *crafted_icmp)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] crafted t_icmp"C_RES"\n");
        printf("sizeof(crafted_icmp): %lu\n", sizeof(crafted_icmp));
        printf("sizeof(crafted_icmp->payload): %lu\n", sizeof(crafted_icmp->payload));
        printf("payload: %s\n", crafted_icmp->payload);
        printf("h.type: %d\n", crafted_icmp->h.type);
        printf("h.code: %d\n", crafted_icmp->h.code);
        printf("h.checksum: %d\n", crafted_icmp->h.checksum);
        printf("h.un.echo.id: %d\n", crafted_icmp->h.un.echo.id);
        printf("h.un.echo.sequence: %d\n", crafted_icmp->h.un.echo.sequence);
        printf("h.un.echo.id: %x\n", crafted_icmp->h.un.echo.id);
        printf("h.un.echo.sequence: %x\n", crafted_icmp->h.un.echo.sequence);
        printf("\n");
    }
}

void    debug_crafted_udp(t_udp *crafted_udp)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] crafted t_udp"C_RES"\n");
        printf("sizeof(crafted_udp): %lu\n", sizeof(crafted_udp));
        printf("sizeof(crafted_udp->payload): %lu\n", sizeof(crafted_udp->payload));
        printf("payload: %s\n", crafted_udp->payload);
        printf("h.uh_sport: %d\n", crafted_udp->h.uh_sport);
        printf("h.uh_dport: %d\n", crafted_udp->h.uh_dport);
        printf("h.uh_ulen: %d\n", crafted_udp->h.uh_ulen);
        printf("h.uh_sum: %d\n", crafted_udp->h.uh_sum);
        printf("\n");
    }
}

void    debug_msghdr(struct msghdr msg)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] recv buf"C_RES"\n");
        printf("iov_base ====> %s \n", (char *)msg.msg_iov[0].iov_base);
        printf("iov_len ====> %lu \n", msg.msg_iov[0].iov_len);
        printf("msg_name ====> %s \n", (char *)msg.msg_name);
        printf("msg_namelen ====> %u \n", msg.msg_namelen);
        printf("msg_iovlen ====> %lu \n", msg.msg_iovlen);
        printf("msg_control ====> %s \n", (char *)msg.msg_control);
        printf("msg_controllen ====> %lu \n", msg.msg_controllen);
        printf("msg_flags ====> %d \n\n", msg.msg_flags);  
        printf("\n");
    }
}

void    debug_packet(t_packet *packet)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] RECEIVED PACKET"C_RES"\n");
        printf(C_G_RED"[DEBUG] IP"C_RES"\n");
        printf("    version: %x\n", packet->ip->version);
        printf("    ihl: %x\n", packet->ip->ihl);
        printf("    tos: %d\n", packet->ip->tos);
        printf("    tot_len: %hx\n", packet->ip->tot_len);
        printf("    id: %04x\n", packet->ip->id);
        printf("    frag_off: %hx\n", ntohs(packet->ip->frag_off));
        printf("    ttl: %hhx\n", packet->ip->ttl);
        printf("    protocol: %hhx\n", packet->ip->protocol);
        printf("    check: 0x%04x\n", packet->ip->check);
        printf("    saddr: %08x\n", packet->ip->saddr);
        printf("    daddr: %08x\n", packet->ip->daddr);
        printf(C_G_RED"[DEBUG] ICMP"C_RES"\n");
        printf("    type: %d\n", packet->icmp->type);
        printf("    code: %d\n", packet->icmp->code);
        printf("    checksum: 0x%04x\n", packet->icmp->checksum);
        printf("    id: 0x%04x\n", packet->icmp->un.echo.id);
        printf("    sequence: 0x%04x\n", packet->icmp->un.echo.sequence);
        printf(C_G_RED"[DEBUG] ICMP IN PAYLOAD"C_RES"\n");
        struct icmphdr  *icmp_in_payload = (struct icmphdr *)((char *)packet->icmp + IP_HEADER_LEN + ICMP_HEADER_LEN);
        printf("    id: 0x%04x\n", icmp_in_payload->un.echo.id);
        printf("    sequence: 0x%04x\n", icmp_in_payload->un.echo.sequence);
        printf("\n");

    }
}

void debug_time(void *content)
{
    if (content)
    {
        int *t = (int *)content;
        printf("time: %d, \n", *t);
    }
}