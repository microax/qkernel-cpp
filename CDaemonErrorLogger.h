// CDAEMON.H    Daemon class definition.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill     Include qkernel.h
// 07/01/96 M. Gill     Update to remove "Diaper" etc.
// 03/01/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef _CDAEMONERRORLOGGER_H_
#define _CDAEMONERRORLOGGER_H_

#include <qkernel.h>

#define	CDAEMONERRORLOGGER_MAX_MESSAGE_SIZE	255

typedef struct daemon_errorTAG { 
        int mSeverity;
	char mMessage[CDAEMONERRORLOGGER_MAX_MESSAGE_SIZE];

} DaemonError_t;


class	CDaemonErrorLogger: public MOS {
	char *mLogName;
private:
	int	SendMessage(void *msg);	// so it can't be accessed

public:
	int	SendMessage(int severity,char *message);
	CDaemonErrorLogger(const char *logname);
	~CDaemonErrorLogger(void);
	virtual int	MessageHandler(RESOURCE_NODE *t);

};


#endif

