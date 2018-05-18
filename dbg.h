#pragma once
#include <stdio.h>

#define DEBUG 0
#undef DEBUG
#include "QDebug"

#define DBG_NEWLINE "\n"

#define INFO(...)
#define INFOX(...);
#define ERR(...)

#ifdef DEBUG
	#define DBG(...) fprintf(stderr,"%s:%d ",__FILE__,__LINE__); fprintf(stderr,__VA_ARGS__); fprintf(stderr,"\r\n");
	#define DBGX(...) fprintf(stderr,__VA_ARGS__);
	#define DBGLX(...) fprintf(stderr,"%s:%d ",__FILE__,__LINE__); fprintf(stderr,__VA_ARGS__);
	#define DBG_PDU() printBin();
#else
	#define DBG(...) {};
	#define DBGX(...) {};
	#define DBGLX(...) {};
	#define DBG_PDU() {};
#endif
