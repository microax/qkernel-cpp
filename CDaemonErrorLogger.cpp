// CDaemonErrorLogger.cpp    Daemon Error logger class.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/08/96 M. Gill	1) Removed "Diaper" support.
//			2) Add support for  tty outputt. This eliminates
//			   the need for EVL.
//
// 04/05/96 M. Gill	1) Add Time stamp to MessageHandler() method.
//			2) Changed MessageHandler() to use PostHoldQueue()
//			   This will allow other application tasks 
//			   to view post processed messages. 
//
// 03/01/95 M. Gill	Initial creation. (based from EVL.cpp)
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <CDaemonErrorLogger.h>
#include <CDaemon.h>

CDaemonErrorLogger::CDaemonErrorLogger(const char *logname)
{
    mLogName = strdup(logname);
}

CDaemonErrorLogger::~CDaemonErrorLogger(void)
{
    delete mLogName;
}

int	
CDaemonErrorLogger::SendMessage(void *msg)
{
    // should never be called
    return 0;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemonErrorLogger::SendMessage
//
// PURPOSE:	Log an error or warning.
//
// INPUT:	a severity value and the message
//
//
// RETURN:	result code
// ---------------------------------------------------------------------------
int CDaemonErrorLogger::SendMessage(int severity,char *message)
{
    DaemonError_t de;

    de.mSeverity = severity;
    strncpy(de.mMessage,message,CDAEMONERRORLOGGER_MAX_MESSAGE_SIZE);
    de.mMessage[CDAEMONERRORLOGGER_MAX_MESSAGE_SIZE] = '\0';
    return MOS::SendMessage(&de);
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemonErrorLogger::MessageHandler
//
// PURPOSE:	Logs events if logging level is less than threshold. Event are
//			sent to the normal Diaper based error location
//
// INPUT:	Resource node that contains the message
//
//
// RETURN:	result code
// ---------------------------------------------------------------------------
int CDaemonErrorLogger::MessageHandler(RESOURCE_NODE *t)
{
    FILE *fd;

    DaemonError_t *de;
    char time_string[40];
    char sender_name[40];
    char tmp[200]="",tmp2[CDAEMONERRORLOGGER_MAX_MESSAGE_SIZE * 3];

    de = (DaemonError_t *)t->text;

    if (de -> mSeverity <= gDaemon -> GetLoggingThreshold())
    {
	GetSenderName(t, sender_name);
	GetMessageTime(t, time_string);
	PostHoldQueue(t);		// Save just in case...
    	sprintf(tmp2,
		"%s %s Severity%d %s\n",
		time_string, sender_name, de->mSeverity, de->mMessage);
 
	if(mLogName[0] != 0)
	{
	    if((fd = fopen(mLogName,"a")) != NULL)
	    {
 	    	fprintf(fd,tmp2);
  	    	fclose(fd);
	    }
	}
	else
	{
	    printf(tmp2);
	}
    }
    return(0);
}








