// sigcatch.CPP		Trap signals 
// ----------------------------------------------------------------------------
// History:
// --------
// 01/24/95 M. Gill       Initial creation.
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <qkernel.h>
#include <sigcatch.h>
#include <evl.h>

extern MOS*	event_log;

// ---------------------------------------------------------------------------
// FUNCTION:	WaitForever()
//
// PURPOSE:	Called from main() after everything is started.
//		This function will NEVER return.
//
// INPUT:	NONE.
//
// RETURN:	N/A
// ---------------------------------------------------------------------------
void WaitForever(void)
{
    SEM  *main_terminate = new SEM(0);

    for(;;)
    {
    	main_terminate->Wait();
    }
    return;
}




// ---------------------------------------------------------------------------
// FUNCTION:	sig_catch()
//
// PURPOSE:	This static function is used as the default
//		signal handler.
//
//		NOTE:
//		-----
//		Add dufault signal handling in the switch statement.
//
// INPUT:	1) Signal number.
//
// RETURN:	NONE
// ---------------------------------------------------------------------------
static void sig_catch(int signo)
{
    char estring[100];

    switch(signo)
    {
	case SIGTERM:	// Someone sent kill signal.
	exit(0);	// Hopefully this is a gracefull termination.
	break;
	
	default:

	sprintf(estring,"Signal %d Caught",signo);
    	event_log->SendMessage(estring);
	break;
    }
    signal(signo, sig_catch);
    return;
}


// ---------------------------------------------------------------------------
// FUNCTION:	SignalCatch()
//
// PURPOSE:	Grab all the signals.
//		Use sig_catch() to handle.
//
// INPUT:	NONE
//
// RETURN:	SUCCESS/FAIL status
// ---------------------------------------------------------------------------
int SignalCatch(void )
{
    signal(SIGABRT, sig_catch);		signal(SIGALRM, sig_catch);
    signal(SIGBUS,  sig_catch);		signal(SIGCHLD, sig_catch);
    signal(SIGCONT, sig_catch);		signal(SIGEMT,  sig_catch);
    signal(SIGFPE,  sig_catch);		signal(SIGHUP,  sig_catch);
    signal(SIGILL,  sig_catch);		signal(SIGIO,   sig_catch);
    signal(SIGIOT,  sig_catch);		signal(SIGKILL, sig_catch);
    signal(SIGPIPE, sig_catch);		signal(SIGPOLL, sig_catch);
    signal(SIGPROF, sig_catch);		signal(SIGPWR,  sig_catch);
    signal(SIGQUIT, sig_catch);		signal(SIGSEGV, sig_catch);
    signal(SIGSTOP, sig_catch);		signal(SIGSYS,  sig_catch);
    signal(SIGTERM, sig_catch);		signal(SIGTRAP, sig_catch);
    signal(SIGTSTP, sig_catch);		signal(SIGTTIN, sig_catch);
    signal(SIGTTOU, sig_catch);		signal(SIGURG,  sig_catch);
    signal(SIGUSR1, sig_catch);		signal(SIGUSR1, sig_catch);
    signal(SIGUSR2, sig_catch);		signal(SIGUSR1, sig_catch);
    signal(SIGVTALRM, sig_catch);	signal(SIGWINCH,  sig_catch);
    signal(SIGXCPU,   sig_catch);	signal(SIGXFSZ,   sig_catch);

    return(0);
}



























