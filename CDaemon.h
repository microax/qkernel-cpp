// CDAEMON.H    Daemon class definition.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill     Include qkernel.h
// 07/01/96 M. Gill     Update to remove "Diaper" etc.
// 03/01/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef _CDAEMON_H_
#define _CDAEMON_H_

#include <qkernel.h>

class MOS;
class SEM;
class CDaemonErrorLogger;
class CDaemonEventLogger;


#define	CDAEMON_MIN_LOG_THRESHOLD 0
#define CDAEMON_MAX_LOG_THRESHOLD 10

class CDaemon {
protected:

    char *mAppCode;
    char *mLoggingDirectory;
    SEM  *mMainSemaphore;
    SEM  *mLoggingSemaphore;
    int	  mLoggingThreshold;

    virtual void		CreateAndStartMainThreads(void) = 0;
    virtual CDaemonErrorLogger*	CreateAndStartErrorLogger(void);
    virtual CDaemonEventLogger*	CreateAndStartEventLogger(void);
    virtual void		WaitTillDone(void);
    virtual void		SetupSignalCatcher(void);

public:
    CDaemon(const char *appcode, const char *logdir);
    ~CDaemon();

    void	 SetLoggingThreshold(int l);
    int		 GetLoggingThreshold(void);
    void         SetLoggingDirectory(const char *);

    virtual int	 Run(int argc,char *argv[],int num_threads);
    virtual int	 RunForeground(int argc,char *argv[],int num_threads);
    virtual void ReleaseMainWait(void);
};

extern	CDaemon		      *gDaemon;
extern	CDaemonEventLogger    *gApplicationEventLogger;
extern	CDaemonErrorLogger    *gApplicationErrorLogger;

#endif
