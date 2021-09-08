#ifndef TELNET_H
#define TELNET_H
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int telnet(char* ip_addr, int port, char *message, char *server_reply);
#endif