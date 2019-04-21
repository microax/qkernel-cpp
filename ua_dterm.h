// UA_DTERM.H	 Definition file for Dumb Terminal  User Agent
//-----------------------------------------------------------------------------
// History:
// --------
// 07/15/96 M. Gill     Support new single instance FSM.
// 04/18/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef UA_DTERM_H
#define UA_DTERM_H

#include <qkernel.h>
#include <tcp_application.h>
#include <sio.h>
#include <stdio.h>

//----------------------------
// DTERM STATES
//----------------------------
const int DTERM_INIT      	=0;
const int DTERM_READY	    	=1;
const int DTERM_LOGIN		=2;
const int DTERM_PASSW		=3;
const int DTERM_USESSION        =4;

//----------------------------
// DTERM EVENTS
//----------------------------
const int DTERM_OK       	=INIT_OK;
const int DTERM_LOGIN_CMD 	=10;
const int DTERM_LOGIN_OK       	=11;
const int DTERM_PASSWORD_CMD 	=12;
const int DTERM_TIMEOUT_EVENT 	=13;
const int DTERM_CLOSE_CMD       =14;
const int DTERM_PASSW_OK	=15;


const int MAX_STRING_LEN	=256;
const int LOGIN_STRING_SIZE     =MAX_STRING_LEN;

static int LoginTimeout(void *);

typedef struct tagDTERM_CONTEXT
{
    RESOURCE_NODE	*ua_resource;
    int			socket;
    char                UserName[MAX_STRING_LEN];
    char                Password[MAX_STRING_LEN];
    int                 uindex;
    int                 pindex;
    SIO                *sio;

}DTERM_CONTEXT;

class DTERM_AGENT: public TCP_APPLICATION
{

public:

       STATE_TABLE *MenuTable;
       SIO         *sio_server;
       int         tcp_socket;
       FILE        *input_stream;
       FILE        *output_stream;

        void InitStateTable(void);
        virtual void SetMainMenu(void);

	int Open(int arg1, void *n);
	int Login(int arg1, void *n);
	int Password(int arg1, void *n);
	int UserTimeout(int arg1, void *n);
	int Connected(int arg1, void *n);
        int OpenSession(int arg1, void *n);
	int CloseSession(int arg1, void *n);
	int GetPasswChar(int arg1, void *n);
	int GetLoginChar(int arg1, void *n);
};

#endif









