// SEM.H	Semaphore class definition
// ----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill     Initial pthread port.
// 01/10/95 M. Gill	Solaris 2.5 port.
// 01/10/85 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef SEM_H
#define SEM_H

#include<pthread.h>

typedef struct{

    pthread_mutex_t lock;
    pthread_cond_t  cond;
    unsigned int    count;
    unsigned int    threads_waiting;
}sem_t;


class SEM
{

protected:

    sem_t *semaphore;

public:

    SEM(int count);
    ~SEM(void);
    int Signal(void);
    int Wait(void);
};
#endif










