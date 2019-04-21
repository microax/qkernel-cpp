// TCP_APPLICATION.H	 Definition file for TCP_APPLICATION
//-----------------------------------------------------------------------------
// History:
// --------
// 07/15/96 M. Gill     support new single instance FSM.
// 04/17/95 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef TCP_APPLICATION_H
#define TCP_APPLICATION_H

#include <tcpserver.h>
#include <qkernel.h>

class TCP_APPLICATION : public FSM
{

public:

	STATE_TABLE *StateTable;
	TCP_SERVER  *tcp_server;
	virtual void InitStateTable(void);
	void Start(TCP_SERVER *tcps, int chan);	
};



#endif












