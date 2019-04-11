##############################################
# Brute force qkernel build
#############################################

############################################
# Library paths & Publish directory
############################################
PTHREADLIB = -L/usr/lib
PUB_DIR	   =  /home/mgill/projects/qkernel_ports/linux

###########################################
# Include directories
###########################################
PTHREADINC = -I/usr/include

QKINC 	   = -I$(PUB_DIR)

###################
# Compiler options
###################
CCOPT  = $(QKINC) $(PTHREADINC)
CC     = g++ -c -w -D_REENTRANT $(CCOPT)

###########################
# Local objects & headers
###########################
OBJECTS= evl.o\
	 fsm.o\
	 mos.o\
	 resource.o\
	 tcpserver.o\
	 tcpsocket.o\
	 tcp_application.o\
	 ua_dterm.o\
	 ua_diag.o\
	 sio.o\
	 timer.o\
	 queue.o\
	 sem.o\
	 CDaemon.o\
	 CDaemonErrorLogger.o\
	 CDaemonEventLogger.o\
	 
HEADERS= ua_dterm.h\
	 ua_diag.h\
	 asc.h\
	 evl.h\
	 fsm.h\
	 mos.h\
	 qkernel.h\
	 qktypes.h\
	 queue.h\
	 resource.h\
	 sem.h\
	 statetbl.h\
	 sysevents.h\
	 tcp_application.h\
	 tcpserver.h\
	 tcpsocket.h\
	 timer.h\
	 CDaemon.h\
	 CDaemonErrorLogger.h\
	 CDaemonEventLogger.h\
	 sio.h

test.bin	:libqkernel.a  test.o $(HEADERS)
		gcc -o $(PUB_DIR)/test.bin test.o -L$(PUB_DIR) -lqkernel

libqkernel.a 	:$(OBJECTS) $(HEADERS)
		gcc -r -nostdlib -o\
		$(PUB_DIR)/libqkernel.a $(OBJECTS) $(PTHREADLIB) -lpthread

test.o			:test.cpp test.h $(HEADERS)
			$(CC) test.cpp

CDaemon.o		:CDaemon.cpp $(HEADERS)
			$(CC) CDaemon.cpp

CDaemonEventLogger.o	:CDaemonEventLogger.cpp $(HEADERS)
			$(CC) CDaemonEventLogger.cpp

CDaemonErrorLogger.o	:CDaemonErrorLogger.cpp $(HEADERS)
			$(CC) CDaemonErrorLogger.cpp

sigcatch.o		:sigcatch.cpp $(HEADERS)
			$(CC) sigcatch.cpp

evl.o			:evl.cpp $(HEADERS)
			$(CC) evl.cpp

fsm.o			:fsm.cpp $(HEADERS)
			$(CC) fsm.cpp

mos.o			:mos.cpp $(HEADERS)
			$(CC) mos.cpp

resource.o		:resource.cpp $(HEADERS)
			$(CC) resource.cpp

ua_diag.o		:ua_diag.cpp $(HEADERS)
			$(CC) ua_diag.cpp

ua_dterm.o		:ua_dterm.cpp $(HEADERS)
			$(CC) ua_dterm.cpp

tcp_application.o	:tcp_application.cpp $(HEADERS)
			$(CC) tcp_application.cpp

tcpserver.o		:tcpserver.cpp $(HEADERS)
			$(CC) tcpserver.cpp

tcpsocket.o		:tcpsocket.cpp $(HEADERS)
			$(CC) tcpsocket.cpp

sio.o 			:sio.cpp $(HEADERS)
			$(CC) sio.cpp

timer.o			:timer.cpp $(HEADERS)
			$(CC) timer.cpp

queue.o			:queue.cpp $(HEADERS)
			$(CC) queue.cpp

sem.o			:sem.cpp $(HEADERS)
			$(CC) sem.cpp

test.o			:test.cpp $(HEADERS)
			$(CC) test.cpp








