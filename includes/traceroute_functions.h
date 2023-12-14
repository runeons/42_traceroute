#ifndef TRACEROUTE_FUNCTIONS_H
# define TRACEROUTE_FUNCTIONS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/ip_icmp.h>
# include <netinet/ip.h>
# include <netinet/udp.h>
# include <libft.h>
# include <utils_colors.h>
# include <utils_constants.h>
# include <utils_options.h>
# include <traceroute_data.h>
# include <errno.h>

// main.c
void            exit_error_clear(t_data *dt, const char *msg, ...);

//  craft.c
void            craft_udp(t_data *dt);

//  traceroute.c
void            traceroute(t_data *dt);

//  socket.c
void            resolve_hostname(t_data *dt);
void            resolve_address(t_data *dt);
void            open_raw_socket(t_data *dt);
void            open_udp_socket(t_data *dt);
void            set_socket_option_ttl(int socket, t_data *dt);
void            set_socket_option_timeout(int socket, t_data *dt);

//  options.c
void            init_options_params(t_data *dt);

//  init_data.c
void            init_data(t_data *dt, t_parsed_cmd *parsed_cmd);

//  display.c
char            *addr_to_str(int addr);
void            display_traceroute_init(t_data *dt);

//  utils_debug.c
void            debug_addrinfo(struct addrinfo *ai);
void            debug_sockaddr_in(struct sockaddr_in *addr);
void            debug_icmp(t_icmp *icmp_packet);
void            debug_udp(t_udp *udp_packet);

#endif
