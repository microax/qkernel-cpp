// CDAEMON.CPP    Daemon class.
//-----------------------------------------------------------------------------
// History:
// --------
// 10/08/97 M. Gill     1) Write Process ID to xxx.pid file in logger directory.
//                      2) exit(0) , when SIGTERM caught *** Temporary hack ***
// 09/07/97 M. Gill     Changed Logger names to xxx.log from xxx.err
// 07/01/96 M. Gill	1) Removed "Diaper" code. Future updates should 
//			   include configuration management (.ie CDaemonConfig)
//			   to replace the "diaper" functions.
//			2) Changed API to make more general purpose.
//
// 03/01/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <CDaemon.h>
#include <CDaemonEventLogger.h>
#include <CDaemonErrorLogger.h>


CDaemon			*gDaemon = NULL;
CDaemonEventLogger	*gApplicationEventLogger = NULL;
CDaemonErrorLogger	*gApplicationErrorLogger = NULL;
CDaemonEventLogger	*event_log = NULL;
CDaemonErrorLogger	*error_log = NULL;


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::SetLoggingThreshold
//
// PURPOSE:	Sets the error logging level for the entire application
//
// INPUT:	Threshold value.
// 		if the error level IS LESS THAN OR EQUAL TO THRESHOLD then it
// 		is reported.
//
// RETURN:	none
// ---------------------------------------------------------------------------
void
CDaemon::SetLoggingThreshold(int l)
{
    mLoggingSemaphore->Wait(); 	// EnterCriticalSection

    if (l >= CDAEMON_MIN_LOG_THRESHOLD && l <= CDAEMON_MAX_LOG_THRESHOLD)
	mLoggingThreshold = l;

    mLoggingSemaphore->Signal();	// LeaveCriticalSection
}

// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::GetLoggingThreshold
//
// PURPOSE:	Get the error logging level for the entire application
//
// INPUT:	None
//
// RETURN:	Threshold value
// ---------------------------------------------------------------------------
int CDaemon::GetLoggingThreshold(void)
{
    return mLoggingThreshold;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::SetLoggingDirectory
//
// PURPOSE:	Sets the event/error log directory path.
//
// INPUT:	Directory Path name.
//
// RETURN:	none
// ---------------------------------------------------------------------------
void
CDaemon::SetLoggingDirectory(const char *dname)
{
    if( dname[0] == 0)
    {
	mLoggingDirectory="";
    }
    else
    {
    	mLoggingDirectory = strdup(dname);
    }
    return;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::CreateAndStartMainThreads
//
// PURPOSE:	This is sort of the main portion of the "daemon" area
//		Create your threads here and let 'em go
//
// INPUT:	none
//
// RETURN:	none
// ---------------------------------------------------------------------------
void
CDaemon::CreateAndStartMainThreads(void)
{

}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::CreateAndStartErrorLogger
//
// PURPOSE:	You can override this to create an error logger of your own
//		design. Remember that loggers should not hold a file open for
//		any long period of time.
//
// INPUT:	none
//
// RETURN:	A subclass of CDaemonErrorLogger
// ---------------------------------------------------------------------------
CDaemonErrorLogger *CDaemon::CreateAndStartErrorLogger(void)
{
    char logname[80];

    if(mLoggingDirectory[0] != 0)
    {
	sprintf(logname, "%s%s.log",mLoggingDirectory, mAppCode);
    }
    else
    {
	logname[0] =0;
    }
    CDaemonErrorLogger *m = new CDaemonErrorLogger(logname);

    m->Start(2048,200,0);	// allow 2k 200 byte messages in queue
    return(m);
}

// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::CreateAndStartEventLogger
//
// PURPOSE:	You can override this to create an event logger of your own
//		design.
//
// INPUT:	none
//
// RETURN:	A subclass of CDaemonEventLogger
// ---------------------------------------------------------------------------
CDaemonEventLogger *CDaemon::CreateAndStartEventLogger(void)
{
    char logname[80];

    if(mLoggingDirectory[0] != 0)
    {
	sprintf(logname, "%s%s.log",mLoggingDirectory, mAppCode);
    }
    else
    {
	logname[0] =0;
    }
    CDaemonEventLogger *m = new CDaemonEventLogger(logname);
  
    m ->Start(2048,sizeof(DaemonError_t),0);	// allow 2k messages in queue
    return(m);
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::WaitTillDone
//
// PURPOSE:	Allows the main application to wait until signaled done
//
// INPUT:	none
//
// RETURN:	none
// ---------------------------------------------------------------------------
void CDaemon::WaitTillDone(void)
{
    for(;;)
    {
    	mMainSemaphore->Wait();
    }
    return;
}

// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::ReleaseMainWait
//
// PURPOSE:	Allows the main application to terminate
//
// INPUT:	none
//
// RETURN:	none
// ---------------------------------------------------------------------------
void CDaemon::ReleaseMainWait(void)
{
    mMainSemaphore -> Signal();
}


// ---------------------------------------------------------------------------
// FUNCTION:	daemon_sig_catch
//
// PURPOSE:	Handles OS signals
//
// INPUT:	signal number
//
// RETURN:	none
// ---------------------------------------------------------------------------
static void daemon_sig_catch(int signo)
{
char estring[100];

    switch(signo)
    {
	case SIGTERM:	// Someone sent kill signal.

	  //	gDaemon -> ReleaseMainWait();

	exit(0); // *** This is a temporary hack
	break;

	default:

	sprintf(estring,"Signal %d Caught",signo);
    	gApplicationEventLogger->SendMessage(estring);
	break;

    }
    signal(signo, daemon_sig_catch);
    return;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::SetupSignalCatcher
//
// PURPOSE:	Sets up catches for the signals
//
// INPUT:	none
//
// RETURN:	none
// ---------------------------------------------------------------------------
void CDaemon::SetupSignalCatcher()
{
  signal(SIGABRT, daemon_sig_catch);	
    //signal(SIGALRM, daemon_sig_catch);
    //signal(SIGBUS,  daemon_sig_catch);	
    signal(SIGCHLD, daemon_sig_catch);
    signal(SIGCONT, daemon_sig_catch);
    //signal(SIGFPE,  daemon_sig_catch);
    signal(SIGHUP,  daemon_sig_catch);
    signal(SIGILL,  daemon_sig_catch);	signal(SIGIO,   daemon_sig_catch);
    signal(SIGIOT,  daemon_sig_catch);	//signal(SIGKILL, daemon_sig_catch);
    signal(SIGPIPE, daemon_sig_catch);	//signal(SIGPOLL, daemon_sig_catch);
    signal(SIGPROF, daemon_sig_catch);	//signal(SIGPWR,  daemon_sig_catch);
    signal(SIGQUIT, daemon_sig_catch);	//signal(SIGSEGV, daemon_sig_catch);
    signal(SIGSTOP, daemon_sig_catch);
    signal(SIGTERM, daemon_sig_catch);	signal(SIGTRAP, daemon_sig_catch);
    signal(SIGTSTP, daemon_sig_catch);	signal(SIGTTIN, daemon_sig_catch);
    signal(SIGTTOU, daemon_sig_catch);	signal(SIGURG,  daemon_sig_catch);
    signal(SIGUSR1, daemon_sig_catch);	signal(SIGUSR1, daemon_sig_catch);
    signal(SIGUSR2, daemon_sig_catch);	signal(SIGUSR1, daemon_sig_catch);
    //    signal(SIGVTALRM, daemon_sig_catch);signal(SIGWINCH,  daemon_sig_catch);
    signal(SIGXCPU,   daemon_sig_catch);signal(SIGXFSZ,   daemon_sig_catch);
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::Run
//
// PURPOSE:	The main routine
//
// INPUT:	argc and argv from the command line, and the number of threads
//		you think you might want to support
//
// RETURN:	an exit code for main
// ---------------------------------------------------------------------------
int
CDaemon::Run(int argc,char *argv[],int approx_num_threads)
{
    pid_t pid;
  
    if ((pid = fork()) < 0)    	
	return -1;  
    else if (pid != 0)    
    	return 0;   	

    gDaemon = this;

    //---------------------------------------------------------
    // Now the process becomes the session leader of a new
    // session, the process group leader of a new process
    // group, and is liberated from any controlling terminal.
    //---------------------------------------------------------
    setsid();

    // get config info before we forget our home directory
    // gDiaper = CreateDiaper(mAppCode);
    // gDiaper -> ParseCommandLine(argc,argv);

    chdir("/");    // forget the home directory

    //---------------------------------------------------------------------
    // Eliminate any chance of inherited file permission snafus. 
    // But in actuality, you'll want to set this to fit the task at hand.
    //---------------------------------------------------------------------
    umask(0);
    //--------------------------------------------------
    // Close any open file descriptors, incl 0, 1 & 2.
    //--------------------------------------------------
    close(0);
    close(1);
    close(2);
//-------------------------------------------
//     Not supported in POSIX threads
//     --------------------------------       
//    thr_setconcurrency(approx_num_threads);

    error_log = gApplicationErrorLogger = CreateAndStartErrorLogger();
    event_log = gApplicationEventLogger = CreateAndStartEventLogger();

    SetupSignalCatcher();
    CreateAndStartMainThreads();
    WaitTillDone();
    return 0;
}

// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::RunForeground
//
// PURPOSE:	This is the main routine for running as a non-daemon.
//		(handy for testing)
//
// INPUT:	argc and argv from the command line, and the number of threads
//		you think you might want to support
//
// RETURN:	an exit code for main
// ---------------------------------------------------------------------------
int CDaemon::RunForeground(int argc,char *argv[],int approx_num_threads)
{
    gDaemon = this;

    int  pid;
    FILE *fd;
    char PidFile[100];

    sprintf(PidFile,"%s%s.pid",mLoggingDirectory,mAppCode);   // Set PidFile path

    pid =getpid();	  			// Get Process ID
    if((fd=fopen(PidFile,"w+")) != NULL)	// Open Pid file in log directory
    {
        fprintf(fd,"%d\n",pid);			// Write pid
        fclose(fd);				// Close Pid file
    }
//-------------------------------------------
//     Not supported in POSIX threads
//     --------------------------------
//    thr_setconcurrency(approx_num_threads);

    error_log = gApplicationErrorLogger = CreateAndStartErrorLogger();
    event_log = gApplicationEventLogger = CreateAndStartEventLogger();

    SetupSignalCatcher();
    CreateAndStartMainThreads();
    WaitTillDone();
    return 0;
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::CDaemon
//
// PURPOSE:	Constructor
//
// INPUT:	The config file appcode.
//
//
// RETURN:	N/A
// ---------------------------------------------------------------------------
CDaemon::CDaemon(const char *appcode, const char *logdir)
{
    SetLoggingDirectory(logdir);    
    mLoggingThreshold = 1;
    mAppCode = NULL;
    mMainSemaphore = NULL;
    mLoggingSemaphore = NULL;
    mAppCode = strdup(appcode);
    mMainSemaphore = new SEM(0);
    mLoggingSemaphore = new SEM(1);
}


// ---------------------------------------------------------------------------
// FUNCTION:	CDaemon::~CDaemon
//
// PURPOSE:	Destructor
//
//
// RETURN:	N/A
// ---------------------------------------------------------------------------
CDaemon::~CDaemon()
{
    if (mAppCode) 
	delete mAppCode;
    if (mMainSemaphore) 
	delete mMainSemaphore;
    if (mLoggingSemaphore) 
	delete mLoggingSemaphore;
}
 







