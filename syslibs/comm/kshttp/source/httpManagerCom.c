/*
*	Copyright (C) 2014
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
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif


#include "config.h"

void kshttp_httpManagerCom_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling);


OV_DLLFNCEXPORT void kshttp_httpManagerCom_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpManagerCom pinst = Ov_StaticPtrCast(kshttp_httpManagerCom, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_cycInterval = 5000; /*	cycle every 5 seconds at first	*/
	pinst->v_actimode = 1;
	pinst->v_Tries = 0;


	return;
}

OV_DLLFNCEXPORT void kshttp_httpManagerCom_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_kshttp_httpManagerCom	thisMngCom = Ov_StaticPtrCast(kshttp_httpManagerCom, pobj);
	OV_INSTPTR_ksbase_Manager		pManager = NULL;
	OV_INSTPTR_kshttp_httpClient		httpClient = NULL;
	OV_ANY							servername;

	if(thisMngCom->v_UseShortCut == TRUE)
	{
		KS_logfile_debug(("%s: unregister: using shortCut", thisMngCom->v_identifier));
		pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
		if(!pManager)
		{
			KS_logfile_error(("%s: shutdown: no Manager here. Cannot unregister", thisMngCom->v_identifier));
			return;
		}

		KS_logfile_debug(("%s: unregistering MANAGER kshttp protocol", thisMngCom->v_identifier));
		if(Ov_Fail(ksbase_Manager_unregister("MANAGER", 2, KSHTTP_IDENTIFIER)))
		{
			thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;	/*	set register error	*/
			thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
			thisMngCom->v_Tries = 0;		/*	for next start up reset counter	*/
			return;
		}
		else
		{
			thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_NOTREGISTERED;	/*	set state to unregistered	*/
			thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
			thisMngCom->v_Tries = 0;
			return;
		}
	}
	else
	{/*	find the first httpClient in containment to request unregister	*/
		httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetFirstChild(ov_containment, thisMngCom));
		if(httpClient)
		{
			while(httpClient && (Ov_GetClassPtr(httpClient) != pclass_kshttp_httpClient))
				httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetNextChild(ov_containment, httpClient));
		}

		servername.value.vartype = OV_VT_VOID;
		servername.value.valueunion.val_string = NULL;
		ov_vendortree_getservername(&servername, NULL);

		if(httpClient)
			kshttp_httpClient_requestUnRegister(Ov_StaticPtrCast(ksbase_ClientBase, httpClient), NULL, servername.value.valueunion.val_string, 2, NULL, NULL);

	}
	ov_string_setvalue(&thisMngCom->v_OwnPort, NULL);
	thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_NOTREGISTERED;	/*	set state to unregistered	*/
	thisMngCom->v_cycInterval = 5000;	/*	5 seconds cyctime for next start up	*/
	thisMngCom->v_Tries = 0;


    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT void kshttp_httpManagerCom_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*
    *   local variables
    */
	OV_INSTPTR_kshttp_httpManagerCom	thisMngCom = Ov_StaticPtrCast(kshttp_httpManagerCom, this);
	OV_INSTPTR_ksbase_Manager 		pManager = NULL;
	OV_INSTPTR_ksbase_ComTask		pListener = NULL;
	OV_INSTPTR_ov_class				pClListener = NULL;
	OV_INT							port = -1;
	OV_STRING						OptValTemp = NULL;
	OV_ELEMENT						elemPort;
	OV_ELEMENT						ListenerElement;
	OV_ANY							servername;
	OV_INSTPTR_kshttp_httpClient		httpClient = NULL;
	OV_RESULT						result;

	switch(thisMngCom->v_RegisterState)
	{

	case HTTP_MNGRCOM_REGISTERSTATE_NOTREGISTERED:	/*	if not registered or registered successfully (in latter case typemethod is slowed down to TTL * 0.9) or error (in this case typemethod is slowed down for retries)	*/
	case HTTP_MNGRCOM_REGISTERSTATE_REGISTERED:
	case HTTP_MNGRCOM_REGISTERSTATE_ERROR:
		if(thisMngCom->v_Tries < 3)
		{
			/*	if the OwnPort is not set check commandline options and if they are not set check for a TCPListener (default binding)	*/
			if(!thisMngCom->v_OwnPort || !(*thisMngCom->v_OwnPort) || ov_string_compare(thisMngCom->v_OwnPort, "-1") == OV_STRCMP_EQUAL)
			{
				/*
				 * Determine port
				 */
				ov_memstack_lock();
				port = ov_vendortree_getport();
				if(port < 0 )// PORT not set
				{
					//check environment Variable OWNPORT (for compatibility reasons)
					if (getenv("OWNPORT")) //OWNPORT set
					{
						OptValTemp = getenv("OWNPORT");
						ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
					}
					else
					{	//check option KS_PORT
						OptValTemp = ov_vendortree_getcmdlineoption_value("KS_PORT");
						if(OptValTemp && *OptValTemp)
						{
							ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
						}
						else	/*	check the TCPListener (default binding) WITHOUT creating a dependency on TCPBind	*/
						{	/*	get TCPlistener class	*/
							/*	ov_searchchild does not work here, as ov_instanciation lacks the property OV_AP_LOCAL	*/
							pClListener = Ov_GetFirstChild(ov_inheritance, pclass_ksbase_ComTask);
							while(pClListener)
							{
								if(ov_string_compare(pClListener->v_identifier, "TCPListener") == OV_STRCMP_EQUAL)
									break;
								pClListener = Ov_GetNextChild(ov_inheritance, pClListener);
							}
							if(!pClListener)
							{
								KS_logfile_error(("%s: typemethod: OwnPort not set and class TCPListener (default binding) not found. retry in 2 minutes.", this->v_identifier));
								this->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
								ov_memstack_unlock();
								return;
							}
							/*	get TCPListener instance	*/
							pListener = Ov_StaticPtrCast(ksbase_ComTask, Ov_GetFirstChild(ov_instantiation, pClListener));
							if(!pListener)
							{
								KS_logfile_error(("%s: typemethod: OwnPort not set and TCPListner instance (default binding) not found. retry in 2 minutes.", this->v_identifier));
								this->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
								ov_memstack_unlock();
								return;
							}

							/*	read out port variable	*/
							ListenerElement.elemtype = OV_ET_OBJECT;
							ListenerElement.pobj = Ov_PtrUpCast(ov_object, pListener);

							if(Ov_Fail(ov_element_searchpart(&ListenerElement, &elemPort, OV_ET_VARIABLE, "port")))
							{
								KS_logfile_error(("%s: typemethod: element port not found. deactivating registration.", this->v_identifier));
								ov_memstack_unlock();
								this->v_actimode = 0;
								return;
							}
							if(elemPort.elemtype == OV_ET_VARIABLE)
							{
								if(elemPort.pvalue)
								{
									port = *((OV_INT*) elemPort.pvalue);
								}
								else
								{
									KS_logfile_error(("element value* is NULL"));
								}
								OptValTemp = ov_memstack_alloc(12);	/*	this way prevent us from NULL-pointer exceptions in ov_string_print	*/
								if(OptValTemp)
								{
									sprintf(OptValTemp, "%" OV_PRINT_INT, port);
									ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
								}
								else
								{
									KS_logfile_error(("%s typemethod: could not set port: HEAPOUTOFMEMORY", thisMngCom->v_identifier));
								}
							}
							else
							{
								KS_logfile_error(("%s: typemethod: could not obtain port from TCPListener. deactivating registration.", this->v_identifier));
								ov_memstack_unlock();
								this->v_actimode = 0;
								return;
							}
						}
					}
				}
				else
				{
					OptValTemp = ov_memstack_alloc(12);
					sprintf(OptValTemp, "%" OV_PRINT_INT, port);
					ov_string_setvalue(&thisMngCom->v_OwnPort, OptValTemp);
				}
				ov_memstack_unlock();
			}
			/*************************************************************************************************************************************************************
			 * here we definitely know the port
			 ************************************************************************************************************************************************************/

			if(thisMngCom->v_UseShortCut == TRUE)
			{
				KS_logfile_debug(("%s: typemethod: using shortCut", this->v_identifier));
				pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
				if(!pManager)
				{
					KS_logfile_error(("%s: typemethod: no Manager here. Cannot register", this->v_identifier));
					return;
				}

				KS_logfile_debug(("%s: registering MANAGER kshttp protocol on port %s", thisMngCom->v_identifier, thisMngCom->v_OwnPort));
				if(Ov_Fail(ksbase_Manager_register("MANAGER", 2, KSHTTP_IDENTIFIER, thisMngCom->v_OwnPort, 30)))
				{
					thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;	/*	set register error	*/
					thisMngCom->v_Tries++;
					return;
				}
				else
				{
					thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_REGISTERED;	/*	set state to registered and slow down typemethod	*/
					thisMngCom->v_cycInterval = 30000;	/*	re-register every 30 seconds (assuming rootcomtask runs at 1 msec)	*/
					thisMngCom->v_Tries = 0;
					return;
				}
			}
			else
			{
				servername.value.vartype = OV_VT_VOID;
				servername.value.valueunion.val_string = NULL;
				ov_vendortree_getservername(&servername, NULL);

				/*	find the first httpClient in containment to request register	*/
				httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetFirstChild(ov_containment, this));
				if(httpClient)
				{
					while(httpClient && (Ov_GetClassPtr(httpClient) != pclass_kshttp_httpClient))
						httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetNextChild(ov_containment, httpClient));
				}

				if(!httpClient)
				{/*	no httpClient in containment --> create one	*/
					result = Ov_CreateObject(kshttp_httpClient, httpClient, this, "httpClient");
					if(Ov_Fail(result))
					{
						KS_logfile_error(("%s: typemethod: could not create httpClient. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
						this->v_actimode = 0;
						thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;
						return;
					}
					httpClient->v_holdConnection = FALSE;	//todo try to figure out if this is easy to implement
				}

				kshttp_httpClient_requestRegister(Ov_PtrUpCast(ksbase_ClientBase, httpClient), NULL, servername.value.valueunion.val_string, 2, strtol(thisMngCom->v_OwnPort, NULL, 10), 30, Ov_PtrUpCast(ov_domain, this), &kshttp_httpManagerCom_Callback);
				thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_WAITING;	/*	set state to waiting for answer	*/
				thisMngCom->v_cycInterval = 5000;	/*	used as a timeout: 5 seconds should be enough for a local connection (assuming rootcomtask runs at 1 msec)	*/
				thisMngCom->v_Tries++;
				return;
			}

		}
		else
		{
			KS_logfile_error(("%s: registering at Manager failed 3 times. retry in 2 minutes.", thisMngCom->v_identifier));
			this->v_cycInterval = 120000;	/*	retry in 2 minutes	*/
			thisMngCom->v_Tries = 0;
			/*	reset client	*/
			/*	find the first httpClient in containment to issue reset	*/
			httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetFirstChild(ov_containment, this));
			if(httpClient)
			{
				while(httpClient && (Ov_GetClassPtr(httpClient) != pclass_kshttp_httpClient))
					httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetNextChild(ov_containment, httpClient));
			}
			kshttp_httpClientBase_reset(Ov_StaticPtrCast(ksbase_ClientBase, httpClient));
			return;
		}
		break;

	case HTTP_MNGRCOM_REGISTERSTATE_WAITING: 	/*	waiting for answer	*/
		/*	the callbackfunction sets state to registered or to error. if typemethod is run in this state a timeout occurred	*/
		/*	increment Tries, reset Client, set state to error and be called again in 5 seconds	*/
		thisMngCom->v_Tries++;

		httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetFirstChild(ov_containment, this));
		if(httpClient)
		{
			while(httpClient && (Ov_GetClassPtr(httpClient) != pclass_kshttp_httpClient))
				httpClient = Ov_StaticPtrCast(kshttp_httpClient, Ov_GetNextChild(ov_containment, httpClient));
		}
		kshttp_httpClientBase_reset(Ov_StaticPtrCast(ksbase_ClientBase, httpClient));

		thisMngCom->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;
		thisMngCom->v_cycInterval = 5000;
	break;

	}
	return;
}

void kshttp_httpManagerCom_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling){
	OV_INSTPTR_ksbase_ClientBase	httpClient = Ov_StaticPtrCast(ksbase_ClientBase, instanceCalling);
	OV_INSTPTR_kshttp_httpManagerCom	this = Ov_StaticPtrCast(kshttp_httpManagerCom, instanceCalled);
	OV_RESULT						ksResult = OV_ERR_OK;
	OV_RESULT						result = OV_ERR_OK;

	result = kshttp_httpClient_processRegister(httpClient, NULL, &ksResult);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: callback: error processing answer: %s", this->v_identifier, ov_result_getresulttext(result)));
		this->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;	/*	error	*/
		return;
	}


	KS_logfile_debug(("%s: callback: register processed: result: %lu", this->v_identifier, result));
	if(Ov_OK(ksResult))
	{
		this->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_REGISTERED;	/*	registered	*/
		this->v_Tries = 0;
		this->v_cycInterval = 30000;	/*	reregister in 30 seconds	*/
		this->v_NextExecTime.secs += 30;
		return;
	}
	else
	{
		this->v_RegisterState = HTTP_MNGRCOM_REGISTERSTATE_ERROR;	/*	error	*/
		return;
	}

	return;
}
