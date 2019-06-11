
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

//TODO maybe its better to make a list of all nodeIds and remove them afterwards
static void removeAllNodes(void *visitorCtx, const UA_Node *node){
	UA_NodestoreInterface* nsi = (UA_NodestoreInterface*)visitorCtx;
	nsi->removeNode(nsi->context, &node->nodeId);
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


OV_DLLFNCEXPORT OV_RESULT opcua_interface_load(OV_INSTPTR_opcua_interface pobj, OV_BOOL forceLoad) {

	UA_StatusCode result = UA_STATUSCODE_GOOD;
	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server == NULL){
		return OV_ERR_GENERIC;
	}

	// Check whether an interface with same uri is already linked
	if(!forceLoad){
		OV_INSTPTR_opcua_interface pInterfaceTemp = NULL;
		Ov_ForEachChild(opcua_serverToInterfaces, server, pInterfaceTemp){
			if(pobj != pInterfaceTemp &&
					ov_string_compare(pobj->v_uri, pInterfaceTemp->v_uri) == OV_STRCMP_EQUAL){
				return OV_ERR_ALREADYEXISTS;
			}
		}
	}

	//Load all dependent interfaces first
	OV_INSTPTR_opcua_interface dependentInterface = NULL;
	Ov_ForEachChild(opcua_interfaceDependency, pobj, dependentInterface){
		opcua_interface_load(dependentInterface, FALSE);
		//TODO error handling
	}


	// Add NamespaceUri
	// Get the namespace index if already added, otherwise add to the end
	pobj->v_index = UA_Server_addNamespace(server->v_server, pobj->v_uri);

	// Add Datatypes
	UA_ServerConfig * config = UA_Server_getConfig(server->v_server);
	if(pobj->v_dataTypes){
		pobj->v_dataTypes->next = config->customDataTypes;
		config->customDataTypes = pobj->v_dataTypes;
	}

	UA_Nodestore_Switch *nsSwitch = UA_Server_getNodestore(server->v_server);
	// Link the nodestore (transformation) to the namespace in the switch
	if(pobj->v_store){
		result = UA_Nodestore_Switch_setNodestore(nsSwitch, pobj->v_index, pobj->v_store);
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
	if (pobj->v_dataTypes){
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
	}

	UA_Nodestore_Switch *nsSwitch = UA_Server_getNodestore(server->v_server);
	// Unlink the nodestore from the namespace
	UA_Nodestore_Switch_changeNodestore(nsSwitch, pobj->v_store, NULL);

	// Remove all nodes from nodestore
	pobj->v_store->iterate(pobj->v_store->context, (UA_NodestoreVisitor)removeAllNodes, pobj->v_store);

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkNode(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, OV_STRING virtualNodePath, void *context) {
    return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkReference(OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
    return FALSE;
}

OV_DLLFNCEXPORT UA_StatusCode opcua_interface_nodeset(UA_Server* server) {
    return (UA_StatusCode)0;
}