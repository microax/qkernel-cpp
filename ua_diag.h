// UA_DIAG.H	 Definition file for DIAG Monitor
//-----------------------------------------------------------------------------
// History:
// --------
// 07/15/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef UA_DIAG_H
#define UA_DIAG_H

#include <qkernel.h>
#include <ua_dterm.h>

//----------------------------
// DIAG STATES
//----------------------------
const int DIAG_INIT      	=0;
const int DIAG_MAIN_MENU	=1;

//----------------------------
// DIAG EVENTS
//----------------------------
const int DIAG_OK       	=INIT_OK;
const int DIAG_CLOSE_CMD       =14;


class UA_DIAG : public DTERM_AGENT
{

public:

	int MainMenu( int, void *);
	int MMGetChar(int, void *);

	void SetMainMenu(void);
};



#endif












