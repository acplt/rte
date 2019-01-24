
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

static OV_RESULT opcua_switch_new(UA_ServerConfig* config, OV_STRING* errorText){
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_NodestoreSwitch * storeSwitch = UA_NodestoreSwitch_new();
    if(storeSwitch == NULL){
    	ov_string_setvalue(errorText, "UA_NodestoreSwitch_new failed. Not enough heap memory.");
    	return OV_ERR_HEAPOUTOFMEMORY;
    }

    //Save default store from config temporary
    UA_Nodestore* uaDefaultStore = UA_malloc(sizeof(UA_Nodestore));
	if(uaDefaultStore == NULL){ //Never gone happen based on current implementation
    	UA_NodestoreSwitch_deleteSwitch(storeSwitch);
    	ov_string_setvalue(errorText, "UA_Nodestore malloc failed. Not enough heap memory.");
    	return OV_ERR_GENERIC;
    }
	//TODO move to own function for reuse (~UA_Nodestore_copy(ns_old, ns_new))
    uaDefaultStore->context = config->nodestore.context;
    uaDefaultStore->deleteNode = config->nodestore.deleteNode;
    uaDefaultStore->deleteNodestore = config->nodestore.deleteNodestore;
    uaDefaultStore->getNode = config->nodestore.getNode;
    uaDefaultStore->getNodeCopy = config->nodestore.getNodeCopy;
    uaDefaultStore->inPlaceEditAllowed = config->nodestore.inPlaceEditAllowed;
    uaDefaultStore->insertNode = config->nodestore.insertNode;
    uaDefaultStore->iterate = config->nodestore.iterate;
    uaDefaultStore->newNode = config->nodestore.newNode;
    uaDefaultStore->releaseNode = config->nodestore.releaseNode;
    uaDefaultStore->removeNode = config->nodestore.removeNode;
    uaDefaultStore->replaceNode = config->nodestore.replaceNode;

    // add ua default store for namespace 0 and 1
    retval = UA_NodestoreSwitch_linkNodestoreToNamespace(storeSwitch, uaDefaultStore, 0);
	if(retval != UA_STATUSCODE_GOOD){ //Never gone happen based on current implementation
    	UA_NodestoreSwitch_deleteSwitch(storeSwitch);
    	UA_free(uaDefaultStore);
		ov_string_print(errorText, "UA_NodestoreSwitch_linkNodestoreToNamespace failed: %s" , UA_StatusCode_name(retval));
    	return OV_ERR_GENERIC;
    }
    retval = UA_NodestoreSwitch_linkNodestoreToNamespace(storeSwitch, uaDefaultStore, 1);
	if(retval != UA_STATUSCODE_GOOD){ //Never gone happen based on current implementation
    	UA_NodestoreSwitch_deleteSwitch(storeSwitch);
    	UA_free(uaDefaultStore);
		ov_string_print(errorText, "UA_NodestoreSwitch_linkNodestoreToNamespace failed: %s" , UA_StatusCode_name(retval));
    	return OV_ERR_GENERIC;
    }

    //link default to switch
	//TODO change due to newNode problem
    retval = UA_NodestoreSwitch_linkDefaultNodestore(storeSwitch, uaDefaultStore);
	if(retval != UA_STATUSCODE_GOOD){ //Never gone happen based on current implementation
    	UA_NodestoreSwitch_deleteSwitch(storeSwitch);
    	UA_free(uaDefaultStore);
		ov_string_print(errorText, "UA_NodestoreSwitch_linkDefaultNodestore failed: %s" , UA_StatusCode_name(retval));
    	return OV_ERR_GENERIC;
    }

	//link switch to config
	UA_NodestoreSwitch_linkNodestoreSwitchToServer(storeSwitch, &config->nodestore);
	return OV_ERR_OK;
}


//TODO move server startup and shutdown to typemethod?
//TODO write some static functions and proper error clean up (goto error/cleanup)
OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_run_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_BOOL  value
) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	OV_RESULT result = OV_ERR_OK;
	if(pobj->v_error)
		return OV_ERR_GENERIC;

	if(value != pobj->v_run){
		if(value){ //start server

			//Create new config
		    UA_ServerConfig* config = NULL;

		    //Update config from ov object
		    OV_INSTPTR_opcua_uaServerConfig pOvConfig = Ov_GetFirstChild(opcua_uaConfigToServer, pobj);
		    if(pOvConfig){
				//Create new config
			    config = UA_ServerConfig_new_minimal(pOvConfig->v_port, NULL);
			    //TODO update other config values
		    }else{
		    	config = UA_ServerConfig_new_default();
		    }
		    config->logger = ov_UAlogger_new();

		    //Check that config was created
		    if(!config){
	            ov_string_setvalue(&pobj->v_errorText, "UA_ServerConfig_new_default failed.");
	            pobj->v_error = TRUE;
	            return OV_ERR_GENERIC;
		    }

		    //Create a new switch and link to config
		    result = opcua_switch_new(config, &pobj->v_errorText);
			if(result != OV_ERR_OK){
		    	UA_ServerConfig_delete(config);
		    	pobj->v_error = TRUE;
		    	return result;
		    }

		    //Create new server
		    UA_Server *server = UA_Server_new(config);
		    if(!server){
		        UA_ServerConfig_delete(config);
	            ov_string_setvalue(&pobj->v_errorText, "UA_Server_new failed.");
	            pobj->v_error = TRUE;
	            return OV_ERR_GENERIC;
		    }

			//Link generic ov interface interface to server at first association //TODO move to constructor
			OV_INSTPTR_opcua_uaInterface pGenericInterface = Ov_PtrUpCast(opcua_uaInterface, Ov_GetPartPtr(genericInterface, pobj));
			Ov_LinkPlaced(opcua_uaServerToInterfaces, pobj, pGenericInterface, OV_PMH_BEGIN);

		    //Startup server
			retval = UA_Server_run_startup(server);
			if(retval != UA_STATUSCODE_GOOD){
				UA_Server_delete(server);
			    UA_ServerConfig_delete(config);
				ov_string_print(&pobj->v_errorText, "UA_Server_run_startup failed: %s" , UA_StatusCode_name(retval));
				pobj->v_error = TRUE;
				return OV_ERR_GENERIC;
			}

			//TODO link interfaces
			//TODO use Call makro?
			//OV_VTBLPTR_opcua_ovInterface pVtblGenericInterface = NULL;
			//Ov_GetVTablePtr(opcua_uaInterface, pVtblGenericInterface, pGenericInterface);
			//if(pVtblGenericInterface){
			//	pVtblGenericInterface->m_load(pGenericInterface);
			//}

			pobj->v_server = server;
			pobj->v_isRunning = TRUE;

		}else{ //shutdown server

			//Shutdown server
			UA_ServerConfig* config = UA_Server_getConfig(pobj->v_server);
			retval = UA_Server_run_shutdown(pobj->v_server); //Always returns good

			//Delete config and server
		    //TODO unload interfaces
			UA_ServerConfig_delete(config);
			UA_Server_delete(pobj->v_server);
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

OV_DLLFNCEXPORT OV_RESULT opcua_uaServer_reset_set(
    OV_INSTPTR_opcua_uaServer          pobj,
    const OV_BOOL  value
) {
    if(value){
    	pobj->v_error = FALSE;
    	opcua_uaServer_run_set(pobj, FALSE);
    }
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_uaServer_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    //Restore running state from shutdown
    opcua_uaServer_run_set(pinst, pinst->v_isRunning);

    return;
}

OV_DLLFNCEXPORT void opcua_uaServer_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, pobj);

    /* do what */
    OV_BOOL isRunning = pinst->v_isRunning; //Save running state
    opcua_uaServer_run_set(pinst, FALSE);
    pinst->v_isRunning = isRunning; //Restore running state

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT OV_ACCESS opcua_uaServer_getaccess(
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

OV_DLLFNCEXPORT void opcua_uaServer_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_opcua_uaServer pinst = Ov_StaticPtrCast(opcua_uaServer, this);
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

