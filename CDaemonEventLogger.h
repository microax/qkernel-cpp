// CDAEMONeventlogger.H    Daemon event logger class definition.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill     Include qkernel.h
// 07/01/96 M. Gill     Update to remove "Diaper" etc.
// 03/01/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef _CDAEMONEVENTLOGGER_H_
#define _CDAEMONEVENTLOGGER_H_

#include <qkernel.h>

class	CDaemonEventLogger: public MOS {
	char *mLogName;

public:
	CDaemonEventLogger(const char *logname);
	~CDaemonEventLogger(void);
	virtual int	MessageHandler(RESOURCE_NODE *t);

};


#endif

