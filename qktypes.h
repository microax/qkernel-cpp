//  QKTYPES.H     COMMON TYPE DEFINITIONS
// ---------------------------------------------------------------------------
//  History:
//  --------
//  11/26/95 M. Gill    Add LPNODE, LPTHREAD_START_ROUTINE.
//  04/26/95 M. Gill    Initial C++ port.
//  01/10/85 M. Gill	Initial creation.
// ---------------------------------------------------------------------------
#ifndef	QKTYPES_H
#define	QKTYPES_H

#define LOW_ORDER   0
#define HIGH_ORDER  1

typedef	unsigned char BYTE;
typedef unsigned short WORD;
typedef	unsigned long DWORD;

typedef void* LPTHREAD_START_ROUTINE;
typedef int HANDLE;


typedef union
{
    WORD  w;
    BYTE  b[2];
}BYTE_2;

typedef union
{
    DWORD l;
    WORD  w[2];
}WORD_2;

typedef void** LPNODE;


#endif

