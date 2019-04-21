// TCPSERVER.CPP	TCP/IP Server class.
// ----------------------------------------------------------------------------
// History:
// --------
// 09/17/96 M. Gill	Add SEM *ua_lock, to syncronize tcpserver to
//			registered user agents. 
// 07/15/96 M. Gill	Support new single instance FSM.
// 01/15/95 M. Gill     Initial creation.
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <qkernel.h>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <CDaemonEventLogger.h>

extern CDaemonEventLogger *event_log;

//-------------------------------------
// This makes the casting less painfull
//-------------------------------------
#define ACTION (ACTION_FUNC) &TCP_SERVER::

static STATE_TABLE  tcpTable[]=
{
/*
 CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
TCP_INIT,	TCP_INIT_OK,		TCP_CLOSE,	ACTION tcp_init_ok,

TCP_OPEN,	TCP_ACCEPT_CMD,		TCP_OPEN,	ACTION tcp_accept,
TCP_OPEN,	TCP_ACCEPT_OK,		TCP_OPEN,	ACTION tcp_accept_ok,
TCP_OPEN,	TCP_ACCEPT_FAIL,	TCP_OPEN,	ACTION tcp_accept_fail,
TCP_OPEN,	TCP_NO_UA_ERR,		TCP_OPEN,	ACTION tcp_no_ua_err,
TCP_OPEN,	TCP_CLOSE_CMD,		TCP_CLOSE,	ACTION tcp_close,

TCP_CLOSE,	TCP_OPEN_CMD,		TCP_CLOSE,	ACTION tcp_open,
TCP_CLOSE,	TCP_OPEN_FAIL,		TCP_CLOSE,	ACTION tcp_open_fail,
TCP_CLOSE,	TCP_OPEN_OK,		TCP_OPEN,	ACTION tcp_open_ok,

NUMBER_OF_STATES,0,0,ACTION tcp_null
};


//---------------------------------------------------------------------------
// Function: tcp_init_ok()
// 
// Purpose: 	Invoked when FSM starts.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_init_ok(int arg1, void *n)
{
    char reg_string[40];
    FSM_PORT *port =appPort;

    sprintf(reg_string,"TCP_SERVER P%d", port_number);
    event_log->Register(port->thread_id, reg_string);

    event_log->SendMessage("Initialization complete.");

    return(0);
}


//---------------------------------------------------------------------------
// Function: tcp_open()
// 
// Purpose:	Open TCP/IP Server.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_open(int arg1, void *n)
{
    event_log->SendMessage("Opening Socket connections...");

    if(!server_socket->Listen())
    {
	Send( TCP_OPEN_OK, 0,0);
    }
    else
    {
	Send( TCP_OPEN_FAIL, 0,0);
    }

    return(0);
}


//---------------------------------------------------------------------------
// Function: tcp_close()
// 
// Purpose: 	Close a TCP/IP Server.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_close(int arg1, void *n)
{
    event_log->SendMessage("Server Closed");
    return(0);
}



//---------------------------------------------------------------------------
// Function: tcp_open_ok()
// 
// Purpose: 	Invoked when OPEN_CMD is sucessfull.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_open_ok(int arg1, void *n)
{
    event_log->SendMessage("Ready to accept connections.");

    Send( TCP_ACCEPT_CMD,0,0);

    return(0);
}


//---------------------------------------------------------------------------
// Function: tcp_open_fail()
// 
// Purpose: 	Invoked when OPEN_CMD fails.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_open_fail(int arg1, void *n)
{
    event_log->SendMessage("Error*** Can't Start Server.");

    return(0);
}




//---------------------------------------------------------------------------
// Function: tcp_accept()
// 
// Purpose: 	1) Allocate a user agent to handle the server_socket.
//		2) Accept new socket connection.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_accept(int arg1, void *n)
{
    int new_socket;    
    RESOURCE_NODE   *p;
    USER_AGENT *user_agent; 

    ua_lock->Wait();    // Wait for UA's to become available. 

    if((p =(RESOURCE_NODE *)ready_list->Dequeue()) ==NULL )
    {
	Send(TCP_NO_UA_ERR,0,0);
	return(0);
    }

    event_log->SendMessage("Waiting for Connect...");

    if((new_socket =server_socket->Accept()) < 0)
    {
	Send( TCP_ACCEPT_FAIL,0,0);
	return(0);
    }

    event_log->SendMessage("Connection request...");

    //----------------------------------------
    // Setup UA handle, and socket.
    //----------------------------------------
    user_agent         = (USER_AGENT *)p->text;
    user_agent->socket = new_socket;

    //----------------------------------------
    // Notify User Agent that it's connected.
    //----------------------------------------
    user_agent->fsm->Send(TCP_CONNECTED, 0,p);

    //----------------------------------------
    // Notify self that We're OK...
    //----------------------------------------
    Send( TCP_ACCEPT_OK, 0,0);

    return(0);
}




//---------------------------------------------------------------------------
// Function: tcp_accept_ok()
// 
// Purpose: 	Invoked when tcp_accept() is OK.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_accept_ok(int arg1, void *n)
{
    event_log->SendMessage("Socket Connected...");

    Send(TCP_ACCEPT_CMD,0,0);
    return(0);
}



//---------------------------------------------------------------------------
// Function: tcp_accept_fail()
// 
// Purpose: 	Invoked when tcp_accept() fails.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_accept_fail(int arg1, void *n)
{
    event_log->SendMessage("Error** Accept Failed.");
    Send(TCP_CLOSE_CMD,0,0);

    return(0);
}



//---------------------------------------------------------------------------
// Function: tcp_no_ua_err()
// 
// Purpose: 	Invoked when there is no user agent to connect.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_no_ua_err(int arg1, void *n)
{
    FSM_PORT *port = appPort;
 
    if(ua_errflg ==0)
    {
	ua_errflg++;	// Report only once.
    	event_log->SendMessage("User Agent list empty.");
    }
//    port->timer->Sleep(10);		// Wait 10 seconds...
//    Send(TCP_ACCEPT_CMD,0,0);	// and try again.
    return(0);
}





//---------------------------------------------------------------------------
// Function: tcp_null()
// 
// Purpose: 	Do Nothing.
//
// INPUTS:	Action routine.
// RETURN:	N/A
//---------------------------------------------------------------------------
int 
TCP_SERVER::tcp_null(int arg1, void *n)
{
    event_log->SendMessage("tcp_null called...");

    return(0);
}


// ----------------------------------------------------------------------------
// FUNCTION TCP_SERVER::Register()
//
// PURPOSE: Register a user agent with TCP_SERVER.
// ----------------------------------------------------------------------------
int TCP_SERVER::Register(FSM *fsm)
{
    char event_string[40];
    USER_AGENT *user;
    RESOURCE_NODE *n = (RESOURCE_NODE *)connection_pool->Allocate(); 

    if(n != NULL)
    {
	user          = (USER_AGENT *)n->text;
	user->fsm     = fsm;			// UA object.
	user->task_id = fsm->appPort->channel;	// UA number
	ready_list->Enqueue((LPNODE)n);		// Make available
	ua_lock->Signal();

	sprintf(event_string,"User Agent %d registered",user->task_id);
	event_log->SendMessage(event_string);

	return(SUCCESS);
    }
    else
    {
	sprintf(event_string,"Error*** Can't Register UA %d",user->task_id);
	event_log->SendMessage(event_string);
	return(FAIL);
    }
}




//---------------------------------------------------------------------------
// Function: TCP_SERVER::Release()
// 
// Purpose: 	Close (accept) socket.
//
//---------------------------------------------------------------------------
int TCP_SERVER::Release(RESOURCE_NODE *p)
{
    USER_AGENT *user =(USER_AGENT *)p->text;

    close(user->socket);		// Close Socket
    ready_list->Enqueue((LPNODE)p);	// Make user agent available for use.
    ua_lock->Signal();			// 

    event_log->SendMessage("Connection released.");

    return(0);
}





// ----------------------------------------------------------------------------
// FUNCTION TCP_SERVER::~TCP_SERVER()
//
// PURPOSE: Destructor.
// ----------------------------------------------------------------------------

TCP_SERVER::~TCP_SERVER()
{
    return;
}


// ----------------------------------------------------------------------------
// FUNCTION TCP_SERVER::TCP_SERVER()
//
// PURPOSE: initialize TCP_SERVER class.
// ----------------------------------------------------------------------------
TCP_SERVER::TCP_SERVER(int portnumber, int number_connect )
{
    port_number     = portnumber;
    ua_errflg       = 0;
    ready_list	    = new QUEUE;
    server_socket   = new TCP_SOCKET(portnumber, number_connect);
    connection_pool = new RESOURCE_POOL(number_connect, sizeof(USER_AGENT));
    ua_lock         = new SEM(0);

    Start(tcpTable,0);
    Send (TCP_OPEN_CMD,0,0);
    return;
}





















