// EVL.CPP    Event logger.
//-----------------------------------------------------------------------------
// History:
// --------
// 04/04/96 M. Gill	1) Place message on holding queue so that
//			   other application tasks  can use it.
//			2) Add overloaded method 'SendMessage' with
//			   severity option.
//			
// 01/02/95 M. Gill     Initial creation.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <qkernel.h>
#include <evl.h>


// ---------------------------------------------------------------------------
// FUNCTION:	EVL::MessageHandler()
//
// PURPOSE:	Method to handle message records.
//
// INPUT:	Pointer to RESOURCE_NODE.
//
// RETURN:	None.
// ---------------------------------------------------------------------------
int EVL::MessageHandler(RESOURCE_NODE *t)
{
    FormatString(t);		// Format message
    PostHoldQueue(t);		// Place in holding queue.
    printf((char *)t->text);	// print it.
    printf("\n");
    return(0);
}

// ---------------------------------------------------------------------------
// FUNCTION:	EVL::SendMessage()
//
// PURPOSE:    Overloaded Method to handle message records.
//
// INPUT:      1) unused int. 2) Pointer to record.
//
// RETURN:     SUCCESS = OK
//             FAIL    = Allocation error.
// ---------------------------------------------------------------------------
int EVL::SendMessage(int n, char *message)
{
    return(MOS::SendMessage(message));

}

// ---------------------------------------------------------------------------
// FUNCTION:	EVL::SendMessage()
//
// PURPOSE:    Overloaded Method to handle message records.
//
// INPUT:      Pointer to record.
//
// RETURN:     SUCCESS = OK
//             FAIL    = Allocation error.
// ---------------------------------------------------------------------------
int EVL::SendMessage(char *message)
{
    return(MOS::SendMessage(message));

}



