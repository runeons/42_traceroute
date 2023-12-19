#include "traceroute_functions.h"

void init_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    // general
    dt->input_dest          = "";
    dt->act_options         = parsed_cmd->act_options;
    dt->resolved_address    = NULL;
    dt->resolved_hostname   = "";
    dt->socket              = 0;
    FD_ZERO(&dt->read_set);
    // options
    dt->max_ttl             = 0;
    dt->nb_probes           = 0;
    dt->reply_timeout       = 0;
    dt->probes_interval_us  = 0;
    // each hop
    dt->curr_ttl            = 1;
    dt->curr_probe          = 0;
	dt->dst_port            = 0;
	dt->src_port            = 32123; // TO DO RDM
    dt->hop_probes          = NULL;
    // addresses
    ft_memset(&(dt->local_address), 0, sizeof(struct sockaddr_in));
    ft_memset(&(dt->target_address), 0, sizeof(struct sockaddr_in));
    dt->target_address.sin_family = AF_INET;
    dt->target_address.sin_port = 0;
    dt->target_address.sin_addr.s_addr = INADDR_ANY;
}

void    init_probe(t_data *dt)
{
    t_probe *probe = NULL;

    probe = mmalloc(sizeof(t_probe));
    if (probe == NULL)
        exit_error_clear(dt, "traceroute: malloc failure.\n");
    probe->nb = dt->curr_probe;
    probe->time = 0;
    probe->name = NULL;
    ft_memset(&(probe->address), 0, sizeof(struct sockaddr_in));
    ft_lst_add_node_back(&dt->hop_probes, ft_lst_create_node(probe));
}
