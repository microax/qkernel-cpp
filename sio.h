// SIO.H	 Definition file for SIO
//-----------------------------------------------------------------------------
// History:
// --------
// 07/15/96 M. Gill     Support new single instance FSM.
// 04/04/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef SIO_H
#define SIO_H

#include <qkernel.h>
#include <stdio.h>

//----------------------------
// SIO STATES
//----------------------------
const int SIO_INIT      	=0;
const int SIO_OPEN	    	=1;
const int SIO_CLOSED		=2;

//----------------------------
// SIO EVENTS
//----------------------------
const int SIO_OK       		=INIT_OK;
const int SIO_CHAR_EVENT	=SYS_SIO_CHAR;
const int SIO_DEVICE_ERROR	=SYS_IO_ERROR;

const int SIO_OPEN_CMD 		=10;
const int SIO_CLOSE_CMD      	=11;
const int SIO_READ_CMD		=12;

class SIO: public FSM
{

protected:

	int sio_started;
	int close_request;
	FILE *stream;
	char read_buffer[2048];
	FSM *user;
	int user_channel;

public:
	//---------------------------------------
   	// Public Methods
	//---------------------------------------

	SIO(void);
	void Open(FILE *, FSM *);
	void Close(void);

	//---------------------------------------
	// Action routines
	//---------------------------------------
	int Init(int arg1, void *n);
	int ReadDevice(int arg1, void *n);
	int sioClose(int arg1, void *n);
	int DoNothing(int arg1, void *n);
};


#endif









