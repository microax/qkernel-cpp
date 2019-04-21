// FSM.H	 Definition file for FSM class.
//-----------------------------------------------------------------------------
// History:
// --------
// 09/07/97 M. Gill     Changed NUMBER_OF_STATES from 25 to 15.
//                      Changed NUMBER_OF_EVENTS from 50 to 25.
// 07/21/96 M. Gill     Initial pthread port.
// 07/15/96 M. Gill     New single instance FSM.
// 04/23/96 N. Radisch  moved INVALID_EVENT and INIT_OK into sysevents file
// 01/10/95 M. Gill	Solaris 2.5 port.
// 01/10/85 M. Gill	Initial Creation.
//----------------------------------------------------------------------------
#ifndef FSM_H
#define FSM_H

#include <qkernel.h>

const int FSM_MAX_THREAD      =200;
const int FSM_HEAD_STRING_LEN =80;
const int FSM_QUE_SIZE        =200;
const int NUMBER_OF_STATES    =15;
const int NUMBER_OF_EVENTS    =25;
const int STRLEN              =FSM_HEAD_STRING_LEN;

const int FSM_STACK_SIZE=4096;

typedef struct tagFSM_MSG
{
    struct tagFSM_MSG *link;	// Link to next message
    int  event;			// Event/transition 
    int  data;	 		// data
    void *d_ptr;		// data pointer
}FSM_MSG;

class FSM;

typedef struct tagFSM_PORT
{
    int  	 hThread;		  // handle to the thread
    pthread_t    thread_id;		  // thread id
    pthread_attr_t thread_attr;           // thread attributes
    size_t       thread_stack_size;       // thread stack size
    int		 channel;	          // channel #
    int		 next_state;		  // next state
    FSM_MSG	 msg_q[FSM_QUE_SIZE];     // message array
    SEM	 	 *msg_sem;		  // message queue semaphore
    QUEUE	 msgque;		  // message queue
    QUEUE	 freeq;			  // free queue
    TIMER	 *timer;		  // Local timer.
    void 	 *app_ctrl;		  // Application Context block
    int		 current_state;		  // Current state
    int 	 event;			  // Event
    int		 event_data;   		  // Event Data
    int          current_state_save;      // Used to hold current state.
    FSM		*mThisPointer;	          // 

    STATE_TABLE_ENTRY State[NUMBER_OF_STATES][NUMBER_OF_EVENTS];
    
}FSM_PORT;

class FSM
{

protected:

    static void *Main(FSM_PORT *);
    static FSM_MSG *RcvMsg(FSM_PORT*);
    static void RelMsg(FSM_PORT*, FSM_MSG*);

public:

    FSM_PORT *appPort;	// Pointer to thread context.

    FSM(void);
    ~FSM(void);
    virtual	int Send(int event, int data, void *d_ptr);
    virtual     int InvalidEvent(int, void *);
    virtual     int DoNothing(int, void *);
    void Start(STATE_TABLE *app_table, int channel_num);
    void Attach(STATE_TABLE *app_table);
};
#endif


