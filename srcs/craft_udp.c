#include "traceroute_functions.h"

// one complement (= inversement de tous les bits) sur 16 bits / 2 bytes
// de la somme des one complements de l'udp header
// pris par mots de 16 bits / 2 bytes / unsigned short

static unsigned short header_checksum(void *packet, int len)
{
    unsigned short *tmp;
    unsigned int checksum;

    tmp = packet;
    checksum = 0;
    while (len > 1)
    {
        // printf("[%d] checksum: %d, checksum[%s]\n", len, checksum, int_to_bin(checksum, 16));
        checksum += *tmp++;
        len -= sizeof(*tmp);
    }
    checksum = (unsigned short)(~((checksum >> 16) + (checksum & 0xFFFF))); // final one complement
    if (DEBUG == 1)
    {
        printf(C_B_RED "[DEBUG] checksum: %d, checksum: [%s]" C_RES "\n", checksum, int_to_bin(checksum, 16));
    }
    return (checksum);
}

static void craft_udp_data(t_data *dt)
{
    struct timeval current_time;

    if (gettimeofday(&current_time, NULL) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
    ft_memset(&dt->crafted_udp, 0, sizeof(dt->crafted_udp));
    printf(C_G_RED"[QUICK DEBUG] UDP_PAYLOAD: %s"C_RES"\n", UDP_PAYLOAD);
    for (int i = 0; i < UDP_D_LEN; i++)
        dt->crafted_udp.payload[i] = UDP_PAYLOAD[i];
    dt->crafted_udp.payload[UDP_D_LEN] = '\0';
    // dt->one_seq.udp_seq_count++;
}

static void craft_udp_header(t_data *dt)
{
    dt->crafted_udp.h.uh_sport = 44558; // RDM ?
    dt->crafted_udp.h.uh_dport = 34434;
    dt->crafted_udp.h.uh_ulen = 40;
    dt->crafted_udp.h.uh_sum = header_checksum(&dt->crafted_udp, sizeof(dt->crafted_udp));
}

void craft_udp(t_data *dt)
{
    ft_bzero(&dt->crafted_udp, sizeof(dt->crafted_udp));
    craft_udp_data(dt);
    craft_udp_header(dt);
}