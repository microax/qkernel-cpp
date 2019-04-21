// TCPSERVER.H	 Definition file for TCP_SERVER class.
//-----------------------------------------------------------------------------
// History:
// --------
// 09/17/96 M. Gill     Add ua_lock.
// 07/15/96 M. Gill     Support new single instance FSM.
// 01/15/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <qkernel.h>
#include <tcpsocket.h>

const int TCP_INIT		=0;
const int TCP_OPEN		=1;
const int TCP_CLOSE		=2;

const int TCP_INIT_OK		=INIT_OK;
const int TCP_OPEN_CMD		=10;
const int TCP_CLOSE_CMD		=11;
const int TCP_ACCEPT_CMD	=12;
const int TCP_ACCEPT_OK		=13;
const int TCP_ACCEPT_FAIL	=14;
const int TCP_NO_UA_ERR		=15;
const int TCP_OPEN_OK		=16;
const int TCP_OPEN_FAIL		=17;

const int TCP_CONNECTED		=3;

typedef struct tagUSER_AGENT
{
	FSM	*fsm;
    int task_id;
    int socket;

}USER_AGENT;

class TCP_SERVER: public FSM
{
public:
	int tcp_init_ok(int arg1, void *n);
	int tcp_open(int arg1, void *n);
	int tcp_close(int arg1, void *n);
	int tcp_open_ok(int arg1, void *n);
	int tcp_open_fail(int arg1, void *n);
	int tcp_accept(int arg1, void *n);
	int tcp_accept_ok(int arg1, void *n);
	int tcp_accept_fail(int arg1, void *n);
	int tcp_no_ua_err(int arg1, void *n);
	int tcp_null(int arg1, void *n);

public:    

    TCP_SOCKET *server_socket;
    QUEUE *ready_list;
    RESOURCE_POOL *connection_pool;
    int port_number; 
    int ua_errflg;
    SEM *ua_lock;

    TCP_SERVER(int port_number, int number_connect);
    ~TCP_SERVER(void);
    Release(RESOURCE_NODE *);
    Register(FSM *fsm);
};

#endif









