#include "traceroute_functions.h"
#include "utils_options.h"

int g_end       = 0;
int g_sigint    = 0;

static void    handle_sigint(int err)
{
    (void)err;
    g_end       = 1;
    g_sigint    = 1;
    printf("\n");
}

static void    traceroute_end(t_data *dt)
{
    if (dt->socket > 0)
        close(dt->socket);
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

static void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

static void    parse_input(t_parsed_cmd *parsed_cmd, int ac, char **av)
{
    if (ac < 2)
        option_h();
    *parsed_cmd = parse_options(ac, av);
    // debug_activated_options(parsed_cmd->act_options);
}

static void    add_destination(t_data *dt, char *curr_arg)
{
    if (dt)
        dt->input_dest = curr_arg;
}

static void    initialise_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    init_data(dt, parsed_cmd);
    init_options_params(dt);
    if (ft_lst_size(parsed_cmd->not_options) != 1)
        exit_error("traceroute: usage error: Destination required and only one.\n");
    else
        add_destination(dt, parsed_cmd->not_options->content);
    resolve_address(dt);
    resolve_hostname(dt);
}

int main(int ac, char **av)
{
    t_data          dt;
    t_parsed_cmd    parsed_cmd;

    parse_input(&parsed_cmd, ac, av);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    initialise_data(&dt, &parsed_cmd);
    open_main_socket(&dt);
    // debug_sockaddr_in(&dt.target_address);
    signal(SIGINT, handle_sigint);
    display_traceroute_init(&dt);
    while (!g_end && dt.curr_ttl <= dt.max_ttl)
        reach_hop(&dt);  
    traceroute_end(&dt);
    return (0);
}