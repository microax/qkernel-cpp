// RESOURCE.H	 Definition file for RESOURCE_POOL class.
//-----------------------------------------------------------------------------
// History:
// --------
// 01/02/95 M. Gill	Initial creation based from rsc.h .
//----------------------------------------------------------------------------
#ifndef RESOURCE_H
#define RESOURCE_H

#include <pthread.h>
#include <time.h>
#include <qktypes.h>
#include <queue.h>
    
//---------------------------
// RESOURCE NODE
//---------------------------
typedef struct tagRESOURCE_NODE
{
    LPNODE	link;			// Allocation link
    int		resource_id;		// Resource Type
    pthread_t	thread_id;		// Thread I.D. of owner
    time_t 	time_created;		// Timestamp of Created node.
    time_t 	time_allocate;		// Timestamp of Allocate() 
    time_t 	time_free;		// timestamp of Free()
    void 	*text;			// Allocated structure data.

}RESOURCE_NODE;


class RESOURCE_POOL
{

protected:

    QUEUE rqueue;
    int   rcount;
    int   rused;

public:

    RESOURCE_POOL(int num, int size);
    ~RESOURCE_POOL(void);
    RESOURCE_NODE *Allocate();
    void   Free(RESOURCE_NODE *);
    pthread_t  GetThreadId(RESOURCE_NODE *);
    time_t GetTimeCreated(RESOURCE_NODE *);
    time_t GetTimeAllocated(RESOURCE_NODE *);
    time_t GetTimeFreed(RESOURCE_NODE *);
    int    GetResourceUsed(void);
    int    GetResourceFree(void);
};
#endif






