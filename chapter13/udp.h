#pragma once
#include <netinet/in.h>
int make_internet_address(const char* addr_str,int port,sockaddr_in*addr);
int make_dgram_server_socket(int port);
int make_dgram_client_socket();
int get_internet_address(char* addr_str,int len,int*port,sockaddr_in* addr);
