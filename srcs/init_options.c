#include "traceroute_functions.h"

void   option_q(t_data *dt)
{
    int  nb_probes  = 0;
    char *param     = NULL;

    if (is_activated_option(dt->act_options, 'q'))
    {
        param = ft_strdup(get_option(dt->act_options, 'q')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("traceroute: invalid value: (`%s' near `%s')\n", param, param);
        nb_probes = ft_atoi(param);
        if (nb_probes <= 0)
            exit_error("traceroute: option value too small: %d\n", nb_probes);
        else if (nb_probes > 255)
            exit_error("traceroute: option value too big: %d\n", nb_probes);
        else
            dt->nb_probes = nb_probes;
    }
    else
        dt->nb_probes = NB_PROBES;
}

void   option_m(t_data *dt)
{
    int  max_ttl   = 0;
    char *param     = NULL;

    if (is_activated_option(dt->act_options, 'm'))
    {
        param = ft_strdup(get_option(dt->act_options, 'm')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("traceroute: invalid value: (`%s' near `%s')\n", param, param);
        max_ttl = ft_atoi(param);
        if (max_ttl <= 0)
            exit_error("traceroute: option value too small: %d\n", max_ttl);
        else if (max_ttl > 255)
            exit_error("traceroute: option value too big: %d\n", max_ttl);
        else
            dt->max_ttl = max_ttl;
    }
    else
        dt->max_ttl = MAX_TTL;
}

void   option_w(t_data *dt)
{
    int  reply_timeout  = 0;
    char *param         = NULL;

    if (is_activated_option(dt->act_options, 'w'))
    {
        param = ft_strdup(get_option(dt->act_options, 'w')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("traceroute: invalid value: (`%s' near `%s')\n", param, param);
        reply_timeout = ft_atoi(param);
        if (reply_timeout <= 0)
            exit_error("traceroute: option value too small: %d\n", reply_timeout);
        else if (reply_timeout > 255)
            exit_error("traceroute: option value too big: %d\n", reply_timeout);
        else
            dt->reply_timeout = reply_timeout;
    }
    else
        dt->reply_timeout = REPLY_TIMEOUT;
}

void   option_z(t_data *dt)
{
    float  interval       = 0;
    int    interval_us    = 0;
    char   *param         = NULL;

    if (is_activated_option(dt->act_options, 'z'))
    {
        param = ft_strdup(get_option(dt->act_options, 'z')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrfloat(param) == 0)
            exit_error("traceroute: invalid value (`%s' near `%s')\nTry 'traceroute --help' for more information.\n", param, param);
        interval = ft_atof(param);
        interval_us = (int)(interval * 1000000);
        if (interval_us < 200000)
            exit_error("traceroute: option value too small: %s\n", param);
        else if (interval > 2048)
            exit_error("traceroute: option value too big: %s\n", param);
        else
            dt->probes_interval_us = interval_us;
    }
    else
        dt->probes_interval_us = PROBES_INTERVAL_S * 1000000;
}

void   option_p(t_data *dt)
{
    int  dst_port  = 0;
    char *param     = NULL;

    if (is_activated_option(dt->act_options, 'p'))
    {
        param = ft_strdup(get_option(dt->act_options, 'p')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("traceroute: invalid value: (`%s' near `%s')\n", param, param);
        dst_port = ft_atoi(param);
        if (dst_port <= 0)
            exit_error("traceroute: option value too small: %d\n", dst_port);
        else if (dst_port > MAX_PORT)
            exit_error("traceroute: option value too big: %d\n", dst_port);
        else
            dt->dst_port = dst_port;
    }
    else
        dt->dst_port = DST_PORT;
}

void   option_f(t_data *dt)
{
    int  first_ttl  = 0;
    char *param     = NULL;

    if (is_activated_option(dt->act_options, 'f'))
    {
        param = ft_strdup(get_option(dt->act_options, 'f')->param);
        if (param == NULL)
            exit_error("traceroute: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("traceroute: invalid value: (`%s' near `%s')\n", param, param);
        first_ttl = ft_atoi(param);
        if (first_ttl <= 0)
            exit_error("traceroute: option value too small: %d\n", first_ttl);
        else if (first_ttl >= dt->max_ttl)
            exit_error("traceroute: option first ttl value higher than max ttl: %d\n", first_ttl);
        else
            dt->curr_ttl = first_ttl;
    }
    else
        dt->curr_ttl = FIRST_TTL;
}

void    init_options_params(t_data *dt)
{
    option_q(dt);
    option_m(dt);
    option_w(dt);
    option_z(dt);
    option_p(dt);
    option_f(dt);
    // option_s(dt);
}
