/*
*   $Id: ov_malloc.c,v 1.2 1999-09-15 10:48:22 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/*
*	History:
*	--------
*	13-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/
/*
*	Description:
*	------------
*	The routines included in this file are a wrapper for the standard
*	malloc(), realloc() and free() functions, which may cause problems
*	with DLLs/shared libraries, if they have an own allocator.
*	Usage: free any heap memory allocated in function of LIBOV using the
*	function ov_free() and (re)allocate any heap memory freed later in
*	LIBOV using ov_malloc() or ov_realloc().
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_malloc.h"
#include "libov/tlsf.h"

#if OV_SYSTEM_MC164
#define malloc	xmalloc
#define free	xfree
#define realloc	xrealloc
#endif

/*	----------------------------------------------------------------------	*/
#ifdef TLSF
static void *heappool = NULL;

OV_DLLFNCEXPORT void ov_initHeap(size_t size, void* heap){
	heappool = heap;
	init_memory_pool(size,heappool);
}

#endif
/*
*	Allocate memory on the heap
*/


OV_DLLFNCEXPORT OV_POINTER ov_malloc(
	OV_UINT		size
) {
#ifdef TLSF
	return malloc_ex(size,heappool);
#endif

	return malloc(size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory allocated in the heap
*/
OV_DLLFNCEXPORT void ov_free(
	OV_POINTER	ptr
) {
#ifdef TLSF
	free_ex(ptr,heappool);
#else
	free(ptr);
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Reallocate memory on the heap
*/
OV_DLLFNCEXPORT OV_POINTER ov_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
) {
#ifdef TLSF
	return realloc_ex(ptr,size,heappool);
#endif
	return realloc(ptr, size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Duplicate a string on the heap using malloc
*/
OV_DLLFNCEXPORT OV_STRING ov_strdup(
	OV_STRING	string
) {
	/*
	*	local variables
	*/
    OV_STRING result;
#ifdef TLSF
    result = (OV_STRING)ov_malloc(strlen(string)+1);
#else
	result = (OV_STRING)malloc(strlen(string)+1);
#endif
    if(!result) {
        return NULL;
    }
    strcpy(result, string);
    return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

