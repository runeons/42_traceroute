#include "traceroute_functions.h"

char    *addr_to_str(int addr)
{
    char res[MAX_IP_LEN];
    
    sprintf(res, "%u.%u.%u.%u", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
    return (ft_strdup(res));
}

static unsigned int    get_flags(unsigned int frag_off)
{
    unsigned int flags = 0;

    flags = (ntohs(frag_off) & IP_FLAGS_MASK) >> IP_OFFSET_SHIFT;
    if (DEBUG == 1)
        printf(C_G_RED"flags: %04x"C_RES"\n", flags);
    return (flags);
}

static unsigned int    get_offset(unsigned int frag_off)
{
    unsigned int offset = 0;

    offset = ntohs(frag_off) & IP_OFFSET_MASK;
    if (DEBUG == 1)
        printf(C_G_RED"offset: %04x"C_RES"\n", offset);
    return (offset);
}

static void    display_ip_header(unsigned char *bytes_ip)
{
    struct iphdr    *ip = (struct iphdr *)(bytes_ip);

    printf("IP Hdr Dump:\n");
    for (int i = 0; i < IP_HEADER_LEN; i = i + 2)
        printf(" %02X%02X", bytes_ip[i], bytes_ip[i + 1]);
    printf("\n");
    printf("%2s %2s %3s %4s %4s %3s %4s %3s %3s %4s %12s %12s %s\n", "Vr","HL","TOS","Len","ID","Flg","off","TTL","Pro","cks","Src","Dst","Data");
    printf("%2x %2x %3x %04x %4x %3x %04x %03x %03x %4x %12s %12s\n", ip->version, ip->ihl, ip->tos, htons(ip->tot_len), ip->id, get_flags(ip->frag_off), get_offset(ip->frag_off), ip->ttl, ip->protocol, htons(ip->check), addr_to_str(ip->saddr), addr_to_str(ip->daddr));

}

static void    display_icmp_header(t_data *dt, unsigned char *bytes)
{
    struct icmphdr  *icmp_in_payload = (struct icmphdr *)(bytes + IP_HEADER_LEN + ICMP_HEADER_LEN);

    printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04d\n", icmp_in_payload->type, icmp_in_payload->code, dt->one_seq.bytes, htons(icmp_in_payload->un.echo.id), icmp_in_payload->un.echo.sequence);
}

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", dt->input_dest, dt->resolved_address, dt->max_hops); // TR to replace 60
}

void    display_ping_OK(t_data *dt)
{
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", dt->one_seq.bytes, addr_to_str(dt->one_seq.final_packet.ip->saddr), dt->one_seq.final_packet.icmp->un.echo.sequence, dt->one_seq.final_packet.ip->ttl, (float)dt->one_seq.time / 1000);
}

void    display_ping_error(t_data *dt, char *msg)
{
    printf("%d bytes from %s: %s\n", dt->one_seq.bytes, addr_to_str(dt->one_seq.final_packet.ip->saddr), msg);
    debug_packet(&dt->one_seq.final_packet);
    if (dt->options_params.v)
    {
        unsigned char *bytes = (unsigned char *)dt->one_seq.final_packet.icmp;
        display_ip_header(bytes + ICMP_HEADER_LEN);
        display_icmp_header(dt, bytes);
    }
}

void    display_ping_end_stats(t_data *dt)
{
    int             ratio;
    struct timeval  final_tv;

    if (gettimeofday(&final_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
    ratio = 100 - ((dt->end_stats.recv_nb * 100) / dt->end_stats.sent_nb);
    printf("--- %s ping statistics ---\n", dt->input_dest);
    printf("%d packets transmitted, %d packets received, %d%% packet loss\n", dt->end_stats.sent_nb, dt->end_stats.recv_nb, ratio);
    if (ft_lst_size(dt->end_stats.times) != 0)
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", (float)get_time_min(dt) / 1000, (float)get_time_avg(dt) / 1000, (float)get_time_max(dt) / 1000, (float)get_time_std(dt) / 1000);
}
