// FSM.CPP	Finite State Machine Application Class.
//----------------------------------------------------------------------------
// History:
// --------
// 07/14/96 M. Gill	New single instance FSM.
// 01/10/96 M. Gill	Initial Solaris port.
// 04/26/95 M. Gill	Initial C++ (QCYCLE) port.
// 01/10/85 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fsm.h>

extern MOS *event_log;


//----------------------------------------------------------------------------
// FUNCTION FSM::Main
//
// PURPOSE:	This is the execution body of the FSM task.
//		The FSM (Finite State Machine) task is started at 
//		initialization and never terminates.
//
// INPUT:	p = Pointer to application context (port) 
//		    attached to this task.
//
// PROCESS:	1) Wait for received message via FSM::RcvMsg()
//		2) Translate message to event/data.
//		3) Execute function associated with translated event.
//----------------------------------------------------------------------------
void *FSM::Main(FSM_PORT *app_context)
{
    STATE_TABLE_ENTRY *transition;
    FSM_PORT *p =app_context;
    FSM_MSG  *n;
    int current_state, event, data;
    void *d_ptr;

    for( ;; )
    {
	n = RcvMsg(p);				// Get a Message Node
	event = n->event;			// Save the Event
	data  = n->data;			// Save the Data
	d_ptr = n->d_ptr;			// ... & Data Pointer
	RelMsg(p, n);				// Release message node
	current_state = p->current_state;	// Save Current State
	p->event = event;
	transition = &p->State[current_state][event];
	p->current_state = transition->next_state;    // Set the Next State
	p->current_state_save = current_state;
	((p->mThisPointer) ->* (transition->action))(data, d_ptr);
    }

    return NULL; // Never reached
}


//----------------------------------------------------------------------------
// FUNCTION FSM::RcvMsg
//
// PURPOSE:	Retrieve message node from queue.
// INPUT:	Pointer to FSM_PORT.
// RETURN:	Pointer to message node.
//
// PROCESS:	1) Use SEM::wait(), to wait for message signal.
//		2) FSM::Send() will use SEM::signal() to wake up this task.
//----------------------------------------------------------------------------
FSM_MSG *FSM::RcvMsg(FSM_PORT *p)
{
    p->msg_sem->Wait();

    return((FSM_MSG *)p->msgque.Dequeue());
}


//----------------------------------------------------------------------------
// FUNCTION FSM::Send
//
// PURPOSE:	Place message node into queue.
//
// INPUT:
// 		1) event.
//		2) event data.
//		3) optional data pointer.
//
// PROCESS:	1) Allocate Message node.
//		2) Place message on FSM message queue.
//		3) signal FSM task.
//
// RETURN:	FAIL    => Can't allocate message node.
//		SUCCESS => OK
//----------------------------------------------------------------------------
int FSM::Send(int event, int data, void *d_ptr)
{
    FSM_PORT *p = appPort;
    FSM_MSG *msg;

    // -------------------------
    //  Allocate a message node
    // -------------------------
    if((msg =(FSM_MSG *)p->freeq.Dequeue()) == NULL)
      return( FAIL );

    msg->event = event;
    msg->data  = data;
    msg->d_ptr = d_ptr;

    p->msgque.Enqueue((LPNODE) msg);	// Send messgae
    p->msg_sem->Signal();		// wake up pending task
    return( SUCCESS );
}




//----------------------------------------------------------------------------
// FUNCTION FSM::RelMsg
//
// PURPOSE:	Release message node to free queue.
//
// INPUT:	1) pointer to FSM_PORT
//		2) Pointer to message node.
//----------------------------------------------------------------------------
void FSM::RelMsg(FSM_PORT *p, FSM_MSG *n)
{
    p->freeq.Enqueue((LPNODE)n);
}





//---------------------------------------------------------------------------
// FUNCTION FSM::InvalidEvent()
//
// PURPOSE: This method is called on an unexpected Event/Transition
//---------------------------------------------------------------------------
int FSM::InvalidEvent(int arg1, void *n)
{
    char message_buffer[80];
    FSM_PORT *port =appPort;

    sprintf(message_buffer,"Event #%d while in state %d", 
	    port->event, port->current_state_save);

    event_log->SendMessage(message_buffer);

    return(0);
}


//---------------------------------------------------------------------------
// Function: DoNothing()
// 
// Purpose:     Do Nothing.
//
// INPUTS:      Action routine.
// RETURN:      N/A
//---------------------------------------------------------------------------
int FSM::DoNothing(int arg1, void *n)
{
    return(0);
}


//----------------------------------------------------------------------------
// FUNCTION FSM::Attach()
//
// PURPOSE:	Attach state table to FSM task.
//
// INPUT:	1) pointer to STATE_TABLE
//		2) Pointer to application context.
//----------------------------------------------------------------------------
void FSM::Attach(STATE_TABLE *app_table)
{
    int i,j;
    STATE_TABLE        *st;
    STATE_TABLE_ENTRY  *ste;
    FSM_PORT           *port = appPort;

    //--------------------------------
    // Initialize default entry.
    //--------------------------------
    for( i = 0; i < NUMBER_OF_STATES; i++ )
    {
	for( j = 0; j < NUMBER_OF_EVENTS; j++ )
	{
	    port->State[i][j].action = (ACTION_FUNC) &FSM::InvalidEvent;
	    port->State[i][j].next_state = i;
	}
    }
    //-------------------------
    // Initialize application 
    // state table.
    //-------------------------- 
    for(st = app_table; st->current_state != NUMBER_OF_STATES; st++)
    {
	ste = &port->State[st->current_state][st->transition];
	ste->next_state = st->next_state;
	ste->action = st->action;
    }
    //------------------------
    // Set initial conditions.
    //------------------------
    port->current_state =0;
    port->next_state = 0;
    return;
}



//----------------------------------------------------------------------------
// FUNCTION FSM::Start()
//
// PURPOSE: Kickoff application.
//
// INPUT:	1) Pointer to Application State Table.
//		2) Channel Number.
//
// RETURN:	NONE
//----------------------------------------------------------------------------
void FSM::Start(STATE_TABLE *app_table, int channel_num)
{
    int i,j,rc;
    FSM_PORT *port;

    appPort = new FSM_PORT;	// Allocate FSM_PORT 

    port=appPort;
    Attach(app_table);		// Attach application state table
    port->channel = channel_num;// Assign channel number.
    port->msg_sem = new SEM(0);	// Create message semaphore.
    port->timer   = new TIMER;	// Create local timer.
    port->mThisPointer = this;

    //---------------------------
    // Create the message queue
    //---------------------------
    for(j= 0; j < FSM_QUE_SIZE; j++ )
    {
 	port->freeq.Enqueue((LPNODE)&port->msg_q[j] );
    }
    //----------------------------------
    // Create thread for execution body
    //----------------------------------
    port->thread_stack_size = FSM_STACK_SIZE;
    pthread_attr_init(&port->thread_attr);
    pthread_attr_setstacksize(
			      &port->thread_attr,
			      port->thread_stack_size);
    rc =pthread_create(&port->thread_id,	// Thread ID 
		   &port->thread_attr, 		// Thread attributes
		   (void * (*)(void *))Main, 	// Start function
		   (void *)port );		// Start function Argument
	Send(INIT_OK,0,0);			// Send initial event

    return;
}


//----------------------------------------------------------------------------
// FUNCTION FSM::~FSM()
//
// PURPOSE: Destructor.
//
// INPUT:	NONE
//
// RETURN:	NONE
//----------------------------------------------------------------------------
FSM::~FSM(void)
{
    return;
}


//----------------------------------------------------------------------------
// FUNCTION FSM::FSM(void)
//
// PURPOSE:     Constructor.
//
// INPUT:	NONE.
//
// RETURN:	NONE.
//----------------------------------------------------------------------------
FSM::FSM(void)
{

}

