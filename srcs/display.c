#include "traceroute_functions.h"

void    display_traceroute_init(t_data *dt)
{
    printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", dt->input_dest, dt->resolved_address, dt->max_ttl); // TR to replace 60
}
