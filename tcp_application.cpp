 // TCP_APPLICATION.CPP		TCP Application class.
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
#include <qkernel.h>
#include <tcp_application.h>

//----------------------
// External References
//----------------------

extern MOS *error_log;
extern MOS *event_log;

//-------------------------------------
// This makes the casting less painfull
//-------------------------------------
#define ACTION (ACTION_FUNC) &TCP_APPLICATION::


static STATE_TABLE tcp_stub_table[]=
{
/*
CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
0,0,0,		      					ACTION DoNothing,
NUMBER_OF_STATES,0,0, 					ACTION DoNothing
};



//---------------------------------------------------------------------------
// Function: TCP_APPLICATION::InitStateTable()
// 
// Purpose:     Set initial StateTable.
//
// RETURN:      None.
//---------------------------------------------------------------------------
void TCP_APPLICATION::InitStateTable(void)	
{
    StateTable = tcp_stub_table;

    return;
}



//---------------------------------------------------------------------------
// Function: TCP_APPLICATION::Start()
// 
// Purpose:     Method to kick off a TCP_APPLICATION.
//
// INPUTS:      1) Pointer to TCP_SERVER
//		2) Channel Number.
//
// RETURN:      N/A
//---------------------------------------------------------------------------
void TCP_APPLICATION::Start(TCP_SERVER *tcps , int chan)	
{
    tcp_server = tcps;

    //-------------------------------
    // Initial State Table.
    //-------------------------------
    InitStateTable();

    //-------------------------------
    // Startup our state machine.
    //-------------------------------
    FSM::Start(StateTable , chan);

    return;
}



























