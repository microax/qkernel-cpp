// TCPSOCKET.H	 Definition file for TCP_SERVER class.
//-----------------------------------------------------------------------------
// History:
// --------
// 01/15/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TCP_SOCKET
{
protected:

    int    socket_fd;
    int    new_socket_fd;
    int    backlog;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
   
public:    

    TCP_SOCKET(int port_number, int number_connect);	// Server
    TCP_SOCKET(int port_number, char *ip_address);	// Client
    ~TCP_SOCKET(void);
    int Accept(void);
    int Listen(void);
    int Connect(void);
    int Close(void);
    int CloseServer(void);
    int Read(char *buffer, int nbyte);
    int Write(char *buffer, int nbyte);
};

#endif









