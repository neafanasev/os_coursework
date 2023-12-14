#ifndef WRAPPED_FUNCS_H
#define WRAPPED_FUNCS_H

#include <sys/types.h>
#include <sys/socket.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);

void ConnectClient(int* out_socket, int port);


#endif