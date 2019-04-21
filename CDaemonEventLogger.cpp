// CDaemonEventLogger.cpp    Daemon event logger class.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/07/96 M. Gill	1) Removed "Diaper" support.
//			2) Add support for tty output. This eliminates
//			   the need for EVL.
//
// 04/05/96 M. Gill	   Changed MessageHandler() to use PostHoldQueue()
//			   This will allow other application tasks 
//			   to view post processed messages. 
//
// 03/01/95 M. Gill	Initial creation (based from EVL.cpp)
//-----------------------------------------------------------------------------
#include <CDaemonEventLogger.h>
#include <CDaemon.h>
#include <stdio.h>
#include <string.h>


CDaemonEventLogger::CDaemonEventLogger(const char *logname)
{
	mLogName = strdup(logname);

}

CDaemonEventLogger::~CDaemonEventLogger(void)
{
	delete mLogName;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemonEventLogger::MessageHandler
//
// PURPOSE:	Logs events if logging level is 10 or greater. Event are
//			sent to the normal Diaper based logging location
//
// INPUT:	Resource node that contains the message
//
//
// RETURN:	result code
// ---------------------------------------------------------------------------
int CDaemonEventLogger::MessageHandler(RESOURCE_NODE *t)
{
    FILE *fd;

    if (gDaemon -> GetLoggingThreshold() == CDAEMON_MAX_LOG_THRESHOLD)
    {
	FormatString(t);		// Format message
	PostHoldQueue(t);		// Save just in case...

	if(mLogName[0] !=0)
	{
	    if((fd = fopen(mLogName,"a")) != NULL)
	    {
 	    	fprintf(fd,(char *)t->text);
		fprintf(fd,"\n");
  	    	fclose(fd);
	    }
	}
	else
	{
	    printf((char *)t->text);
	    printf("\n");
	}
    }
    return(0);
}






