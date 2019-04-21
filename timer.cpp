// TIMER.CPP    TIMER services class.
// ----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill   Initial pthread port.
// 04/26/95 M. Gill   Initial C++ (QCYCLE) port.
// 01/10/85 M. Gill   Initial creation.
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <timer.h>
#include <CDaemonEventLogger.h>
#include <CDaemonErrorLogger.h>

static int timer_started =0;	  // Timer services flag.
static TIMER_NODE timerq;	  // Timer Q
static pthread_mutex_t timerq_cs; // Critical section for timerq.

extern CDaemonEventLogger *event_log;
extern CDaemonErrorLogger *error_log;

//----------------------------------------------------------------------------
// FUNCTION TIMER::Main()
//
// PURPOSE: This method is invoked by the system timer interrupt (see notes:)
//	    to process TIMER nodes associated with application tasks.
//	    
//
// PROCESS: 1) Decrement count for each TIMER node in timerq.
//	       The timer queue is double pointer circular queue.
//	    2) Execute function specified in TIMER node when it
//	       expires.
//	    3) Remove expired nodes from queue.
// NOTES:
// ------
//		The following are the operating system specific methods
//		to invoke a timer interrupt:
//
//		MS-DOS	=>Vector 0x1c
//		UNIX	=>SIGALM via alarm()
//		WINDOWS =>TIMERPROC
//		WIN32   =>TIMERPROC
//
//		For any embedded system, the timer interrupt handler
//		should invoke TIMER::Main().
//-----------------------------------------------------------------------------
int TIMER::Main( TIMER *x)
{
    TIMER_NODE *t, *next;

    //-----------------------------------------
    // Register this thread with loggers...
    //-----------------------------------------
    event_log->Register(pthread_self(),"TIMER");
    error_log->Register(pthread_self(),"TIMER");

    //-------------------------------------------
    // Timers are searched ever second...
    //-------------------------------------------
    for(;;)
    {
    sleep(1);

    for(t = timerq.forward; t != &timerq; t = next )
    {
	next = t->forward;
	if(!(--t->ticks))
	{
	    Remove(t);		// Remove node from queue
	    (*t->func)(t->arg); // Call function
	    if(!next->forward)
		break;
	}
    }
    }
}


//-----------------------------------------------------------------------------
// FUNCTION TIMER::Remove()
//
// PURPOSE:  Remove TIMER node from list.
//
// INPUT:    Pointer to TIMER_NODE.
//-----------------------------------------------------------------------------
void TIMER::Remove(TIMER_NODE *t)
{
    pthread_mutex_lock(&timerq_cs);		// EnterCriticalSection
    if(t->forward != 0 && t->backward != 0)
    {
	t->forward->backward = t->backward;
	t->backward->forward = t->forward;
	t->forward = t->backward = 0;
    }
    pthread_mutex_unlock(&timerq_cs);		// LeaveCriticalSection
}



//-----------------------------------------------------------------------------
// FUNCTION TIMER::Insert()
//
// PURPOSE:  Insert TIMER node onto list.
//
// INPUT:    Pointer to TIMER_NODE.
//-----------------------------------------------------------------------------
void  TIMER::Insert(TIMER_NODE *t)
{
    pthread_mutex_lock(&timerq_cs);		// EnterCriticalSection
    if(!(t->forward && t->backward))
    {
	timerq.backward->forward = t;
	t->backward = timerq.backward;
	timerq.backward = t;
	t->forward = &timerq;
    }
    pthread_mutex_unlock(&timerq_cs);		// LeaveCriticalSection
}


//-----------------------------------------------------------------------------
// FUNCTION TIMER::Sleep()
//
// PURPOSE:   Sleep for n seconds.
//
// INPUT:
//	    Number of seconds/ticks.
//-----------------------------------------------------------------------------
void TIMER::Sleep(int ticks)
{
    Start(ticks, timeout, (void *)timer.semaphore);
    timer.semaphore->Wait();
    return;
}




//-----------------------------------------------------------------------------
// FUNCTION TIMER::Start()
//
// PURPOSE:   Start timer.
//
// INPUT:
//	    1) number of seconds/ticks.
//	    2) pointer to callback function.
//	    3) pointer to callback argument.
//-----------------------------------------------------------------------------
void TIMER::Start(int ticks, int (*func)(void *), void *arg)
{
    timer.ticks = ticks;   // ticks
    timer.func= func;
    timer.arg = arg;
    Insert(&timer);         // insert node onto timer queue
}



//-----------------------------------------------------------------------------
// FUNCTION TIMER::Stop()
//
// PURPOSE: Stop a timer.
//
// INPUT:   NONE
//-----------------------------------------------------------------------------
void TIMER::Stop(void)
{
    Remove(&timer);        // remove the timer from queue
}





//-----------------------------------------------------------------------------
// FUNCTION TIMER::timeout()
//
// PURPOSE: Callback used in Sleep method.
//-----------------------------------------------------------------------------
int TIMER::timeout(void *p)
{
   SEM *s = (SEM *)p;

    s->Signal();	// wakeup sleeping task.

    return(0);
}


//-----------------------------------------------------------------------------
// FUNCTION TIMER::~TIMER()
//
// PURPOSE: TIMER object destructor.
//-----------------------------------------------------------------------------
TIMER::~TIMER()
{

}



//-----------------------------------------------------------------------------
// FUNCTION TIMER::TIMER()
//
// PURPOSE: TIMER object constructor.
//-----------------------------------------------------------------------------
TIMER::TIMER()
{
    //-----------------------
    // Every instance gets 
    // a TIMER_NODE.
    //-----------------------
    timer.forward   = timer.backward = 0;	
    timer.semaphore = new SEM(0);
    //---------------------------
    // We will initialize the
    // timer service only once.
    //---------------------------
    if(timer_started ==0)
    {
	pthread_mutex_init(&timerq_cs,0); //Init. Critical section.
	timer_started++;		// Set flag
    	timerq.forward = &timerq;	// Initialize Timer Q
	timerq.backward= &timerq;	// 
	
	tmr_stack_size = 4096;
	pthread_attr_init(&tmr_attr);
	pthread_attr_setstacksize(&tmr_attr,tmr_stack_size);
        pthread_create(&tmr_id,      // Thread ID
		       &tmr_attr,    // Thread attributes
           (void * (*)(void *))Main, // Start function
           (void *)this );           // Start function Argument
    }
  
    return;
}
