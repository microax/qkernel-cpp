// SEM.CPP	Semaphore class.
// ----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill 	Initial pthread port.
// 01/10/95 M. Gill	Solaris 2.5 port.
// 01/10/85 M. Gill 	Initial creation.
// ----------------------------------------------------------------------------
#include <sem.h>


// ----------------------------------------------------------------------------
// FUNCTION SEM::Wait()
//
// PURPOSE:	Task wait (semaphore 'p' operation).
//
// PROCESS:	1) Decrement semaphore count. 
//		2) If count becomes negative, enqueue TCB onto
//		   semaphore queue and, force a task switch.
//
// INPUT:	NONE.
//
// RETURN:	NONE.
// ----------------------------------------------------------------------------
int SEM::Wait(void)
{
    pthread_mutex_lock(&semaphore->lock);
    while(semaphore->count ==0)
    {
	semaphore->threads_waiting++;
	pthread_cond_wait(&semaphore->cond, &semaphore->lock);
 	semaphore->threads_waiting--;
    }
    semaphore->count--;
    pthread_mutex_unlock(&semaphore->lock);

    return(0);
}


// ----------------------------------------------------------------------------
// FUNCTION SEM::Signal()
//
// PURPOSE:	signal a task (semaphore 'v' operation).
// ----------------------------------------------------------------------------
int SEM::Signal(void)
{
    pthread_mutex_lock(&semaphore->lock);

    if(semaphore->threads_waiting)
    {
	pthread_cond_signal(&semaphore->cond);
    }

    semaphore->count++;

    pthread_mutex_unlock(&semaphore->lock);

    return(0);
}


// ----------------------------------------------------------------------------
// FUNCTION SEM::SEM()
//
// PURPOSE:	SEM class constructor.
// ----------------------------------------------------------------------------
SEM::SEM(int count)
{
    semaphore 	     = new sem_t;		// Get new semaphore type
    semaphore->count = count;			// Set initial count
    pthread_mutex_init(&semaphore->lock,0);	// Init. mutex with defaults
    pthread_cond_init (&semaphore->cond,0);	// Init. cond. with defaults

    semaphore->threads_waiting =0;
    return; 
}

// ----------------------------------------------------------------------------
// FUNCTION SEM::SEM()
//
// PURPOSE:	SEM class destructor.
// ----------------------------------------------------------------------------
SEM::~SEM(void)
{

}



