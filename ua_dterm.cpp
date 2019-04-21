// UA_DTERM.CPP		DTERM (Dumb terminal) User Agent.
// ----------------------------------------------------------------------------
// History:
// --------
// 04/23/97 M. Gill   	Fixed bug where file descriptors opened via
//			fdopen() were not released (.ie fclose())
// 07/15/96 M. Gill	Support new single instance FSM.
// 04/18/95 M. Gill     Initial creation.
// ----------------------------------------------------------------------------
#include <ua_dterm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

//----------------------
// External References
//----------------------

extern MOS *error_log;
extern MOS *event_log;



//-------------------------------------
// This makes the casting less painfull
//-------------------------------------
#define ACTION (ACTION_FUNC) &DTERM_AGENT::


static STATE_TABLE  DtermTable[]=
{
/*
CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
DTERM_INIT,	SYS_INIT_OK,		DTERM_READY,	ACTION Open,
DTERM_INIT,	SYS_RESTART_OK,		DTERM_READY,	ACTION DoNothing,

DTERM_READY,	TCP_CONNECTED,		DTERM_READY,	ACTION Connected,
DTERM_READY,	DTERM_LOGIN_CMD,	DTERM_LOGIN, 	ACTION Login,
DTERM_READY,	SYS_EVENT_FAIL,		DTERM_READY,	ACTION DoNothing,
DTERM_READY,	SIO_CHAR_EVENT,		DTERM_READY,	ACTION DoNothing,

DTERM_LOGIN,	SIO_CHAR_EVENT,		DTERM_LOGIN,	ACTION GetLoginChar,
DTERM_LOGIN,	DTERM_LOGIN_OK,		DTERM_USESSION,	ACTION OpenSession,
DTERM_LOGIN,	DTERM_PASSWORD_CMD,	DTERM_PASSW,	ACTION Password,
DTERM_LOGIN,	DTERM_TIMEOUT_EVENT,	DTERM_LOGIN,	ACTION UserTimeout,
DTERM_LOGIN,	DTERM_CLOSE_CMD,	DTERM_READY,	ACTION CloseSession,
DTERM_LOGIN,	SIO_DEVICE_ERROR,	DTERM_READY,	ACTION CloseSession,

DTERM_PASSW,	SIO_CHAR_EVENT,		DTERM_PASSW,	ACTION GetPasswChar,
DTERM_PASSW,	DTERM_PASSW_OK,		DTERM_USESSION,	ACTION OpenSession,
DTERM_PASSW,	DTERM_LOGIN_CMD,	DTERM_LOGIN,	ACTION Login,
DTERM_PASSW,	DTERM_TIMEOUT_EVENT,	DTERM_PASSW,	ACTION UserTimeout,
DTERM_PASSW,	DTERM_CLOSE_CMD,	DTERM_READY,	ACTION CloseSession,
DTERM_PASSW,	SIO_DEVICE_ERROR,	DTERM_READY,	ACTION CloseSession,

DTERM_USESSION,	SYS_RESTART_OK,		DTERM_READY,	ACTION DoNothing,


NUMBER_OF_STATES,0,0, ACTION DoNothing
};


//--------------------------------------------
// This is a stub menu table which does
// nothing, but Close the session.
//--------------------------------------------
static STATE_TABLE  DtermMenuTable[]=
{
/*
CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
0,		SYS_RESTART_OK,		0,		ACTION CloseSession,

NUMBER_OF_STATES,0,0, ACTION DoNothing
};

//----------------------------------------------------------------------------
// Function:	LoginTimeout()
//
// Purpose:	TIMER object Callback for Login timeout.
//----------------------------------------------------------------------------
static int LoginTimeout(void *n)
{
    DTERM_AGENT *t = (DTERM_AGENT *)n;

    t->Send(DTERM_TIMEOUT_EVENT, 0,0);
    return(0);
}


//---------------------------------------------------------------------------
// Function: Open()
// 
// Purpose:     Register UA with TCP_SERVER.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::Open(int arg1, void *n)
{
    char reg_string[40];
    FSM_PORT *port = appPort;
    DTERM_CONTEXT   *context;

    port->app_ctrl = new DTERM_CONTEXT;
    context        = (DTERM_CONTEXT *)port->app_ctrl;

    sio_server=context->sio = new SIO;
	
    //-----------------------------------
    // Register with the logging systems
    //-----------------------------------
    sprintf(reg_string,"DUMB TERMINAL %d",port->channel);
    event_log->Register(port->thread_id, reg_string);
    error_log->Register(port->thread_id, reg_string);

    event_log->SendMessage(" Open called...");

    tcp_server->Register(this);		// register with TCP server

    return(0);
}



//---------------------------------------------------------------------------
// Function: Connected()
// 
// Purpose:     Invoked on socket connection.
//		'n' is a pointer to a RESOURCE_NODE that holds
//	 	a USER_AGENT struct.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::Connected(int arg1, void *n)
{
    int rc;
    char buff[40];
    FSM_PORT *port = appPort;

    //----------------------------------------------
    // First, let's get this anoying casting thing
    // out of the way...
    //----------------------------------------------
    RESOURCE_NODE *p           = (RESOURCE_NODE *)n;
    USER_AGENT    *user        = (USER_AGENT  *)p->text;
    DTERM_CONTEXT   *context   = (DTERM_CONTEXT *)port->app_ctrl;
    context->ua_resource       = p;
    tcp_socket=context->socket = user->socket;

    input_stream = fdopen(tcp_socket,"r");
    output_stream= fdopen(tcp_socket,"w");
    setbuf(input_stream,0);
    setbuf(output_stream,0);

    //---------------------------------------------
    // OK, for now on, 'port->app_ctrl' will
    // reference everything that we need.
    //---------------------------------------------

    event_log->SendMessage("Connected called...");

    //-------------------------------------
    // Proceed to authenticate user...
    //-------------------------------------
    Send(DTERM_LOGIN_CMD,0,0);

    //--------------------------------------
    // Start listening to SIO characters
    //--------------------------------------
    context->sio->Open(input_stream, this);

    return(0);
}



//---------------------------------------------------------------------------
// Function: Login
// 
// Purpose:     
//		Display Login Prompt 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::Login(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;

    event_log->SendMessage("Login() called...");

    context->UserName[0] =0;
    context->Password[0] =0;
    context->uindex =0;
    context->pindex =0;

    //    port->timer = new TIMER;

//    fputc(5, output_stream);		// CTRL-E turn off line mode
    fprintf(output_stream,"Login:");

//    port->timer->Start(20, LoginTimeout, this); 
    return(0);
}


//---------------------------------------------------------------------------
// Function: GetLoginChar
// 
// Purpose:     
//		Process SIO_CHAR_EVENT  
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::GetLoginChar(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;
    char sio_char;

    event_log->SendMessage("GetLoginChar() called...");

    context->UserName[context->uindex++] = (char)arg1;
    sio_char = (char)arg1;

    fputc(sio_char,output_stream);

    if((sio_char == ASCII_LF) || (context->uindex == LOGIN_STRING_SIZE))
    {
	Send(DTERM_PASSWORD_CMD,0,0);
    }
    return(0);
}


//---------------------------------------------------------------------------
// Function: Password()
// 
// Purpose:     
//		Display Password Prompt 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::Password(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;

    event_log->SendMessage("Password() called...");

    fprintf(output_stream,"Password:");

    return(0);
}


//---------------------------------------------------------------------------
// Function: GetPasswChar
// 
// Purpose:     
//		Process SIO_CHAR_EVENT  
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::GetPasswChar(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;
    char sio_char;

    event_log->SendMessage("GetPasswChar() called...");

    context->Password[context->pindex++] = (char)arg1;

//    fputc(output_strean,'*');		// Echo '*'character.

    if((arg1 == ASCII_LF) || (context->pindex == LOGIN_STRING_SIZE))
    {
      //	port->timer->Stop();	// Kill timer.
	Send(DTERM_PASSW_OK,0,0);
    }
    return(0);
}


//---------------------------------------------------------------------------
// Function: OpenSession
// 
// Purpose:     
//		Start session by attaching a menu. 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::OpenSession(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
 
    event_log->SendMessage("OpenSession() called...");

    SetMainMenu();		// Setup Main menu.
    Attach(MenuTable);		// Attach menu table.
    Send(INIT_OK, 0,0);		// Kick off.

    return(0);
}


//---------------------------------------------------------------------------
// Function: UserTimeout
// 
// Purpose:     
//		Could not authenticate user before timeot. 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::UserTimeout(int arg1, void *n)
{
    FSM_PORT      *port    = appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;

    event_log->SendMessage("UserTimeout() called...");

    write(context->socket,"Timeout closing connection....", 30);

    Send(DTERM_CLOSE_CMD, 0,0);		// Your outta time...
    return(0);
}


//---------------------------------------------------------------------------
// Function: CloseSession
// 
// Purpose:     
//		Close/Release everything, and restart. 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int DTERM_AGENT::CloseSession(int arg1, void *n)
{
    FSM_PORT      *port    =appPort;
    DTERM_CONTEXT *context = (DTERM_CONTEXT *)port->app_ctrl;

    event_log->SendMessage("CloseSession() called...");

    context->sio->Close();			// Close sio server
    Attach(StateTable);				// Set Default State Table
    Send(SYS_RESTART_OK, 0,0);			// Restart State machine.
    fclose(output_stream);			// Release FD's... 
    fclose(input_stream);			// and associated buffers
    tcp_server->Release(context->ua_resource);	// Close socket

    return(0);
}




//------------------------------------------
// SetMainMenu()
// Purpose:	Set main menu State Table
//-----------------------------------------
void DTERM_AGENT::SetMainMenu(void)
{
    MenuTable = DtermMenuTable;
    return;
}

//---------------------------------------
// SetStateTable()
// Purpose:	Set State Table
//--------------------------------------
void DTERM_AGENT::InitStateTable(void)
{
    StateTable = DtermTable;
    return;
}
