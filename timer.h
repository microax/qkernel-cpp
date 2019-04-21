// TIMER.H	Timer class definition.
//-----------------------------------------------------------------------------
// History:
// --------
// 04/26/95 M. Gill	Initial C++ (QCYCLE) port.
// 01/10/85 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef TIMER_H
#define TIMER_H

#include <sem.h>
#include <pthread.h>

const int K1SEC=1;

typedef struct tagTIMER_NODE
{
    struct tagTIMER_NODE *forward;
    struct tagTIMER_NODE *backward;
    int (*func)(void *);
    void *arg;
    int  ticks;
    SEM  *semaphore;

}TIMER_NODE;


class TIMER
{

protected:

    TIMER_NODE     timer;		// Timer node
    pthread_t      tmr_id;		// thread id
    pthread_attr_t tmr_attr;            // thread attributes
    size_t         tmr_stack_size;      // thread stack size

public:

    TIMER();
    ~TIMER();
    void Start(int ticks, int (*func)(void *), void *arg);
    void Stop(void);
    void Sleep(int ticks);
    static int  Main(TIMER *);
    static void Remove(TIMER_NODE *t);
    static void Insert(TIMER_NODE *t);
    static int timeout(void *p);
};

#endif












