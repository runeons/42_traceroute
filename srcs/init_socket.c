#include "traceroute_functions.h"

void resolve_address(t_data *dt) // check that dest exists and resolve address if input == hostname
{
    int                 r;
    struct addrinfo     *resolved_add;
    struct addrinfo     *tmp;

    r = getaddrinfo(dt->input_dest, NULL, NULL, &resolved_add);
    // debug_addrinfo(resolved_add);
    if (r != 0)
        exit_error("traceroute: unknown host\n");
    tmp = resolved_add;
    while (tmp != NULL)
    {
        char ip_str[MAX_IP_LEN];

        ft_bzero(ip_str, MAX_IP_LEN);
        if (inet_ntop(tmp->ai_family, &((struct sockaddr_in *)tmp->ai_addr)->sin_addr, ip_str, sizeof(ip_str)) == NULL)
            exit_error("traceroute: address error: Conversion from network to presentation format failed.\n");
        dt->resolved_address = ft_strdup(ip_str);
        if (dt->resolved_address == NULL)
            exit_error("traceroute: malloc failure.\n");
        tmp = tmp->ai_next;
        break; // useful if many
    }
    // if (DEBUG == 1)
    //     printf(C_B_RED"dt->resolved_address %s"C_RES"\n", dt->resolved_address);
    freeaddrinfo(resolved_add);
}

void resolve_hostname(t_data *dt) // useful only when input_dest is ip address (vs. hostname)
{
    char    host[MAX_HOSTNAME_LEN];
    int r = 0;

    ft_bzero(host, MAX_HOSTNAME_LEN);
    if (inet_pton(AF_INET, dt->resolved_address, &(dt->address.sin_addr)) <= 0)
        exit_error("traceroute: address error: Invalid IPv4 address.\n");
    r = getnameinfo((struct sockaddr*)&(dt->address), sizeof(dt->address), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("traceroute: address error: The hostname could not be resolved. %d\n", r);
    else
    {
        dt->resolved_hostname = ft_strdup(host);
        if (dt->resolved_hostname == NULL)
            exit_error("traceroute: malloc failure.\n");
    }
    // if (DEBUG == 1)
    //     printf(C_B_RED"dt->resolved_hostname %s"C_RES"\n", dt->resolved_hostname);
}

void open_raw_socket(t_data *dt)
{
    dt->socket_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket_raw < 0)
        exit_error("traceroute: raw socket error: Check that you have the correct rights.\n");
}

void open_udp_socket(t_data *dt)
{
    dt->socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (dt->socket_udp < 0)
        exit_error("traceroute: udp socket error: Please retry.\n");
}

void set_socket_option_ttl(int socket, t_data *dt)
{
    int r           = 0;

    r = setsockopt(socket, IPPROTO_IP, IP_TTL, &dt->curr_ttl, sizeof(dt->curr_ttl));
    if (r != 0)
        exit_error_clear(dt, "traceroute: socket error in setting TTL option: Exiting program.\n");
}

void set_socket_option_timeout(int socket, t_data *dt)
{
    int r           = 0;
    struct timeval tv_out;
	tv_out.tv_sec = dt->reply_timeout;
	tv_out.tv_usec = 0;
    r = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)); // setting timeout option
    if (r != 0)
        exit_error_clear(dt, "traceroute: socket error in setting timeout option: Exiting program.\n");
}
