// STATETBL.H  Define state table structure.
// ----------------------------------------------------------------------------
// History:
// --------
// 07/15/96 M. Gill     Support new single instance FSM.
// 04/26/95 M. Gill     Initial C++ port.
// 01/10/85 M. Gill	Initial creation.
// ----------------------------------------------------------------------------
#ifndef STATETBL_H
#define STATETBL_H

class FSM;
typedef int (FSM::*ACTION_FUNC)(int, void *);

typedef struct tagSTATE_TABLE 
{
    int		current_state;
    int		transition;
    int		next_state;
    ACTION_FUNC	action;
}STATE_TABLE;

typedef struct tagSTATE_TABLE_ENTRY
{
    int  next_state;
    ACTION_FUNC	action;
}STATE_TABLE_ENTRY;

#endif
