
/******************************************************************************
*
*   FILE
*   ----
*   uaServer.c
*
*   History
*   -------
*   2019-01-16   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "opcua.h"
#include "libov/ov_macros.h"
#include "opcua_helpers.h"


UA_StatusCode opcua_server_createConfig(UA_Server* server, OV_INSTPTR_opcua_serverConfig pOvConfig){
    //Create new config with port if pOvConfig is available

	UA_StatusCode result = UA_STATUSCODE_GOOD;
	UA_ServerConfig* config = UA_Server_getConfig(server);
	if (pOvConfig){
		result = UA_ServerConfig_setMinimal(config, pOvConfig->v_port, NULL);
	}else{
		result = UA_ServerConfig_setDefault(config);
	}

	if (result != UA_STATUSCODE_GOOD)
		return result;

    //Delete old application name and uri
    UA_String_deleteMembers(&config->applicationDescription.applicationName.text);
    UA_String_deleteMembers(&config->applicationDescription.applicationUri);
    UA_String_deleteMembers(&config->endpoints[0].server.applicationName.text); //TODO issue in open62541
    UA_String_deleteMembers(&config->endpoints[0].server.applicationUri);

    //Fill in application name
    if(pOvConfig != NULL && ov_string_getlength(pOvConfig->v_applicationName)){
    	config->applicationDescription.applicationName.text = UA_String_fromChars(pOvConfig->v_applicationName);
    	config->endpoints[0].server.applicationName.text = UA_String_fromChars(pOvConfig->v_applicationName);
    }else{
    	//Append OPCUA_DEFAULT_APPLICATIONNAME and SERVERNAME
    	OV_ANY serverName = OV_ANY_INIT;
    	OV_STRING applicationName = NULL;
    	ov_vendortree_getservername(&serverName, NULL); //Do not free, points to static servername
    	ov_string_print(&applicationName,"%s/%s",  OPCUA_DEFAULT_APPLICATIONNAME, serverName.value.valueunion.val_string);
    	config->applicationDescription.applicationName.text = UA_String_fromChars(applicationName);
    	config->endpoints[0].server.applicationName.text = UA_String_fromChars(applicationName);
    	ov_string_setvalue(&applicationName, NULL);
    }

    //Fill in application uri
    if(pOvConfig != NULL && ov_string_getlength(pOvConfig->v_applicationURI)){
    	config->applicationDescription.applicationUri = UA_String_fromChars(pOvConfig->v_applicationURI);
    	config->endpoints[0].server.applicationUri = UA_String_fromChars(pOvConfig->v_applicationURI);
    }else{
    	config->applicationDescription.applicationUri = UA_String_fromChars(OPCUA_DEFAULT_APPLICATIONURI);
    	config->endpoints[0].server.applicationUri = UA_String_fromChars(pOvConfig->v_applicationURI);
    }

    //Set ov logger
    config->logger = opcua_ovUAlogger_new();

    return UA_STATUSCODE_GOOD;
}


//TODO move server startup and shutdown to typemethod?
//TODO write some static functions and proper error clean up (goto error/cleanup)
//TODO write errors to log file?
OV_DLLFNCEXPORT OV_RESULT opcua_server_run_set(
    OV_INSTPTR_opcua_server          pobj,
    const OV_BOOL  value
) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	if(pobj->v_error)
		return OV_ERR_GENERIC;

	if(value != pobj->v_run){
		if(value){ //start server
		    //Create new config and update from linked config in ov
			OV_INSTPTR_opcua_serverConfig pOvConfig = Ov_GetFirstChild(opcua_configToServer, pobj);
			retval = opcua_server_createConfig(pobj->v_server, pOvConfig);
			if(retval != UA_STATUSCODE_GOOD){
				ov_string_print(&pobj->v_errorText, "UA_Server_run_startup failed: %s" , UA_StatusCode_name(retval));
				pobj->v_error = TRUE;
				return OV_ERR_GENERIC;
			}

		    //Startup server
			retval = UA_Server_run_startup(pobj->v_server);
			if(retval != UA_STATUSCODE_GOOD){
				ov_string_print(&pobj->v_errorText, "UA_Server_run_startup failed: %s" , UA_StatusCode_name(retval));
				pobj->v_error = TRUE;
				return OV_ERR_GENERIC;
			}

			pobj->v_isRunning = TRUE;

			//Add opc.tcp port and protocol to server representation
	    	OV_ANY serverName = OV_ANY_INIT;
	    	ov_vendortree_getservername(&serverName, NULL); //Do not free, points to static servername
	    	OV_STRING serverPortStr = NULL;
	    	ov_string_print(&serverPortStr, "%d", (pOvConfig ? pOvConfig->v_port : 4840));
			ksbase_Manager_register(serverName.value.valueunion.val_string,
					2, //TODO define constant
					"OPC.TCP",//TODO get from config : config->endpoints->endpointDescription.... or config->networkLayers-> ...
					serverPortStr,
					30); //TODO define constant
			ov_string_setvalue(&serverPortStr, NULL);

			//Load all interfaces, that are linked with associations
			OV_INSTPTR_opcua_interface pInterface = NULL;
			OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
			Ov_ForEachChild(opcua_serverToInterfaces, pobj, pInterface){
				Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterface);
				if(pVtblInterface){
					pVtblInterface->m_load(pInterface, FALSE);
				}
			}

		}else{ //shutdown server

			//Unload all interfaces, that are linked with associations
			OV_INSTPTR_opcua_interface pInterface = NULL;
			OV_VTBLPTR_opcua_interface pVtblInterface = NULL; //TODO use Call makro instead?
			Ov_ForEachChild(opcua_serverToInterfaces, pobj, pInterface){
				Ov_GetVTablePtr(opcua_interface, pVtblInterface, pInterface);
				if(pVtblInterface){
					pVtblInterface->m_unload(pInterface);
				}
			}

			//Shutdown server
			retval = UA_Server_run_shutdown(pobj->v_server); //Always returns good

			//Unregister opc.tcp port and protocol to server representation
	    	OV_ANY serverName = OV_ANY_INIT;
	    	ov_vendortree_getservername(&serverName, NULL); //Do not free, points to static servername
			ksbase_Manager_unregister(serverName.value.valueunion.val_string, //TODO check what happens, when multiple servers are registered with OPC.TCP --> adapt unregister function for multiple ports
					2, //TODO define constant for ov version, as vendortree version is 1.3.1 or 2.0.0
					"OPC.TCP");//TODO get from config : config->endpoints->endpointDescription.... or config->networkLayers-> ...

			pobj->v_isRunning = FALSE;

			if(retval != UA_STATUSCODE_GOOD){ //Never gone happen based on current implementation
				ov_string_print(&pobj->v_errorText, "UA_Server_run_shutdown failed: %s" , UA_StatusCode_name(retval));
				pobj->v_error = TRUE;
				return OV_ERR_GENERIC; //TODO check wether return is neccessary?
			}

		}
	    pobj->v_run = value;
	}
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_server_reset_set(
    OV_INSTPTR_opcua_server          pobj,
    const OV_BOOL  value
) {
    if(value){
    	pobj->v_error = FALSE;
    	opcua_server_run_set(pobj, FALSE);
    }
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_server_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_opcua_server pinst = Ov_StaticPtrCast(opcua_server, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    //Create new server
	UA_Server *server = UA_Server_new();
	if(!server){
		ov_string_setvalue(&pinst->v_errorText, "UA_Server_new failed.");
		pinst->v_error = TRUE;
		return;
	}
	pinst->v_server = server;

    //Restore running state from shutdown
    opcua_server_run_set(pinst, pinst->v_isRunning);

    return;
}

OV_DLLFNCEXPORT void opcua_server_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_server pinst = Ov_StaticPtrCast(opcua_server, pobj);

    /* do what */
    OV_BOOL isRunning = pinst->v_isRunning; //Save running state
	if (pinst->v_run == TRUE){
		opcua_server_run_set(pinst, FALSE);
	}
	UA_Server_delete(pinst->v_server);
    pinst->v_isRunning = isRunning; //Restore running state

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_ACCESS opcua_server_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	switch(pelem->elemtype) {
	case OV_ET_VARIABLE:
		if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
			if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
				return OV_AC_NONE;
			else{
				if((pelem->elemunion.pvar->v_varprops & OV_VP_DERIVED)){
					if((pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR)){
						return OV_AC_READWRITE;
					} else {
						return OV_AC_READ;
					}
				} else {
					return OV_AC_READWRITE;
				}
			}
		}
		break;
	default:
		break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}

OV_DLLFNCEXPORT void opcua_server_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_opcua_server pinst = Ov_StaticPtrCast(opcua_server, this);
	if(pinst->v_isRunning){
		/* timeout is the maximum possible delay (in millisec) until the next
		   _iterate call. Otherwise, the server might miss an internal timeout
		   or cannot react to messages with the promised responsiveness. */
		/* If multicast discovery server is enabled, the timeout does not not consider new input data (requests) on the mDNS socket.
		 * It will be handled on the next call, which may be too late for requesting clients.
		 * if needed, the select with timeout on the multicast socket server->mdnsSocket (see example in mdnsd library)
		 */
//		UA_UInt16 timeout = UA_Server_run_iterate(pinst->v_server, FALSE);
		UA_Server_run_iterate(pinst->v_server, FALSE);
		/* Now we can use the max timeout to do something else. In this case, we
		   just sleep. (select is used as a platform-independent sleep
		   function.)
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout * 1000;
		select(0, NULL, NULL, NULL, &tv);
	    */
	}
    return;
}

