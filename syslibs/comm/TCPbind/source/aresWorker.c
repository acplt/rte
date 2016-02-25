
/******************************************************************************
 *
 *   FILE
 *   ----
 *   aresWorker.c
 *
 *   History
 *   -------
 *   2015-10-29   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_TCPbind
#define OV_COMPILE_LIBRARY_TCPbind
#endif

#if OV_SYSTEM_UNIX
#define _POSIX_C_SOURCE	199309L
#include <time.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "TCPbind.h"
#include "libov/ov_macros.h"
#include "ks_logfile.h"
#include "TCPbind_helper.h"

#if OV_SYSTEM_NT
#include <windows.h>
#elif OV_SYSTEM_UNIX
#include <pthread.h>
#endif
static struct getAddrInfoElemList elemList;

static OV_INSTPTR_TCPbind_aresWorker pAresWorker = NULL;

/*	host and port are copied and deleted by the worker thread. addrInfo needs to be freed by the caller	*/

OV_DLLFNCEXPORT getAddrInfoElem *TCPbind_aresWorker_insertGetAddrInfo(
		OV_STRING host,
		OV_STRING port,
		struct addrinfo *hints
) {
	struct getAddrInfoElem *pNewElem;

	if(!host || !port || !(*host) || !(*port) || !hints){
		return 0;
	}
	pNewElem = calloc(1, sizeof(getAddrInfoElem));
	if(!pNewElem){
		return 0;
	}

	pNewElem->hints = *hints;
	pNewElem->host = calloc(ov_string_getlength(host)+1, sizeof(char));
	if(!pNewElem->host){
		return 0;
	}
	pNewElem->port = calloc(ov_string_getlength(port)+1, sizeof(char));
	if(!pNewElem->port){
		return 0;
	}

	strcpy(pNewElem->host, host);
	strcpy(pNewElem->port, port);
	pNewElem->status = AIESTATUS_WAITING;
	/*	last element is never deleted, so the pointer is always valid	*/
	pNewElem->pPrevious = elemList.pLastElem;
	/*	insert Element -> this is the only atomic action	*/
	elemList.pLastElem->pNext = pNewElem;
	/*	this one does not need to be atomic, as no other thread touches this value	*/
	elemList.pLastElem = pNewElem;
	return pNewElem;
}

/********************************************************************************************************************************************/

OV_DLLFNCEXPORT void TCPbind_aresWorker_delGetAddrInfoElem(
		getAddrInfoElem *elem
) {
	elem->delete = AIEDELETE_DO;	//	a simple set is atomic
	return;
}

/********************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT TCPbind_aresWorker_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_aresWorker pinst = Ov_StaticPtrCast(TCPbind_aresWorker, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = ksbase_ComTask_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	/* do what */
	if(pAresWorker != NULL){
		KS_logfile_error(("%s: aresWorker needs to be a singleton. will not create.", pinst->v_identifier));
		return OV_ERR_BADPLACEMENT;
	}
	pAresWorker = pinst;

	return OV_ERR_OK;
}

/********************************************************************************************************************************************/

#if OV_SYSTEM_NT || OV_SYSTEM_UNIX

#if OV_SYSTEM_NT
static volatile LONG runWorkerThread;
/*	NEVER EVER let this function return STILL_ACTIVE (259) as the parent process would not recognize the function has returned then	*/
DWORD WINAPI workerThread( LPVOID lpParam ){
	LONG statusOld;
	int opt_on = 1;
	DWORD NumberOfBytesReturned = 0;	//used for SIO_LOOPBACK_FAST_PATH
#elif OV_SYSTEM_UNIX
static volatile unsigned int runWorkerThread;
static volatile unsigned int workerThreadRunning;

void resetRunFlag (void* par){
	workerThreadRunning = 0;
}

void* workerThread( void* lpParam ){
	unsigned int statusOld;
	struct timespec s;
#endif
	struct getAddrInfoElem *pCurrElem;
	struct getAddrInfoElem *pNextElem;
	struct addrinfo *walk;

#if OV_SYSTEM_UNIX
	pthread_cleanup_push(resetRunFlag, NULL);
#endif

	while(runWorkerThread){
		if(elemList.pFirstElem){
			pCurrElem = elemList.pFirstElem;
			while(pCurrElem){
				/*	get the next element	*/
				pNextElem = pCurrElem->pNext;
				/*	check if getAddrInfo is to be done, if so, do it	*/
				statusOld = pCurrElem->status;
				if(statusOld == AIESTATUS_WAITING){
					pCurrElem->status = AIESTATUS_WORKING;
				}
				if(statusOld == AIESTATUS_WAITING){
					//resolve address
					KS_logfile_debug(("aresWorkerThread: issuing getaddrinfo"));
					pCurrElem->result = getaddrinfo(pCurrElem->host, pCurrElem->port, &(pCurrElem->hints), &(pCurrElem->addrInfo));
					if (pCurrElem->result != 0)
					{
						KS_logfile_error(("aresWorkerThread: getaddrinfo failed"));
					}
					KS_logfile_debug(("aresWorkerThread: getaddrinfo done"));
					for (walk = pCurrElem->addrInfo; walk != NULL; walk = walk->ai_next) {
						KS_logfile_debug(("file %s\nline %u:\twalking...", __FILE__, __LINE__));
						pCurrElem->socket = socket(walk->ai_family, walk->ai_socktype, walk->ai_protocol);
						if (pCurrElem->socket == TCPBIND_INVALID_SOCKET){
							KS_logfile_debug(("aresWorkerThread: address not usable"));
							continue;
						}
						KS_logfile_debug(("file %s\nline %u:\tconnectiong to %i", __FILE__, __LINE__, sockfd));
#if OV_SYSTEM_NT
					//opt in for faster localhost connections on new windows hosts. This has to be before connect
					//old includes does not have this new define
#ifndef SIO_LOOPBACK_FAST_PATH
#define SIO_LOOPBACK_FAST_PATH 0x98000010
#endif
					//we are not really interested in errors. Most would be WSAEOPNOTSUPP for pre Windows Server 2012 or Windows 8
					(void)WSAIoctl(pCurrElem->socket, SIO_LOOPBACK_FAST_PATH, &opt_on, sizeof(opt_on), NULL, 0, &NumberOfBytesReturned, 0, 0);
#endif
						if (connect(pCurrElem->socket, walk->ai_addr, walk->ai_addrlen) == TCPBIND_SOCKET_ERROR) {
							TCPBIND_CLOSE_SOCKET(pCurrElem->socket);
							pCurrElem->socket = TCPBIND_INVALID_SOCKET;
							KS_logfile_debug(("%s: connect failed", thisTCPCh->v_identifier));
							continue;
						}
						break;
					}

					//free structures
					freeaddrinfo(pCurrElem->addrInfo);
					pCurrElem->addrInfo = NULL;
					pCurrElem->status = AIESTATUS_DONE;	//	a simple set is atomic
				}
				/*	check if current element should be deleted	*/
				if(pCurrElem->delete == AIEDELETE_DO){
					/*	only delete if it is NOT the last element	*/
					if(pCurrElem->pNext){
						if(pCurrElem->pPrevious){
							pCurrElem->pPrevious->pNext = pCurrElem->pNext;
						}else{
							elemList.pFirstElem = pCurrElem->pNext;
						}
						pCurrElem->pNext->pPrevious = pCurrElem->pPrevious;
						free(pCurrElem->host);
						free(pCurrElem->port);
						if(pCurrElem->addrInfo){
							freeaddrinfo(pCurrElem->addrInfo);
						}
						free(pCurrElem);
					}
				}
				/*	go to the next element	*/
				pCurrElem = pNextElem;
			}
		}
#if OV_SYSTEM_UNIX
		s.tv_sec = 0;
		s.tv_nsec = 10*1000*1000;
		nanosleep(&s, NULL);
#else
		Sleep(10);
#endif
	}
#if OV_SYSTEM_UNIX
	pthread_cleanup_pop(1);
#endif
	return 0;
}
#endif

/********************************************************************************************************************************************/

OV_DLLFNCEXPORT void TCPbind_aresWorker_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_aresWorker pinst = Ov_StaticPtrCast(TCPbind_aresWorker, pobj);
	struct getAddrInfoElem *pNewElem;
#if OV_SYSTEM_NT
	DWORD dwThreadId;
#elif OV_SYSTEM_UNIX
	int iret;
#endif
	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_cycInterval = 50;	/*	just checks if the thread is running -> not needed so often	*/
	/*	create a dummy element	*/
	pNewElem = calloc(1, sizeof(getAddrInfoElem));
	if(!pNewElem){
		KS_logfile_error(("%s: creation of dummy element failed", pinst->v_identifier));
		return;
	}
	pNewElem->status = AIESTATUS_DONE;
	pNewElem->delete = AIEDELETE_DO;
	elemList.pFirstElem = pNewElem;
	elemList.pLastElem = pNewElem;
#if OV_SYSTEM_NT
	runWorkerThread = 1;
	pinst->v_threadHandle = CreateThread(NULL, 0, workerThread, (void*) NULL, 0, &dwThreadId);
	if(pinst->v_threadHandle == 0){
		pinst->v_handleValid = FALSE;
		KS_logfile_error(("%s: creation of worker thread failed", pinst->v_identifier));
		return;
	} else {
		pinst->v_handleValid = TRUE;
	}
#elif OV_SYSTEM_UNIX
	runWorkerThread = 1;
	KS_logfile_debug(("%s: creating worker thread", pinst->v_identifier));
	iret = pthread_create( &(pinst->v_threadHandle), NULL,  workerThread, (void*) NULL);
	if(iret){
		pinst->v_handleValid = FALSE;
		KS_logfile_error(("%s: creation of worker thread failed", pinst->v_identifier));
		return;
	} else {
		pinst->v_handleValid = TRUE;
	}
	workerThreadRunning = 1;
#endif
	return;
}

/********************************************************************************************************************************************/

OV_DLLFNCEXPORT void TCPbind_aresWorker_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_TCPbind_aresWorker pinst = Ov_StaticPtrCast(TCPbind_aresWorker, pobj);
	struct getAddrInfoElem *pCurrElem, *pNextElem;
	/* do what */
	if(pinst->v_handleValid){
#if OV_SYSTEM_NT
		/*	yes, here it waits until the thread is through....	*/
		InterlockedExchange(&(runWorkerThread), 0);
		if(WaitForSingleObject(pinst->v_threadHandle, 5000) != WAIT_OBJECT_0){
			KS_logfile_warning(("%s: worker thread did not terminate correctly and in time. some memory might be lost."));
		}
		pinst->v_handleValid = FALSE;
#elif OV_SYSTEM_UNIX
		/*	we can leave out the atomic one here since it should be an atomic set anyways	*/
		runWorkerThread = 0;
		pthread_join(pinst->v_threadHandle, NULL);
		pinst->v_handleValid = FALSE;
#endif
		/*	delete the elements in the list	*/
		if(elemList.pFirstElem){
			pCurrElem = elemList.pFirstElem;
			while(pCurrElem){
				/*	get the next element	*/
				pNextElem = pCurrElem->pNext;
				if(pCurrElem->pNext){
					if(pCurrElem->pPrevious){
						pCurrElem->pPrevious->pNext = pCurrElem->pNext;
					}else{
						elemList.pFirstElem = pCurrElem->pNext;
					}
					pCurrElem->pNext->pPrevious = pCurrElem->pPrevious;
					free(pCurrElem->host);
					free(pCurrElem->port);
					free(pCurrElem);
				}
				/*	go to the next element	*/
				pCurrElem = pNextElem;
			}
		}
		elemList.pFirstElem = NULL;
		elemList.pLastElem = NULL;
	}
	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);
	return;
}

/********************************************************************************************************************************************/

OV_DLLFNCEXPORT void TCPbind_aresWorker_typemethod (
		OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	 *   local variables
	 */
#if OV_SYSTEM_NT
	DWORD exitCode, dwThreadId;
#elif OV_SYSTEM_UNIX
	int iret;
#endif
	OV_INSTPTR_TCPbind_aresWorker pinst = Ov_StaticPtrCast(TCPbind_aresWorker, this);
	/*	TODO check if thread is running, if not restart it.	*/
#if OV_SYSTEM_NT
	if(!GetExitCodeThread(pinst->v_threadHandle, &exitCode)){
		this->v_actimode = 0;
		KS_logfile_error(("%s: could not determine status of worker thread", pinst->v_identifier));
		return;
	}
	if(exitCode != STILL_ACTIVE){
		KS_logfile_warning(("%s: worker thread exited -restarting it", pinst->v_identifier));
		pinst->v_threadHandle = CreateThread(NULL, 0, workerThread, (void*) NULL, 0, &dwThreadId);
		if(pinst->v_threadHandle == 0){
			pinst->v_handleValid = FALSE;
			KS_logfile_error(("%s: creation of worker thread failed", pinst->v_identifier));
			return;
		}
	}
#elif OV_SYSTEM_UNIX
	if(!workerThreadRunning){
		runWorkerThread = 1;
		KS_logfile_warning(("%s: creating worker thread", pinst->v_identifier));
		iret = pthread_create( &(pinst->v_threadHandle), NULL,  workerThread, (void*) NULL);
		if(iret){
			pinst->v_handleValid = FALSE;
			KS_logfile_error(("%s: creation of worker thread failed", pinst->v_identifier));
			return;
		}
		workerThreadRunning = 1;
	}
#endif
	return;
}

