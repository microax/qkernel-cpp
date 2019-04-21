// DBPOOL.H	 Definition file for DB_POOL class.
//-----------------------------------------------------------------------------
// History:
// --------
// 03/18/95 M. Gill	Initial creation.
//----------------------------------------------------------------------------
#ifndef DBPOOL_H
#define DBPOOL_H

#define	FORCE_PROTOTYPES

extern "C"{

#include <sybfront.h>
#include <sybdb.h>
#include <syberror.h>
}
#include "resource.h"
#include "evl.h"

typedef struct tagDB_NODE
{
    char 	user[80];
    char 	password[80];
    char 	server[80];
    LOGINREC    *login;
    DBPROCESS   *dbproc;

}DB_NODE;

class DB_POOL : public RESOURCE_POOL
{
public:    

    DB_POOL::DB_POOL(int , char *, char *, char *);
    DB_POOL::~DB_POOL(void);
    int InitDbNodes(int, char *, char *, char *);
};

#endif


















