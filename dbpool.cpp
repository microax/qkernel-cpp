// DBPOOL.CPP	Database resource pool class.
//-----------------------------------------------------------------------------
// History:
// --------
// 03/18/95 M. Gill     Initial creation.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "qkernel.h"
#include "dbpool.h"

static int dbinit_flag =0;
extern MOS*	event_log;

// ---------------------------------------------------------------------------
// FUNCTION:	DB_POOL::InitDbNodes()
//
// PURPOSE:	Method to Initialize DB_NODEs.
//		DB_NODE is the text portion of RESOURCE_NODE. 
//
// INPUT:	1) Number of nodes.
//		2) Server name.
//		3) User name.
//		4) User password.
//
// RETURN:	Always 0.
// ---------------------------------------------------------------------------
int DB_POOL::InitDbNodes(int numnodes, char *server, char *user, char *passwd)
{

    int i;
    DB_NODE *dbnode; 
    RESOURCE_NODE *n;

    if(dbinit_flag ==0)
    {
	dbinit();		// Do this only once per process.
	dbinit_flag++;
    }

    for(i=0; i<numnodes ;i++)
    {
	n      = Allocate();
	dbnode = (DB_NODE *)n->text;

	sprintf(dbnode->user, user);
	sprintf(dbnode->password, passwd);
	sprintf(dbnode->server, server);

	dbnode->login = dblogin();

	DBSETLUSER(dbnode->login, dbnode->user);
	DBSETLPWD(dbnode->login,  dbnode->password);

	dbnode->dbproc = dbopen(dbnode->login, dbnode->server);	
	if( dbnode->dbproc == NULL )
	{
	    event_log->SendMessage("dbopen() failed");
	}
    	else
    	{
	    event_log->SendMessage("DB_NODE created.");
	    Free(n);	// Return to list
    	}
    }
    return(0);
}


// ---------------------------------------------------------------------------
// FUNCTION:	DB_POOL::DB_POOL()
//
// PURPOSE:	Contructor to create a pool of Database interface
//		nodes (dbproc).
//		
// INPUT:	1) Number of nodes.
//		2) Server name.
//		3) User name.
//		4) User password.
//
// RETURN:	NONE.
// ---------------------------------------------------------------------------
DB_POOL::DB_POOL(int num, char *server, char *user, char *password) 
		: RESOURCE_POOL(num, sizeof(DB_NODE))
{
    InitDbNodes(num, server, user, password);

}


DB_POOL::~DB_POOL()
{

}








