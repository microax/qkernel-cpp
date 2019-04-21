##############################################
# basic qkernel build
#############################################

############################################
# Library paths & Publish directory
############################################
PTHREADLIB = -L/usr/lib//x86_64-linux-gnu
PUB_DIR	   =  .

###########################################
# Include directories
###########################################
PTHREADINC = -I/usr/include
QKINC 	   = -I$(PUB_DIR)

###################
# Compiler options
###################
CCOPT  = $(QKINC) $(PTHREADINC)
CC     = g++ -c -w -fpermissive $(CCOPT)

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
	 queue.o\
	 sem.o\
	 CDaemon.o\
	 CDaemonErrorLogger.o\
	 CDaemonEventLogger.o\
	 timer.o\

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
		g++ -o $(PUB_DIR)/test.bin test.o -L$(PUB_DIR) $(PTHREADLIB) -lpthread -lqkernel

libqkernel.a 	:$(OBJECTS) $(HEADERS)
		ar cr $(PUB_DIR)/libqkernel.a $(OBJECTS)

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

clean			:
			@rm -f $(PUB_DIR)/libqkernel.a || true
			@rm -f $(PUB_DIR)/*.o          || true
			@rm -f $(PUB_DIR)/test.bin     || true
			@rm -f $(PUB_DIR)/testd.pid    || true
			@rm -f $(PUB_DIR)/testd.log    || true
