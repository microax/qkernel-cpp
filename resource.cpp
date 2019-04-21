// RESOURCE.CPP		RESOURCE_POOL class 
// ----------------------------------------------------------------------------
// History:
// --------
// 01/02/95 M. Gill        Initial creation (based from bfr.c)
// ----------------------------------------------------------------------------
//
#include <resource.h>


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::Allocate()
//
// PURPOSE: Allocate resource.
//
// INPUT:  None.
//
// RETURN: Pointer to allocated RESOURCE_NODE.
//-----------------------------------------------------------------------------
RESOURCE_NODE *RESOURCE_POOL::Allocate(void)
{
    RESOURCE_NODE *rn;
    void *n = (void *)rqueue.Dequeue();
  
    rn =(RESOURCE_NODE *)n;

    if(rn != NULL)
    {
	rn->thread_id= pthread_self();	// GetCurrentThreadId()
	time(&rn->time_allocate);	// Timestamp.
	rcount--;			// Decrement free count.
 	rused++;			// Increment Used count.
    }
    return(rn);				// and...
}




// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::Free()
//
// PURPOSE:  Free an allocated resource.
//
// INPUT:   Pointer to RESOURCE_NODE.
//
// RETURN:  None.
// ----------------------------------------------------------------------------
void RESOURCE_POOL::Free(RESOURCE_NODE *rn)
{
    LPNODE n = (LPNODE)rn;

    time(&rn->time_free);	// Timestamp.
    rqueue.Enqueue(n);		// Return Resource to Q.
    rcount++;			// Increment item count.
    rused--;			// Decrement used count.
    return;
}



// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetResourceFree()
//
// PURPOSE:  Return the number of available resources.
//
// INPUT:    None.
//
// RETURN:   Count.
// ----------------------------------------------------------------------------
int RESOURCE_POOL::GetResourceFree(void)
{
    return(rcount);	// That's all.
}


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetResourceUsed()
//
// PURPOSE:  Return the number of Used resources.
//
// INPUT:    None.
//
// RETURN:   Count.
// ----------------------------------------------------------------------------
int RESOURCE_POOL::GetResourceUsed(void)
{
    return(rused);	// That's all.
}


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetTimeCreated()
//
// PURPOSE: Return the time of when a RESOURCE_NODE is created.
//
// INPUT:    Pointer to RESOURCE_NODE.
//
// RETURN:   time_t.
// ----------------------------------------------------------------------------
time_t RESOURCE_POOL::GetTimeCreated(RESOURCE_NODE *n)
{
    return(n->time_created);
}


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetTimeFreed()
//
// PURPOSE: Return the time of when a RESOURCE_NODE is freed.
//
// INPUT:    Pointer to RESOURCE_NODE.
//
// RETURN:   time_t.
// ----------------------------------------------------------------------------
time_t RESOURCE_POOL::GetTimeFreed(RESOURCE_NODE *n)
{
    return(n->time_free);
}


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetTimeAllocated()
//
// PURPOSE: Return the time of when a RESOURCE_NODE is allocated.
//
// INPUT:    Pointer to RESOURCE_NODE.
//
// RETURN:   time_t.
// ----------------------------------------------------------------------------
time_t RESOURCE_POOL::GetTimeAllocated(RESOURCE_NODE *n)
{
    return(n->time_allocate);
}


// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::GetThreadId()
//
// PURPOSE: Return the thread_id of the owner.
//
// INPUT:    Pointer to RESOURCE_NODE.
//
// RETURN:   DWORD.
// ----------------------------------------------------------------------------
pthread_t RESOURCE_POOL::GetThreadId(RESOURCE_NODE *n)
{
    return(n->thread_id);
}

// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::~RESOURCE_POOL()
//
// PURPOSE: RESOURCE_POOL object destructor.
// ----------------------------------------------------------------------------
RESOURCE_POOL::~RESOURCE_POOL(void)
{


}



// ----------------------------------------------------------------------------
// FUNCTION RESOURCE_POOL::RESOURCE_POOL()
//
// PURPOSE: Initialize RESOURCE_POOL.
// ----------------------------------------------------------------------------
RESOURCE_POOL::RESOURCE_POOL(int num, int size)
{
    RESOURCE_NODE *node = new RESOURCE_NODE[num];

    rused=rcount=0;
    
    for(int i=0; i < num;i++, node++)
    {
	node->text = new char[size];	// Allocate buffer space for node.
	time(&node->time_created);	// Timestamp of Created. 
	rqueue.Enqueue((LPNODE)node);	// Attach to node to list
	rcount++;			// Update available nodes counter.
    }
}









