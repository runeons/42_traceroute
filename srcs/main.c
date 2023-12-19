#include "traceroute_functions.h"
#include "utils_options.h"

int g_loop = 1;

void    handle_sigint(int err)
{
    (void)err;
    g_loop = 0;
}

void    close_sockets(t_data *dt)
{
    if (dt->socket > 0)
        close(dt->socket);
}

void    traceroute_end(t_data *dt)
{
    close_sockets(dt);
    free_all_malloc();
}

void    exit_error_clear(t_data *dt, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    traceroute_end(dt);
    exit(1);
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
    // debug_activated_options(parsed_cmd->act_options);
}

void    add_destination(t_data *dt, char *curr_arg)
{
    if (dt)
        dt->input_dest = curr_arg;
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
}

int main(int ac, char **av)
{
    t_data          dt;
    t_parsed_cmd    parsed_cmd;
    int             ttl = 1;

    parse_input(&parsed_cmd, ac, av);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    initialise_data(&dt, &parsed_cmd);
    open_main_socket(&dt);
    // debug_sockaddr_in(&dt.target_address);
    signal(SIGINT, handle_sigint);
    display_traceroute_init(&dt);
    ttl = dt.first_ttl;
    while (g_loop && ttl <= dt.max_ttl)
        reach_hop(&dt, ttl++);  
    traceroute_end(&dt);
    return (0);
}