// sysevents.cpp	System wide events for FSM
//----------------------------------------------------------------------------
// History:
// --------
// 04/23/95 M. GILL    Initial creation
//----------------------------------------------------------------------------
#ifndef	_SYSEVENTS_H_
#define	_SYSEVENTS_H_

const	int	SYS_INVALID_EVENT	=0;
const	int	SYS_INIT_OK		=1;
const	int	SYS_CLOSE               =2;
const	int	SYS_OPEN		=3;
const	int	SYS_IO_ERROR		=4;
const   int     SYS_RESTART_OK          =5;
const   int     SYS_EVENT_FAIL          =6;
const   int     SYS_SIO_CHAR            =7;
const	int	SYS_FIRST_USER_EVENT	=11;

// These are for backwards compatibilityZ
const	int	INIT_OK			=SYS_INIT_OK;
const	int	INVALID_EVENT	        =SYS_INVALID_EVENT;

#endif


