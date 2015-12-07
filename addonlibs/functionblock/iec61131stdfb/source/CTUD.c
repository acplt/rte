/*
*	Copyright (C) 2011
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************
*
*   FILE
*   ----
*   CTUD.c
*
*   History
*   -------
*   2011-04-18   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec61131stdfb
#define OV_COMPILE_LIBRARY_iec61131stdfb
#endif


#include "iec61131stdfb.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT void iec61131stdfb_CTUD_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec61131stdfb_CTUD pinst = Ov_StaticPtrCast(iec61131stdfb_CTUD, pfb);
	
	if(pinst->v_R)
	{
		pinst->v_CV = 0;
		pinst->v_LastCLKU = pinst->v_CU;
		pinst->v_LastCLKD = pinst->v_CD;
		pinst->v_QU = (pinst->v_CV >= pinst->v_PV);
		pinst->v_QD = (pinst->v_CV <= 0);
		return;
	}
	else if(pinst->v_LD)
	{
		pinst->v_CV = pinst->v_PV;
		pinst->v_LastCLKU = pinst->v_CU;
		pinst->v_LastCLKD = pinst->v_CD;
		pinst->v_QU = (pinst->v_CV >= pinst->v_PV);
		pinst->v_QD = (pinst->v_CV <= 0);
		return;
	}
	else
	{
		if(pinst->v_CU && (!pinst->v_LastCLKU)
			&& !(pinst->v_CD && (!pinst->v_LastCLKD))
			&& pinst->v_CV < pinst->v_PVmax)
				pinst->v_CV++;
		else
			if(pinst->v_CD && (!pinst->v_LastCLKD)
			&& !(pinst->v_CU && (!pinst->v_LastCLKU))
			&& pinst->v_CV > pinst->v_PVmin)
				pinst->v_CV--;
				
		pinst->v_LastCLKU = pinst->v_CU;
		pinst->v_LastCLKD = pinst->v_CD;
		pinst->v_QU = (pinst->v_CV >= pinst->v_PV);
		pinst->v_QD = (pinst->v_CV <= 0);
			return;
	}
	
    return;
}
