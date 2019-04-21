// MOS.CPP    Message Oriented Server class.
//-----------------------------------------------------------------------------
// History:
// --------
// 12/24/99 M. Gill     Fixed Y2K bug in GetMessageTime().
// 07/21/96 M. Gill 	Initial pthread port.
// 04/05/96 M. Gill	Add methods GetSenderName(), and GetMessageTime().
// 04/03/96 M. Gill	Fixed time_t to ascii conversion in FormatString(). 
// 01/02/95 M. Gill     Initial creation.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <qkernel.h>
#include <mos.h>

// ---------------------------------------------------------------------------
// FUNCTION:	MOS::Main()
//
// PURPOSE:	Main entry point for a MOS thread.
//
// INPUT:	Null pointer.
//
// RETURN:	N/A
// ---------------------------------------------------------------------------
int MOS::Main(MOS *t)
{
    RESOURCE_NODE *n;

    for(;;)
    {
	t->message_semaphore->Wait();	// Wait for message,

	n = (RESOURCE_NODE *)t->message_queue->Dequeue();

	t->MessageHandler(n);		// Invoke handler.
	t->message_resource->Free(n);	// Release mesage node.
    }
	return 0;
}


// ---------------------------------------------------------------------------
// FUNCTION:	MOS::MessageHandler()
//
// PURPOSE:	Virtual Method to handle message records.
//
// INPUT:	Pointer to RESOURCE_NODE.
//
// RETURN:	None.
// ---------------------------------------------------------------------------
int MOS::MessageHandler(RESOURCE_NODE *t)
{
    FormatString(t);		// Format message
    PostHoldQueue(t);		// Place on holding queue
    return(0);
}



// ---------------------------------------------------------------------------
// FUNCTION:    MOS::SendMessage()
//
// PURPOSE:    Method to handle message records.
//
// INPUT:      Pointer to record.
//
// RETURN:     SUCCESS = OK
//             FAIL    = Allocation error.
// ---------------------------------------------------------------------------
int MOS::SendMessage(void *msg)
{
    RESOURCE_NODE *n;
    char *s,*d;

    if((n = (RESOURCE_NODE *)message_resource->Allocate())==NULL )
	return(FAIL);

    s = (char *)msg;			// source 
    d = (char *)n->text;		// destination

    for(int i=0; i < node_size ;i++ )
	*d++ = *s++;			// Copy struct 
    message_queue->Enqueue((LPNODE)n);  // Post message.
    message_semaphore->Signal();	// Wake up MOS thread.
    return( SUCCESS );
}


//
// ---------------------------------------------------------------------------
// FUNCTION:  MOS::Register()
//
// PURPOSE:   Method to register a task with MOS.
//
// INPUT:     1) thread id.
//            2) class name.
//
// RETURN:    SUCCESS = OK
//            FAIL    = Allocation error.
// ---------------------------------------------------------------------------
int MOS::Register(pthread_t ThreadId, char *szClassName)
{
    RESOURCE_NODE *n;
    MOS_REG *t;

    if((n = registration_resource->Allocate() ) == 0 )
	return( FAIL );

    t = (MOS_REG *)n->text;
    t->text[MOS_REG_NAME_SIZE-1] =0;
    strcpy(t->text, szClassName);
    t->thread_id = ThreadId;

    registration_list->Enqueue((LPNODE) n);

    return( SUCCESS );
}


//
// ---------------------------------------------------------------------------
// FUNCTION:  MOS::GetSenderName()
//
// PURPOSE:   	Method to get the class name of the thread
//		that owns a RESOURCE_NODE.
//
// INPUT:     1) Pointer to RESOURCE_NODE.
//            2) Pointer to buffer to hold class name.
//
// RETURN:    SUCCESS = OK
//            FAIL    = No match found.
// ---------------------------------------------------------------------------
int MOS::GetSenderName(RESOURCE_NODE *p, char *szClassName)
{
    int i;
    RESOURCE_NODE *n;
    MOS_REG       *t;

    szClassName[0] =0;	// Init. to Null string

    for( i=0; i < REG_LIST_SIZE; i++ )
    {
	if( (n =(RESOURCE_NODE *)registration_list->Dequeue() ) != 0 )
	{
	    registration_list->Enqueue((LPNODE) n); // Put back at end of list.

	    t =(MOS_REG *)n->text;
	    if(p->thread_id == t->thread_id)
	    {
		//-----------------------------------
		// When the thread IDs match...
		// Copy the registered name.
		//-----------------------------------
		strcpy(szClassName, (char *)t->text);
		break;
	    }    
	}
	else
	{
	    break;
	}
    }
    return( SUCCESS );
}

//
// ---------------------------------------------------------------------------
// FUNCTION:  MOS::GetMessageTime()
//
// PURPOSE:   	Method to get a formatted time string 
//		for an allocated message node.
//
// INPUT:     1) Pointer to RESOURCE_NODE.
//            2) Pointer to buffer to hold time string.
//
// RETURN:    SUCCESS = OK
// ---------------------------------------------------------------------------
int MOS::GetMessageTime(RESOURCE_NODE *p, char *szTimeString)
{
    time_t        ttime;
    tm		  ltime;
    int nyear;

    ttime =message_resource->GetTimeAllocated(p);
    gmtime_r(&ttime, &ltime);

    if(ltime.tm_year > 99)
        nyear = 100 - ltime.tm_year;
    else
        nyear = ltime.tm_year;
    
    sprintf(szTimeString, "%02d%02d%02d-%02d:%02d:%02d",
			  nyear, ltime.tm_mon+1, ltime.tm_mday,
			  ltime.tm_hour, ltime.tm_min,   ltime.tm_sec);
    return( SUCCESS );
}


//
// ---------------------------------------------------------------------------
// FUNCTION:    MOS::FormatString()
//
// PURPOSE:	Format Message as string.
//
//		1) Find Registered class name.
//		2) concatinate class name to message buffer.
//
// INPUT:	Pointer to (MOS) resource.
//
// RETURN:	None.
// ---------------------------------------------------------------------------
int MOS::FormatString(RESOURCE_NODE *p)
{
    int		   i,j;
    RESOURCE_NODE *n;
    MOS_REG       *t;
    char	  time_string[100];
    time_t        ttime;
    tm		  ltime;
    int           nyear;

    for( i=0; i < REG_LIST_SIZE; i++ )
    {
	if( (n =(RESOURCE_NODE *)registration_list->Dequeue() ) != 0 )
	{
	    registration_list->Enqueue((LPNODE) n); // Put back at end of list.

	    t =(MOS_REG *)n->text;
	    if(p->thread_id == t->thread_id)
	    {
		//-----------------------------------
		// When the thread IDs match,
		// we will concatenate the strings
		//-----------------------------------
		strcpy(szFormatString, (char *)p->text);
		ttime =message_resource->GetTimeAllocated(p);
                gmtime_r(&ttime, &ltime);

                if(ltime.tm_year > 99)
                    nyear = 100 - ltime.tm_year;
                else
                    nyear = 100 - ltime.tm_year;		    
		sprintf(time_string, "%02d%02d%02d-%02d:%02d:%02d",
			nyear, ltime.tm_mon+1, ltime.tm_mday,
			ltime.tm_hour, ltime.tm_min,   ltime.tm_sec);
 		sprintf((char *)p->text,"%s %s %s"
			,time_string,t->text,szFormatString);
// 		sprintf((char *)p->text,"%s %s"
//			,t->text,szFormatString);
		break;
	    }
	}
	else
	{
	    break;
	}
    }
    return( SUCCESS );
}



// ---------------------------------------------------------------------------
// FUNCTION:   MOS::PostHoldQueue()
//
// PURPOSE:    Place processed mos node in Holding queue.
//
// INPUT:	Pointer to Resource.
//
// RETURN: None
// ---------------------------------------------------------------------------
int MOS::PostHoldQueue(RESOURCE_NODE *p)
{
    RESOURCE_NODE *t;
    char *p1, *p2;

    if( (t = holding_resource->Allocate() ) == 0 )
    {
	t= (RESOURCE_NODE *)holding_queue->Dequeue(); 	// Discard oldest
	holding_resource->Free(t);			// Create new
	t= holding_resource->Allocate();		// Allocate Node
    }
    p2 = (char *)t->text;			// Newly alloacted node.
    p1 = (char *)p->text;			// Node to copy
    for(int i=0; i < node_size; i++)
	*p2++ = *p1++;              		// Copy struct
    holding_queue->Enqueue((LPNODE)t);		// Place on Holding Queue
    return( SUCCESS );
}



// ---------------------------------------------------------------------------
// FUNCTION:  MOS::GetMessageText()
//
// PURPOSE:   Get text portion of message record from the holding queue.
//
// INPUT:     Pointer to buffer (MOS_NODE), where the record will be copied.
//
// RETURN:    FAIL    = Holding Queue is empty.
//            SUCCESS = Record copied.
// ---------------------------------------------------------------------------
int MOS::GetMessageText( void *s)
{
    RESOURCE_NODE *n;
    char *p1, *p2;

    if( (n = (RESOURCE_NODE *)holding_queue->Dequeue() ) == NULL )
	return( FAIL );

    p2 = (char *)s;			// Returned Message 
    p1 = (char *)n->text;		// Source node data

    for(int i=0; i < node_size ;i++ )
	*p2++ = *p1++;			// Copy struct 
    holding_resource->Free(n);		// Release resource
    return( SUCCESS );
}

// ---------------------------------------------------------------------------
// FUNCTION:  MOS::GetMessageNode()
//
// PURPOSE:   Get text portion of message record from the holding queue.
//
// INPUT:     Pointer to buffer (MOS_NODE), where the record will be copied.
//
// RETURN:    FAIL    = Holding Queue is empty.
//            SUCCESS = Record copied.
// ---------------------------------------------------------------------------
int MOS::GetMessageNode( RESOURCE_NODE *s)
{
    RESOURCE_NODE *n;
    char *p1, *p2;

    if( (n = (RESOURCE_NODE *)holding_queue->Dequeue() ) == NULL )
	return( FAIL );

    p2 = (char *)s;			// Returned Message 
    p1 = (char *)n;			// Source node data

    for(int i=0; i < sizeof(RESOURCE_NODE) ;i++ )
	*p2++ = *p1++;			// Copy struct 
    holding_resource->Free(n);		// Release resource
    return( SUCCESS );
}




// ---------------------------------------------------------------------------
// FUNCTION:	MOS::Init()
//
// PURPOSE:	Virtual initialization Method.
//
// INPUT:	None.
//
// RETURN:	None.
// ---------------------------------------------------------------------------
int MOS::Init(void)
{
    return(0);
}


// ---------------------------------------------------------------------------
// FUNCTION:  MOS::~MOS()
//
// PURPOSE:   MOS destructor.
// ---------------------------------------------------------------------------
MOS::~MOS()
{

}

// ---------------------------------------------------------------------------
// FUNCTION:  MOS::MOS()
//
// PURPOSE:   Create MOS task.
// ---------------------------------------------------------------------------
MOS::MOS()
{

}


// ---------------------------------------------------------------------------
// FUNCTION:  MOS::Start()
//
// PURPOSE:   Start MOS task.
// ---------------------------------------------------------------------------
void
MOS::Start(int msg_depth, int msg_size, int msg_type)
{
    message_queue_size = msg_depth;
    node_size          = msg_size;
    message_type       = msg_type;

    registration_resource = new RESOURCE_POOL(REG_LIST_SIZE,sizeof(MOS_REG));
    holding_resource	  = new RESOURCE_POOL(msg_depth, msg_size);
    message_resource 	  = new RESOURCE_POOL(msg_depth, msg_size);

    message_queue	  = new QUEUE;
    registration_list 	  = new QUEUE;
    holding_queue	  = new QUEUE;

    message_semaphore	  = new SEM(0);
    szFormatString	  = new char[msg_size];

    Register(pthread_self(),"MAIN");

    Init();	

    thread_stack_size = MOS_STACK_SIZE;
    pthread_attr_init(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, thread_stack_size);
    pthread_create(&thread_id,        		// Thread ID
                   &thread_attr,          	// Thread attributes
                   (void * (*)(void *))Main,    // Start function
                   (void *)this );              // Start function Argument

}
