// QUEUE.CPP            QUEUE operations (enqueue & dequeue)
// ----------------------------------------------------------------------------
// History:
// --------
// 01/11/96 M. Gill	   Solaris 2.5 port.
// 04/26/95 M. Gill        Initial C++ (QCYCLE) port.
// 01/10/85 M. Gill        Initial creation.
// ----------------------------------------------------------------------------
//
#include <qkernel.h>

static pthread_mutex_t queue_cs;
static int csInit = 0;



// ----------------------------------------------------------------------------
// FUNCTION QUEUE::Enqueue()
//
// PURPOSE: place item onto FIFO QUEUE (signle link list).
//
// INPUT:  1) Pointer to QUEUE type.
//	   2) Pointer to item to be placed.
//-----------------------------------------------------------------------------
int QUEUE::Enqueue(void **item)
{
    pthread_mutex_lock(&queue_cs); // EnterCriticalSection()

    *item = (void *)0;
    if(queue.head)
	*queue.tail = item;	// link item onto end of queue
    else
	queue.head = item;	// queue was empty
    queue.tail= item;		// update tail pointer

    pthread_mutex_unlock(&queue_cs); // LeaveCriticalSection()
    return( SUCCESS );
}


// ----------------------------------------------------------------------------
// FUNCTION QUEUE::Dequeue()
//
// PURPOSE:  remove item from FIFO QUEUE (signle linked list).
//	     Update queue head pointer.
//
// INPUT:   NONE.
//
// RETURN:  pointer to item at head (void *).
// ----------------------------------------------------------------------------
LPNODE QUEUE::Dequeue(void)
{
    LPNODE item;

    pthread_mutex_lock(&queue_cs);	// EnterCriticalSection()
    if((item = queue.head))
    {
	if(!(queue.head = (LPNODE) *queue.head))
	    queue.tail = (LPNODE)0;
    }
    pthread_mutex_unlock(&queue_cs);	// LeaveCriticalSection()
    return(item);
}


// ----------------------------------------------------------------------------
// FUNCTION QUEUE::QUEUE()
//
// PURPOSE: initialize QUEUE class.
// ----------------------------------------------------------------------------
QUEUE::QUEUE(void)
{

    if(csInit == 0)
    {
        csInit = 1;
        pthread_mutex_init(&queue_cs,0); // InitCriticalSection()
    }

    queue.head = (LPNODE)0;
    queue.tail = (LPNODE)0;
}


// ----------------------------------------------------------------------------
// FUNCTION QUEUE::~QUEUE()
//
// PURPOSE: QUEUE class destructor.
// ----------------------------------------------------------------------------
QUEUE::~QUEUE(void)
{

}











