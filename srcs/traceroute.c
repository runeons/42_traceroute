#include "traceroute_functions.h"

void    display_traceroute_hop(t_data *dt, struct sockaddr_in *r_addr)
{
    printf("%-4d %s\n", dt->curr_ttl, addr_to_str(r_addr->sin_addr.s_addr));
}

void    display_traceroute_hop_NOK(t_data *dt, struct sockaddr_in *r_addr)
{
    printf(C_G_RED"%-4d %s %s\n"C_RES, dt->curr_ttl, addr_to_str(r_addr->sin_addr.s_addr), strerror(errno));
}

static void    handle_reply(t_data *dt, struct sockaddr_in *r_addr)
{
    // if (dt->icmp_packet.h.type == ICMP_ERR_TIME_EXCEEDED)
    //     printf(C_G_RED"[QUICK DEBUG] : %s"C_RES"\n", "Time to live exceeded");
    // else if (dt->icmp_packet.h.type == ICMP_ERR_UNREACHABLE)
    //     printf(C_G_RED"[QUICK DEBUG] : %s"C_RES"\n", "Destination Host Unreachable");
    // else if (dt->icmp_packet.h.type == ICMP_ECHO_REPLY)
    //     printf(C_G_RED"[QUICK DEBUG] : %s"C_RES"\n", "OK Echo Reply");
    // else
    //     warning_error(C_G_BLUE"Packet type %d"C_RES"\n", dt->icmp_packet.h.type);
    // printf("ICMP received from TTL %d:\n", dt->curr_ttl);
    // debug_sockaddr_in(r_addr);
    // debug_icmp(&dt->icmp_packet);
    display_traceroute_hop(dt, r_addr);
}

static void    receive_icmp(t_data *dt)
{
    int                 r = 0;	
	struct sockaddr_in  r_addr;
	int		            r_addr_len = 0;

	r_addr_len = sizeof(r_addr);
    memset(&dt->icmp_packet, 0, sizeof(dt->icmp_packet));
    r = recvfrom(dt->socket_raw, &dt->icmp_packet, sizeof(dt->icmp_packet), 0, (struct sockaddr *)&r_addr, (socklen_t *)&r_addr_len);
    if (r >= 0)
        handle_reply(dt, &r_addr); // then, send new request after handle_reply when same id as request
    else
    {
        display_traceroute_hop_NOK(dt, &r_addr);
        // warning_error(C_G_BLUE"No reply received"C_RES"\n"); // then, ignore and send new request if no reply received
    }
}

static void    update_address(t_data *dt)
{
    dt->address.sin_port = dt->dest_port; // TO CHECK - need dt->dest_port ?
}


static void    send_udp_and_receive_icmp(t_data *dt)    
{
    int     r = 0;	
    
    r = sendto(dt->socket_udp, &dt->udp_packet,  sizeof(dt->udp_packet), 0, (struct sockaddr *)&dt->address, sizeof(dt->address));
    if (r <= 0)
        warning_error(C_G_RED"packet sending failure : %s"C_RES"\n", strerror(r));
    else if (r != sizeof(dt->udp_packet))
        warning_error(C_G_RED"packet not completely sent"C_RES"\n");
    else
    {
        open_raw_socket(dt);
        set_socket_option_timeout(dt->socket_raw, dt);
        receive_icmp(dt);
    }
}

void traceroute(t_data *dt)
{
    set_socket_option_ttl(dt->socket_udp, dt);
    craft_udp(dt);
    // debug_udp(&dt->udp_packet);
    update_address(dt);
    // debug_sockaddr_in(&dt->address);
	// usleep(1000000);
    send_udp_and_receive_icmp(dt);
    
    dt->curr_ttl++;
    dt->dest_port++;
    dt->src_port++;
}