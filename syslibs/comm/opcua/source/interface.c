
/******************************************************************************
*
*   FILE
*   ----
*   uaInterface.c
*
*   History
*   -------
*   2019-01-24   File created
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
#include "opcua_ovStore.h"
#include "opcua_helpers.h"



OV_DLLFNCEXPORT OV_RESULT opcua_interface_uri_set(
    OV_INSTPTR_opcua_interface          pobj,
    const OV_STRING  value
) {
	// Check for valid namespace uri
	if(ov_string_getlength(value) == 0){
		return OV_ERR_BADNAME;
	}
	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	// Change trafo

	if(server != NULL && server->v_isRunning){
		UA_String trafoUri = UA_String_fromChars(value);
		UA_String oldUri = UA_String_fromChars(pobj->v_uri);
		opcua_interface_setNamespace(server->v_server, oldUri, trafoUri, NULL);
		// Change internal URIs
		ov_string_setvalue(&pobj->v_uri, value);
	}
    return OV_ERR_OK;
}


//TODO use macro
OV_DLLFNCEXPORT OV_ACCESS opcua_interface_getaccess(
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


//TODO subscribe to changes of namespace array and update v_trafo.index and v_types.index
OV_DLLFNCEXPORT OV_RESULT opcua_interface_load(OV_INSTPTR_opcua_interface pobj, OV_BOOL forceLoad) {

	UA_StatusCode result = UA_STATUSCODE_GOOD;
	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server == NULL){
		return OV_ERR_GENERIC;
	}

	//Load all dependent interfaces first
	OV_INSTPTR_opcua_interface dependentInterface = NULL;
	Ov_ForEachChild(opcua_interfaceDependency, pobj, dependentInterface){
		opcua_interface_load(dependentInterface, FALSE);
		//TODO error handling
	}

	size_t index = 0;
	UA_String uri = UA_String_fromChars(pobj->v_uri);
	if(!forceLoad){
		if(UA_Server_getNamespaceByName(server->v_server, uri, &index) == UA_STATUSCODE_GOOD){
			UA_String_deleteMembers(&uri);
			return OV_ERR_ALREADYEXISTS;
		}
	}
	UA_ServerConfig * config = UA_Server_getConfig(server->v_server);

	// Add NamespaceUri
	// Try to replace empty namespace otherwise add to the end
	pobj->v_index = UA_Server_addNamespace(server->v_server, pobj->v_uri);

	UA_String_deleteMembers(&uri);

	// Add Datatypes
	if(pobj->v_dataTypes){
		pobj->v_dataTypes->next = config->customDataTypes;
		config->customDataTypes = pobj->v_dataTypes;
	}

	UA_Nodestore_Switch *nsSwitch = UA_Server_getNodestore(server->v_server);
	// Link the nodestore (transformation) to the namespace in the switch
	if(pobj->v_store){
		result = UA_Nodestore_Switch_linkNodestoreToNamespace(nsSwitch, pobj->v_store, pobj->v_index);
		if (result != UA_STATUSCODE_GOOD){
			return OV_ERR_GENERIC;
		}
	}

	// Load predefined nodeset
	OV_VTBLPTR_opcua_interface pVtable = NULL;
	Ov_GetVTablePtr(opcua_interface, pVtable, pobj);
	result = pVtable->m_nodeset(server->v_server);
	if (result != UA_STATUSCODE_GOOD){
		return OV_ERR_GENERIC;
	}
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_unload(OV_INSTPTR_opcua_interface pobj) {

	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server == NULL){
		return OV_ERR_GENERIC;
	}

	// unload all dependent interfaces
	OV_INSTPTR_opcua_interface dependentInterface = Ov_GetParent(opcua_interfaceDependency, pobj);
	if(dependentInterface != NULL){
		opcua_interface_unload(dependentInterface);
		//TODO error handling
	}

	// unload Types
	// Remove Datatypes
	UA_ServerConfig * config = UA_Server_getConfig(server->v_server);
	UA_DataTypeArray * dataTypes = config->customDataTypes;
	UA_DataTypeArray * lastDataTypes = NULL;
	while(dataTypes != NULL){
		if(dataTypes == pobj->v_dataTypes){
			if(lastDataTypes == NULL){
				config->customDataTypes = dataTypes->next;
			}else{
				lastDataTypes->next = dataTypes->next;
			}
			break;
		}
		lastDataTypes = dataTypes;
		dataTypes = dataTypes->next;
	}

	UA_Nodestore_Switch *nsSwitch = UA_Server_getNodestore(server->v_server);
	// Unlink the nodestore from the namespace
	UA_Nodestore_Switch_unlinkNodestoreFromNamespace(nsSwitch, pobj->v_store);
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkNode(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, OV_STRING virtualNodePath, void *context) {

    return TRUE;
}

OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkReference(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {

    return FALSE;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_interface_nodeset(UA_Server* server) {

    return (UA_StatusCode)0;
}
