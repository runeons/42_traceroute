#include "traceroute_functions.h"

void init_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    // general
    dt->input_dest = "";
    dt->act_options = parsed_cmd->act_options;
    dt->resolved_address = NULL;
    dt->resolved_hostname = "";
    dt->socket_udp = 0;
    dt->socket_raw = 0;
    
    ft_memset(&(dt->address), 0, sizeof(dt->address));
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;
    dt->max_ttl = 0;
    dt->nb_probes = 0;
    dt->reply_timeout = 0;
    
    // each hop
    dt->curr_ttl = 1;
    memset(&dt->udp_packet, 0, sizeof(dt->udp_packet));
    memset(&dt->icmp_packet, 0, sizeof(dt->icmp_packet));
	dt->dest_port = 34434;
	dt->src_port = 50000; // TO CHECK
}
