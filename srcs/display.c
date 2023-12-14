#include "traceroute_functions.h"

char    *addr_to_str(int addr)
{
    char res[MAX_IP_LEN];
    
    sprintf(res, "%u.%u.%u.%u", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
    return (ft_strdup(res));
}

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl); // TR to replace 60
}
