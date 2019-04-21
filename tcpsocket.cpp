// TCPSOCKET.CPP	TCP/IP Socket class.
// ----------------------------------------------------------------------------
// History:
// --------
// 01/18/95 M. Gill 	Initial creation.
// ----------------------------------------------------------------------------
#include <tcpsocket.h>


// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Read()
//
// PURPOSE:
//
// INPUT:	NONE.
//
// RETURN:	NONE.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Read(char *buffer, int nbyte)
{
    int rc = read(socket_fd, buffer, nbyte);

    return(rc);
}



// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Write()
//
// PURPOSE:	
//
// INPUT:	NONE.
//
// RETURN:	NONE.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Write(char *buffer, int nbyte)
{

    int rc = write(socket_fd, buffer, nbyte);

    return(rc);
}

// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Accept()
//
// PURPOSE:	
//
// INPUT:	NONE.
//
// RETURN:	new socket descriptor.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Accept(void)
{
    int len= sizeof(client_address);
 
    int rc =accept(socket_fd,0,0);
			//  (struct sockaddr *)&client_address,
			//  &len );

    return(rc);
}

// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Listen()
//
// PURPOSE:
//
// INPUT:	NONE.
//
// RETURN:	NONE.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Listen(void)
{
    int rc = listen(socket_fd, backlog);

    return(rc);
}

// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Connect()
//
// PURPOSE:
//
// INPUT:	NONE.
//
// RETURN:	NONE.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Connect(void)
{
    int len = sizeof(server_address);

    int rc = connect(new_socket_fd,
		     (struct sockaddr *) &server_address,
		     len);
    return(rc);
}



// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::CloseServer()
//
// PURPOSE:	Close Socket.
// ----------------------------------------------------------------------------
int TCP_SOCKET::CloseServer(void)
{
    shutdown(socket_fd,0);
    int rc = close(socket_fd);

    return(rc);
}




// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::Close()
//
// PURPOSE:	Close Socket.
// ----------------------------------------------------------------------------
int TCP_SOCKET::Close(void)
{
    shutdown(socket_fd,0);

    int rc = close(socket_fd);

    return(rc);
}


// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::TCP_SOCKET()
//
// PURPOSE:	TCP_SOCKET class constructor (SERVER).
// ----------------------------------------------------------------------------
TCP_SOCKET::TCP_SOCKET(int port, int number_connect)
{
    backlog = number_connect;

    socket_fd =socket(AF_INET, SOCK_STREAM, 0);
  
    memset((char *)&server_address, 1, sizeof(server_address));

    server_address.sin_family	   =AF_INET;
    server_address.sin_addr.s_addr =htonl(INADDR_ANY);
    server_address.sin_port	   =htons(port);

    bind(socket_fd,
	 (struct sockaddr *)&server_address,
	 sizeof(server_address));    

    return;
}

// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::TCP_SOCKET()
//
// PURPOSE:	TCP_SOCKET class constructor (CLIENT).
// ----------------------------------------------------------------------------
TCP_SOCKET::TCP_SOCKET(int port, char *address)
{
    memset((char *)&server_address, 1, sizeof(server_address));

    server_address.sin_family	   =AF_INET;
    server_address.sin_addr.s_addr =htonl(INADDR_ANY);
    server_address.sin_port	   =htons(port);

    new_socket_fd =socket(AF_INET, SOCK_STREAM, 0);

    return;
}


// ----------------------------------------------------------------------------
// FUNCTION TCP_SOCKET::TCP_SOCKET()
//
// PURPOSE:	TCP_SOCKET class destructor.
// ----------------------------------------------------------------------------
TCP_SOCKET::~TCP_SOCKET(void)
{

}








