// MOS.H	 Definition file for MOS class.
//-----------------------------------------------------------------------------
// History:
// --------
// 09/29/96 M. Gill	Changed scope of methods to public virtual.
// 07/21/96 M. Gill     Initial pthread port.
// 04/05/96 M. Gill	Add methods GetSenderName(), and GetMessageTime().
// 01/03/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef MOS_H
#define MOS_H

#include <time.h>
#include <sem.h>
#include <pthread.h>
#include <qktypes.h>
#include <resource.h>


const int REG_LIST_SIZE	    =1024; // Max. number of threads that can register.
const int MOS_REG_NAME_SIZE =32;   // Max. string length for class name.
const int MOS_STACK_SIZE    =4096;

//----------------------------
// MOS registration entry
//----------------------------
typedef struct tagMOS_REG
{
    pthread_t thread_id;
    char text[MOS_REG_NAME_SIZE];

}MOS_REG;


class MOS
{

protected:

    int            thread_handle;
    pthread_t      thread_id;
    size_t         thread_stack_size;
    pthread_attr_t thread_attr;

    RESOURCE_POOL *message_resource;
    RESOURCE_POOL *registration_resource;
    RESOURCE_POOL *holding_resource;

    QUEUE *registration_list;
    QUEUE *holding_queue;
    QUEUE *message_queue;

    SEM   *message_semaphore;
    char  *szFormatString;
    int   message_queue_size;
    int   node_size;
    int   message_type;

public:

    virtual int MessageHandler(RESOURCE_NODE *t);
    virtual int Init(void);
    virtual int GetSenderName(RESOURCE_NODE *, char *);
    virtual int GetMessageTime(RESOURCE_NODE *, char *);
    virtual int GetMessageText( void *s);
    virtual int GetMessageNode(RESOURCE_NODE *N);
    virtual int FormatString(RESOURCE_NODE *n);
    virtual int PostHoldQueue(RESOURCE_NODE *n);

    MOS(void);
    ~MOS(void);
    static int Main(MOS *t);
    int SendMessage(void *msg);
    int Register( pthread_t Threadid, char *szClassname);
    void Start(int msg_depth, int msg_size, int msg_type);

};

#endif











