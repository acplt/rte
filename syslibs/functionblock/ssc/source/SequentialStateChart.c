
/******************************************************************************
*
*   FILE
*   ----
*   SequentialControlChart.c
*
*   History
*   -------
*   2011-07-04   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif


#include "ssclib.h"

OV_DLLFNCEXPORT OV_RESULT ssc_SequentialStateChart_initStepName_set(
		OV_INSTPTR_ssc_SequentialStateChart          pobj,
		const OV_STRING  value
)
{
	OV_INSTPTR_ssc_SequentialStateChart pinst = Ov_StaticPtrCast(ssc_SequentialStateChart, pobj);

	OV_INSTPTR_ssc_step pstep;
	OV_INSTPTR_ssc_step foundStep = NULL;

	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow old INITIAL_VALUE for loading an backup
		return OV_ERR_OK;
	}

	if (pinst->v_workingState == SSC_WOST_INIT){
		foundStep = Ov_SearchChildEx(ov_containment, pobj, value, ssc_step);
		if(foundStep != NULL){
			Ov_ForEachChildEx(ov_containment, pobj, pstep, ssc_step){
				if(foundStep == pstep)
				{
					pstep->v_internalRole = SSC_STEPROLE_START;
					pstep->v_actimode = FB_AM_ON;
				}
				else //deactivate all non init steps and set the correct id
				{
					if (pstep->v_internalRole != SSC_STEPROLE_END) //not an end step
					{
						pstep->v_internalRole = SSC_STEPROLE_NORMAL;
					}
					pstep->v_actimode = FB_AM_OFF;
				}
			}
		}
		else
		{
			pinst->v_error = TRUE;
			ov_string_setvalue(&(pinst->v_errorDetail), "Step not found.");
			return OV_ERR_BADVALUE;
		}
	}
	else
	{
		pinst->v_error = TRUE;
		ov_string_setvalue(&(pinst->v_errorDetail),"Init and end step can only be set when workingState is 0.");
		return OV_ERR_BADVALUE;
	}
	return ov_string_setvalue(&pobj->v_initStepName, value);
}

OV_DLLFNCEXPORT OV_RESULT ssc_SequentialStateChart_endStepName_set(
		OV_INSTPTR_ssc_SequentialStateChart          pobj,
		const OV_STRING  value
)
{
	OV_INSTPTR_ssc_SequentialStateChart pinst = Ov_StaticPtrCast(ssc_SequentialStateChart, pobj);
	OV_UINT count = 0; // count of end step strings
	OV_UINT i = 0; // loop variable
	OV_UINT n = 1;
	OV_STRING *pEndStepList = NULL;
	OV_INSTPTR_ssc_step pstep = NULL;

	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow old INITIAL_VALUE for loading an backup
		return OV_ERR_OK;
	}
	pEndStepList = ov_string_split(value,",",&count);

	Ov_ForEachChildEx(ov_containment, pobj, pstep, ssc_step){
		for(i = 0; i<count; i++){
			if(ov_string_compare(pstep->v_identifier, pEndStepList[i]) == OV_STRCMP_EQUAL)
			{
				pstep->v_internalRole = SSC_STEPROLE_END;
				n++;
			}
		}
	}
	ov_string_freelist(pEndStepList);
	if(n < count)
	{
		pinst->v_error = TRUE;
		ov_string_setvalue(&(pinst->v_errorDetail),"Could not set all end steps.");
		return OV_ERR_BADVALUE;
	}
	return ov_string_setvalue(&pobj->v_endStepName,value);
}

OV_DLLFNCEXPORT OV_RESULT ssc_SequentialStateChart_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ssc_SequentialStateChart pinst = Ov_StaticPtrCast(ssc_SequentialStateChart, pobj);
	OV_INSTPTR_ssc_step pInitStep = NULL;
	OV_INSTPTR_ssc_step pEndStep = NULL;
	OV_RESULT    result;

	/* do what the base class does first */
	result = fb_functionblock_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	// create INIT- & END-step
	result = Ov_CreateObject(ssc_step, pInitStep, pinst, "INIT");
	pInitStep->v_internalRole = SSC_STEPROLE_START;
	pInitStep->v_actimode = FB_AM_ON;
	result = Ov_CreateObject(ssc_step, pEndStep, pinst, "END");
	pEndStep->v_internalRole = SSC_STEPROLE_END;

	//init variables
	pinst->v_workingState = SSC_WOST_INIT;
	pinst->v_actimode = FB_AM_ON;
	pinst->v_iexreq = TRUE;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ssc_SequentialStateChart_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_SequentialStateChart pinst = Ov_StaticPtrCast(ssc_SequentialStateChart, pfb);
    OV_INSTPTR_ssc_step      pActiveStep = NULL;
    OV_INSTPTR_fb_task      taskActivestep = Ov_GetPartPtr(taskActiveStep, pinst);
    OV_INSTPTR_fb_task    pTrans = Ov_GetPartPtr(trans, pinst);
    // helper variables
    OV_RESULT    			 result;
    OV_BOOL	                 exitLoop = TRUE;

	OV_INSTPTR_fb_task          intask = Ov_GetPartPtr(intask, pinst);

	/* Init functionchart intask */
	intask->v_actimode = FB_AM_ON;
	intask->v_cyctime.secs = 0;
	intask->v_cyctime.usecs = 0;
	intask->v_proctime = *pltc;


    // init variables
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);
    taskActivestep->v_actimode = FB_AM_ON;
    taskActivestep->v_cyctime.secs = 0;
    taskActivestep->v_cyctime.usecs = 0;
    pTrans->v_actimode = FB_AM_ON;

	/* Trigger all connections on chart input ports */
	fb_object_triggerInpGetConnections(Ov_PtrUpCast(fb_object, pinst));

    // find active step
    Ov_GetFirstChildEx(fb_tasklist, taskActivestep, pActiveStep, ssc_step);
    if (pActiveStep != NULL){
    	pActiveStep->v_actimode = FB_AM_OFF;
    }

    /*################################
    * state machine: working state
    #################################*/
    do{
    	exitLoop=TRUE;
    	switch (pinst->v_workingState)
    	{
    	/* step 0 init */
    	case SSC_WOST_INIT:
    		switch (pinst->v_woStPhase)
    		{
    		case SSC_PHASE_ENTRYDO:
    			/* init/entry: */
    			if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
    				ov_string_setvalue(&pinst->v_woStText, "INIT");

    				// generic part
    				pinst->v_woStQualifier = SSC_QUALIFIER_DO;
    			}

    			/* init/do: */
    			// reset ssc
    			result=Ov_Call0 (ssc_SequentialStateChart, pinst, resetSsc);
    			if(Ov_Fail(result)) return;

    			// generic part
    			pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
    		break;
    		case SSC_PHASE_EXITTRANS:
    			/* init/transitions */
    			// INIT to START
    			if (pinst->v_EN == SSC_CMD_START){
    				pinst->v_workingState = SSC_WOST_START;
    			}

    			/* init/exit */
    			if (pinst->v_workingState != SSC_WOST_INIT){
    				// generic part
    				pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
				}

    			// generic part
    			pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
    			exitLoop=FALSE;
    		break;
    		} // END switch woStPhase
    	break;
    	/* END step 0 init */

    	/* step 1 START */
        case SSC_WOST_START:
        	switch (pinst->v_woStPhase)
        	{
        	case SSC_PHASE_ENTRYDO:
        		/* entry: */
        		if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
       				ov_string_setvalue(&pinst->v_woStText, "START");

       				// generic part
       				pinst->v_woStQualifier = SSC_QUALIFIER_DO;
       				//printf("start, entry\n");
       			}

       			/* do: */
       			//printf("start, do\n");
       			// activate active step
       			if (pActiveStep != NULL){
       				pActiveStep->v_actimode = FB_AM_ON;
       			}

       			// generic part
       			pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
       		break;
       		case SSC_PHASE_EXITTRANS:
       			/* start/transitions */

       			// START to BREAK
    			if (pinst->v_EN == SSC_CMD_BREAK){
    				pinst->v_workingState = SSC_WOST_BREAK;
    			}else
    			// START to STOP
    			if (pinst->v_EN == SSC_CMD_STOP){
    				pinst->v_workingState = SSC_WOST_STOP;
    			}else
    			// START to RESET
    			if (pinst->v_EN == SSC_CMD_RESET){
    				pinst->v_workingState = SSC_WOST_TERMINATE;
    			}

       			/* start/exit */
       			if (pinst->v_workingState != SSC_WOST_START){
      				// generic part
       				pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
   				}

       			// generic part
               	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
               	exitLoop=FALSE;
       		break;
        	} // END switch woStPhase
        break;
        /* END step 1 START */

        /* step 2 BREAK */
        case SSC_WOST_BREAK:
        switch (pinst->v_woStPhase)
        {
           	case SSC_PHASE_ENTRYDO:
           		/* break/entry: */
           		if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
           			// activate active step
           			if (pActiveStep != NULL){
           				pActiveStep->v_actimode = FB_AM_ON;
           			}

           			ov_string_setvalue(&pinst->v_woStText, "BREAK");

           			// generic part
           			pinst->v_woStQualifier = SSC_QUALIFIER_DO;
           		}

           		/* break/do: */
           		// activate active step
           		if (pActiveStep != NULL){
           			pActiveStep->v_actimode = FB_AM_OFF;
           			pActiveStep->v_phase = SSC_PHASE_ENTRYDO;
           			pActiveStep->v_X=FALSE;
           		}

           		// generic part
           		pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
           		break;
           	case SSC_PHASE_EXITTRANS:
           		/* break/transitions */

           		// BREAK to STOP
           		if (pinst->v_EN == SSC_CMD_STOP){
           			pinst->v_workingState= SSC_WOST_STOP;
           		}else
           		// BREAK to START
           		if (pinst->v_EN == SSC_CMD_START){
           			pinst->v_workingState= SSC_WOST_START;
           		}
           		// BREAK to RESET
           		if (pinst->v_EN == SSC_CMD_RESET){
           			pinst->v_workingState = SSC_WOST_TERMINATE;
           		}

           		// generic part
           		if (pinst->v_workingState != SSC_WOST_BREAK){
           			pinst->v_woStQualifier = SSC_QUALIFIER_EXIT;
           		}

           		/* break/exit */
           		if (pinst->v_woStQualifier == SSC_QUALIFIER_EXIT){
           			// generic part
           			pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
           		}

           	// generic part
           	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
           	exitLoop=FALSE;
           	break;
            } // END switch woStPhase
        break;
        /* END step 2 BREAK */

        /* step 3 STOP */
        case SSC_WOST_STOP:
        switch (pinst->v_woStPhase)
        {
           	case SSC_PHASE_ENTRYDO:
        	/* stop/entry: */
        	if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
        		// execute exit-actions of the end step
        		if (pActiveStep == NULL)					// step specific
        		{
        			pinst->v_error = TRUE;
        			ov_string_setvalue(&pinst->v_errorDetail, "Internal Error (active Step unset)");
        		}else{
        			//Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pActiveStep), execute, pltc);
        			//Frage: Warum funktionier hier der Aufruf von execute nicht?
        			//Ov_Call1 (ssc_step, pActiveStep, typemethod, pltc);
        			Ov_Call1 (fb_functionblock, pActiveStep, typemethod, pltc);
        		}

        		ov_string_setvalue(&pinst->v_woStText, "STOP");

        		// generic part
        		pinst->v_woStQualifier = SSC_QUALIFIER_DO;
        	}

        	/* stop/do: */
        	// reset ssc
        	result=Ov_Call0 (ssc_SequentialStateChart, pinst, resetSsc);
        	if(Ov_Fail(result)) return;

        	// generic part
        	pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
        	break;
       	case SSC_PHASE_EXITTRANS:
        	/* stop/transitions */

           	// STOP to START
        	if (pinst->v_EN == SSC_CMD_START){
        		pinst->v_workingState= SSC_WOST_START;
        	}
        	// STOP to RESET
        	if (pinst->v_EN == SSC_CMD_RESET){
        		pinst->v_workingState = SSC_WOST_TERMINATE;
        	}

        	// generic part
           	if (pinst->v_workingState != SSC_WOST_STOP){
           		pinst->v_woStQualifier = SSC_QUALIFIER_EXIT;
           	}

           	/* exit */
           	if (pinst->v_woStQualifier == SSC_QUALIFIER_EXIT){
           		// generic part
           		pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
       		}

           	// generic part
           	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
           	exitLoop = FALSE;
           	break;
            } // END switch woStPhase
        break;
        /* END step 3 STOP */

    	/* step 4 terminated */
    	case SSC_WOST_TERMINATE:
    		if (pinst->v_EN == SSC_CMD_RESET){
    			// reset ssc
    			result=Ov_Call0 (ssc_SequentialStateChart, pinst, resetSsc);
    			if(Ov_Fail(result)) return;

    			// generic part
    			pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
    			pinst->v_workingState = SSC_WOST_INIT;
    			pinst->v_EN = SSC_CMD_STOP;
    		}
    		break;
    	}
    } while (!exitLoop);
    /* END: state machine: working state
    #################################*/

	/* Execute internal tasks */
	Ov_Call1 (fb_task, taskActivestep, execute, pltc);
	Ov_Call1 (fb_task, pTrans, execute, pltc);

	/* Execute internal SSC task */
	Ov_Call1(fb_task, intask, execute, pltc);

	/* Trigger all connections on chart output ports */
	fb_object_triggerOutSendConnections(Ov_PtrUpCast(fb_object, pinst));

	return;
}

OV_DLLFNCEXPORT OV_RESULT ssc_SequentialStateChart_resetSsc(
	OV_INSTPTR_ssc_SequentialStateChart    pinst
) {
	OV_INSTPTR_ssc_step      pStep = NULL;
	OV_INSTPTR_ssc_step      pInitStep = NULL;
	OV_INSTPTR_fb_task 		 taskActivestep = Ov_GetPartPtr(taskActiveStep, pinst);
	OV_INSTPTR_fb_task       pTransTaskParent = Ov_GetParent(fb_tasklist, pInitStep);
	OV_INSTPTR_fb_functionblock pFbAction=NULL;
	OV_INSTPTR_ssc_SequentialStateChart pSscAction = NULL;
	OV_RESULT result = OV_ERR_OK;
	//OV_ANY orderVar = OV_ANY_INIT;
	//OV_UINT iterator = 0;

	//reset all steps; find and link INIT-step to taskActiveStep
	Ov_ForEachChildEx(ov_containment, pinst, pStep, ssc_step){
		result=Ov_Call0(ssc_step, pStep, resetStep);

		if(pStep->v_internalRole == SSC_STEPROLE_START){
			pInitStep=pStep;
		}
	}
	if (pInitStep == NULL){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "No INIT step is defined.");
		return OV_ERR_BADPATH;
	}
	//should be done with the resetStep a few lines above
	// unlink the init step from its taskparent
	if (pTransTaskParent != NULL){
		Ov_Unlink(fb_tasklist, pTransTaskParent, pInitStep);
	}

	result=Ov_Link(fb_tasklist, taskActivestep, pInitStep);
	if(Ov_Fail(result)){
		return result;
	}

	// reset all actions (FB, CFC or SSC) under .actions folder
	Ov_ForEachChildEx(ov_containment, Ov_GetPartPtr(actions, pinst), pFbAction, fb_functionblock){
		// reset subSSC action
		pSscAction=Ov_DynamicPtrCast(ssc_SequentialStateChart,pFbAction);
		if (pSscAction != NULL){
			pSscAction->v_actimode = FB_AM_ON;
			pSscAction->v_EN=SSC_CMD_STOP;
			//Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pSscAction), execute, &pTime);
			result = Ov_Call0 (ssc_SequentialStateChart, pSscAction, resetSsc);
			if(Ov_Fail(result)){
				return result;
			}
		}
	}

	// clear senderID if this exists
//	orderVar.value.vartype = OV_VT_STRING;
//	orderVar.value.valueunion.val_string = NULL;
//	(void)ssc_setNamedVariable(Ov_PtrUpCast(ov_object, pinst), CC_SENDERID_VARIABLENAME, &orderVar);

	//clear all variables which are references with the ORDEREXECUTOR to get rid of old commands
//	for(iterator = 0;iterator < pinst->v_ORDEREXECUTOR.veclen;iterator++){
//		(void)ssc_setNamedVariable(Ov_PtrUpCast(ov_object, pinst), pinst->v_ORDEREXECUTOR.value[iterator], &orderVar);
//	}

	return OV_ERR_OK;
}

/**
 * disallow deletion in run state
 */
OV_DLLFNCEXPORT OV_ACCESS ssc_SequentialStateChart_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_SequentialStateChart activeHeader = Ov_StaticPtrCast(ssc_SequentialStateChart, pobj);

	OV_ACCESS access_code = fb_functionblock_getaccess(pobj, pelem, pticket);
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		if(!activeHeader){
			//skip handling
		}else if(	activeHeader->v_error == TRUE ||
				activeHeader->v_workingState == SSC_WOST_INIT ||
				activeHeader->v_workingState == SSC_WOST_STOP ||
				activeHeader->v_workingState == SSC_WOST_TERMINATE)
		{
			//allow deletion
			access_code = (access_code | OV_AC_DELETEABLE);
		}else{
			//disallow deletion
			access_code = (access_code &~ OV_AC_DELETEABLE);
		}
		break;
	default:
		break;
	}
	return access_code;
}

OV_DLLFNCEXPORT OV_STRING ssc_SequentialStateChart_activeStep_get(
		OV_INSTPTR_ssc_SequentialStateChart          pinst
) {
	OV_INSTPTR_fb_task	taskActivestep = Ov_GetPartPtr(taskActiveStep, pinst);
	OV_INSTPTR_fb_task	pTaskChild = NULL;

	pTaskChild = Ov_GetFirstChild(fb_tasklist, taskActivestep);
	if(pTaskChild != NULL){
		return pTaskChild->v_identifier;
	}

	return (OV_STRING)NULL;
}
