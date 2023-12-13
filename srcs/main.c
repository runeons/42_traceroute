#include "traceroute_functions.h"
#include "utils_options.h"

int g_traceroute = 1;

void    handle_sigint(int err)
{
    (void)err;
    g_traceroute = 0;
}

void    add_destination(t_data *dt, char *curr_arg)
{
    if (dt)
        dt->input_dest = curr_arg;
}

void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

void    parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av)
{
    if (ac < 2)
        option_h();
    *parsed_cmd = parse_options(ac, av);
    debug_activated_options(parsed_cmd->act_options);
}

void    initialise_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    init_data(dt, parsed_cmd);
    init_options_params(dt);
    if (ft_lst_size(parsed_cmd->not_options) != 1)
        exit_error("ping: usage error: Destination required and only one.\n");
    else
        add_destination(dt, parsed_cmd->not_options->content);
    resolve_address(dt);
    resolve_hostname(dt);
    open_udp_socket(dt);
    // open_socket(dt);
    // set_socket_options(dt->socket, dt);
}

void    traceroute_init(t_data *dt)
{
    display_traceroute_init(dt);
    if (gettimeofday(&dt->init_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
}

void    traceroute_end(t_data *dt)
{
    // display_ping_end_stats(dt);
    close(dt->socket);
    free_all_malloc();
}

int main(int ac, char **av)
{
    t_data          dt;
    t_parsed_cmd    parsed_cmd;

    parse_input(&parsed_cmd, ac, av);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    initialise_data(&dt, &parsed_cmd);
    signal(SIGINT, handle_sigint);
    traceroute_init(&dt);
    while (g_traceroute)
        trace_hops(&dt);
    traceroute_end(&dt);
    return (0);
}