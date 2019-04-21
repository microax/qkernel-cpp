// EVL.H	 Definition file for EVL class.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/21/96 M. Gill     Include qkernel.h
// 04/04/96 M. Gill	Add overloaded SendMessage method. (stub)
// 01/02/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef EVL_H
#define EVL_H

#include <qkernel.h>

class EVL : public MOS
{
public:    
    int MessageHandler(RESOURCE_NODE *t);
    int SendMessage(int, char *);
    int SendMessage(char *);

};

#endif









