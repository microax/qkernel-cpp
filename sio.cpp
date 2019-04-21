// SIO.CPP	Serial I/O object.
// ----------------------------------------------------------------------------
// History:
// --------
// 07/15/96 M. Gill	Support new single instance FSM.
// 04/04/95 M. Gill     Initial creation.
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sio.h>

//----------------------
// External References
//----------------------

extern MOS *error_log;
extern MOS *event_log;

//-------------------
// Local Stuff
//-------------------

//-------------------------------------
// This makes the casting less painfull
//-------------------------------------
#define ACTION (ACTION_FUNC) &SIO::


static STATE_TABLE  SioTable[]=
{
/*
CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
SIO_INIT,	SIO_OK,			SIO_INIT,	ACTION Init,
SIO_INIT,	SIO_OPEN_CMD,		SIO_OPEN,	ACTION ReadDevice,

SIO_OPEN,	SIO_READ_CMD,		SIO_OPEN,	ACTION ReadDevice,
SIO_OPEN,	SIO_CLOSE_CMD,		SIO_CLOSED,	ACTION sioClose,
SIO_OPEN,	SIO_OPEN_CMD,		SIO_OPEN,	ACTION DoNothing,

SIO_CLOSED,	SIO_OPEN_CMD,		SIO_OPEN,	ACTION ReadDevice,
SIO_CLOSED,	SIO_READ_CMD,		SIO_CLOSED,	ACTION DoNothing,
SIO_CLOSED,	SIO_CLOSE_CMD,		SIO_CLOSED,	ACTION DoNothing,

NUMBER_OF_STATES,0,0, ACTION DoNothing
};



//---------------------------------------------------------------------------
// Function: Init()
// 
// Purpose:     Invoked on INIT_OK, Post OPEN_CMD.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int SIO::Init(int arg1, void *n)
{

    FSM_PORT *port = appPort;

    char reg_string[40];

    //-----------------------------------
    // Register with the logging systems
    //-----------------------------------
    sprintf(reg_string,"SIO %d",user->appPort->channel);
    event_log->Register(port->thread_id, reg_string);
    error_log->Register(port->thread_id, reg_string);


    event_log->SendMessage("Init OK");

    //-------------------------------------
    // Let's get this show on the road...
    //-------------------------------------
    Send(SIO_OPEN_CMD,0,0);
    return(0);
}




//---------------------------------------------------------------------------
// Function: ReadDevice()
// 
// Purpose:     Read from device and send to upper layer task via Send().
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int SIO::ReadDevice(int arg1, void *n)
{
    int i,rc;
    FSM_PORT *port= appPort;
			
    if(close_request !=0)		//--------------------------
	return(0);			// Do nothing, if the agent
					// is closing the device
    rc =fgetc(stream);

    if(rc < 0)
    {					//-------------------------
	Send(SIO_CLOSE_CMD,0,0);	// Close things up.
					// and send device error.
					//-------------------------
	user->Send(SIO_DEVICE_ERROR,0,0);
    }
    else
    {
    	user->Send(SIO_CHAR_EVENT, rc,0);
	Send(SIO_READ_CMD,0,0);		// Post another read command.
    }
    return(0);
}




//---------------------------------------------------------------------------
// Function: sioClose()
// 
// Purpose:     Invoked on SIO_CLOSE_CMD 
//		Currently this does nothing because the state
//		change basicly closes the object.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int SIO::sioClose(int arg1, void *n)
{

    event_log->SendMessage("sioClose() called...");

    return(0);
}



//---------------------------------------------------------------------------
// Function: DoNothing()
// 
// Purpose:     Do Nothing.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int SIO::DoNothing(int arg1, void *n)
{
    return(0);
}



//---------------------------------------------------------------------------
// Function: SIO::Close()
// 
// Purpose:     Close SIO.
//
// INPUTS: 	None.
//
// RETURN:      None.
//---------------------------------------------------------------------------
void SIO::Close(void)
{

    //----------------------------
    // Close SIO object.
    //----------------------------
    close_request =1;
    Send(SIO_CLOSE_CMD,0,0);
    return;
}



//---------------------------------------------------------------------------
// Function: SIO::Open()
// 
// Purpose:     Open SIO.
//
// INPUTS:      1) File descriptor to read from.
//		2) Pointer to attached FSM.
//
// RETURN:      None.
//---------------------------------------------------------------------------
void SIO::Open( FILE *fd, FSM *fsm)
{
    user 	 = fsm;    
    stream       = fd;

    //-------------------------------
    // Startup our state machine.
    //-------------------------------
    if(sio_started ==0)
    {
	Start(SioTable,0);
	sio_started++;
    }
    //----------------------------
    // Start reading...
    //----------------------------
    close_request =0;
    Send(SIO_OPEN_CMD,0,0);
    return;
}

//---------------------------------------------------------------------------
// Function: SIO::SIO()
// 
// Purpose:     Class constructor.
//
// INPUTS:
//
//
// RETURN:      N/A
//---------------------------------------------------------------------------
SIO::SIO()
	: FSM()
{
    sio_started =0;
    return;
}



























