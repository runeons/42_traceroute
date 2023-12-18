#include "traceroute_functions.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG == 1 && ai != NULL)
    {
        printf(C_B_RED"[DEBUG] addrinfo"C_RES"\n");
        printf("        ai_family: %d\n", ai->ai_family);
        printf("        ai_socktype: %d\n", ai->ai_socktype);
        printf("        ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
        printf("\n");
    }
}

void    debug_sockaddr_in(struct sockaddr_in *addr)
{
    if (DEBUG == 1 && addr != NULL)
    {
        printf(C_B_RED"[DEBUG] sockaddr_in"C_RES"\n");
        printf("        sin_family: %d\n", addr->sin_family);
        printf("        sin_port: %d\n", addr->sin_port);
        printf("        sin_addr.s_addr: %s (%d)\n", inet_ntoa(addr->sin_addr), addr->sin_addr.s_addr);
        printf("\n");
    }
}
