#include "traceroute_functions.h"

// one complement (= inversement de tous les bits) sur 16 bits / 2 bytes
// de la somme des one complements de l'udp header
// pris par mots de 16 bits / 2 bytes / unsigned short

char    *int_to_bin(int n, int len)
{
    char    *bin;
    int     k = 0;

    bin = (char*)mmalloc(sizeof(char) * len);
    if (bin == NULL)
        exit_error("ping: malloc failure.");
    for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2)
        bin[k++] = (n & i) ? '1' : '0';
    bin[k] = '\0';
    return (bin);
}

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
    // if (DEBUG == 1)
    // {
    //     printf(C_B_RED "[DEBUG] checksum: %d, checksum: [%s]" C_RES "\n", checksum, int_to_bin(checksum, 16));
    // }
    return (checksum);
}

static void craft_udp_data(t_data *dt)
{
    for (int i = 0; i < UDP_D_LEN; i++)
        dt->udp_packet.payload[i] = UDP_PAYLOAD[i];
    dt->udp_packet.payload[UDP_D_LEN] = '\0';
}

static void craft_udp_header(t_data *dt)
{
    dt->udp_packet.h.uh_sport = htons(dt->src_port); // RDM ?
    dt->udp_packet.h.uh_dport = htons(dt->dest_port);
    dt->udp_packet.h.uh_ulen = sizeof(t_udp);
    dt->udp_packet.h.uh_sum = header_checksum(&dt->udp_packet, sizeof(dt->udp_packet));
}

void craft_udp(t_data *dt)
{
    ft_memset(&dt->udp_packet, 0, sizeof(dt->udp_packet));
    craft_udp_data(dt);
    craft_udp_header(dt);
}