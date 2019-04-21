 // UA_DIAG.CPP		Diag monitor.
// ----------------------------------------------------------------------------
// History:
// --------
// 07/15/95 M. Gill     Initial creation.
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <qkernel.h>
#include <ua_diag.h>

//----------------------
// External References
//----------------------

extern MOS *error_log;
extern MOS *event_log;

//-------------------------------------
// This makes the casting less painfull
//-------------------------------------
#define ACTION (ACTION_FUNC) &UA_DIAG::


static STATE_TABLE DiagMenuTable[]=
{
/*
CURRENT_ST	COMMAND/EVENT		NEXT_ST		ACTION
----------	-------------		-------		------
*/
DIAG_INIT,	DIAG_OK,		DIAG_MAIN_MENU,	ACTION MainMenu,

DIAG_MAIN_MENU,	SIO_CHAR_EVENT,		DIAG_MAIN_MENU,	ACTION MMGetChar,
DIAG_MAIN_MENU, SIO_DEVICE_ERROR,	DIAG_INIT,	ACTION CloseSession,
DIAG_MAIN_MENU,	DIAG_CLOSE_CMD,		DIAG_INIT,	ACTION CloseSession,

NUMBER_OF_STATES,0,0, ACTION DoNothing
};



//---------------------------------------------------------------------------
// Function: MainMenu
// 
// Purpose:     
//		Main Menu 
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int UA_DIAG::MainMenu(int arg1, void *n)
{
    event_log->SendMessage("MainMenu() called...");

    fprintf(output_stream,"\nMain Menu...\n");

    return(0);
}


//---------------------------------------------------------------------------
// Function: MMGetChar
// 
// Purpose:     
//		Process SIO_CHAR_EVENT  
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int UA_DIAG::MMGetChar(int arg1, void *n)
{
    char sio_char;

    event_log->SendMessage("MMGetChar() called...");

    sio_char = (char)arg1;

    fputc(sio_char, output_stream);	// Echo character.

    if(sio_char == ASCII_ESC)
    {
	Send(DIAG_CLOSE_CMD,0,0);
    }
    return(0);
}



//------------------------------------------
// SetMainMenu()
// Purpose:	Set main menu State Table
//-----------------------------------------
void UA_DIAG::SetMainMenu(void)
{
    event_log->SendMessage("SetMainMenu() Called...");

    MenuTable = DiagMenuTable;
    return;
}
