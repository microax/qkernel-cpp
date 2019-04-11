// QUEUE.H	 Definition file for QUEUE class.
//-----------------------------------------------------------------------------
// History:
// --------
// 12/29/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef QUEUE_H
#define QUEUE_H

#include <qktypes.h>

typedef struct
{
    LPNODE head;
    LPNODE tail;

}QUEUE_STRUCT;

class QUEUE
{

protected:

    QUEUE_STRUCT queue;

public:

    QUEUE(void);
    ~QUEUE(void);
    int Enqueue(LPNODE);
    LPNODE Dequeue(void);
};
#endif






