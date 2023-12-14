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
            dt->nb_probes = ft_atoi(param);
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
            dt->max_ttl = ft_atoi(param);
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
            dt->reply_timeout = ft_atoi(param);
    }
    else
        dt->reply_timeout = REPLY_TIMEOUT;
}

void    init_options_params(t_data *dt)
{
    option_q(dt);
    option_m(dt);
    option_w(dt);
    // option_p(dt);
    // option_s(dt);
    // option_z(dt);
    // option_f(dt);
}
