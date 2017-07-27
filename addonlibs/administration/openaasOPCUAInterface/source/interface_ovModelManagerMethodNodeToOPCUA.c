/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_openaasOPCUAInterface
#define OV_COMPILE_LIBRARY_openaasOPCUAInterface
#endif

#include "openaasOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "nodeset_openaas.h"
#include "libov/ov_element.h"

extern OV_INSTPTR_openaasOPCUAInterface_interface pinterface;



OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_ovModelManagerMethodNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
/*	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	UA_NodeClass 			nodeClass;
	OV_STRING 				tmpString = NULL;
	OV_UINT 				len = 0;
	OV_UINT 				len2 = 0;
	OV_STRING 				*plist = NULL;
	OV_STRING 				*plist2 = NULL;
	OV_ELEMENT				element;

	if (pinterface == NULL)
		return UA_STATUSCODE_BADOUTOFSERVICE;

	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "||", &len);
	plist2 = ov_string_split(plist[1], ".", &len2);
	ov_string_setvalue(&tmpString, NULL);

	UA_NodeId tmpNodeId;
	UA_NodeId_init(&tmpNodeId);
	tmpNodeId.namespaceIndex = nodeId->namespaceIndex;
	tmpNodeId.identifierType = nodeId->identifierType;

	if (ov_string_compare(plist2[0], "startGetAssetLCEData") == OV_STRCMP_EQUAL || ov_string_compare(plist2[0], "stopGetAssetLCEData") == OV_STRCMP_EQUAL){
		ov_memstack_lock();
		OV_STRING tmpString2 = ov_path_getcanonicalpath(Ov_DynamicPtrCast(ov_object, pinterface), 2);
		tmpNodeId.identifier.string = UA_String_fromChars(tmpString2);
		ov_memstack_unlock();
	}else{
		tmpNodeId.identifier.string = UA_String_fromChars(plist[0]);
	}
	ov_string_freelist(plist);
	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path);
	UA_NodeId_deleteMembers(&tmpNodeId);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		ov_string_freelist(plist2);
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&element, pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		ov_string_freelist(plist2);
		return result;
	}


	nodeClass = UA_NODECLASS_METHOD;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_MethodNode));

	OV_ELEMENT tmpElement;
	tmpElement.elemtype = OV_ET_NONE;
	tmpElement.pobj = NULL;
	ov_element_searchpart(&element, &tmpElement, OV_ET_OPERATION, plist2[0]);
	ov_string_freelist(plist2);
	if (tmpElement.pobj == NULL){
		return OV_ERR_BADPATH;
	}

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(pobj->v_identifier);
	qName.namespaceIndex = pinterface->v_interfacenamespace.index;
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &element);
	UA_LocalizedText lText;
	lText.locale = UA_String_fromChars("en");
	if(tempString){
		lText.text = UA_String_fromChars(tempString);
	} else {
		lText.text = UA_String_fromChars("");
	}
	newNode->description = lText;

	// DisplayName
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars(tmpElement.elemunion.pop->v_identifier);
	newNode->displayName = lText;

	// NodeId
	UA_NodeId_copy(nodeId, &newNode->nodeId);

	// NodeClass
	newNode->nodeClass 	= nodeClass;

	// WriteMask
	UA_UInt32 writeMask = 0;
	if(element.elemtype != OV_ET_VARIABLE){
		if(access & OV_AC_WRITE){
			writeMask |= (1<<2);	//	BrowseName
			writeMask |= (1<<6);	//	DisplayName
		}
		if(access & OV_AC_RENAMEABLE){
			writeMask |= (1<<14);	//	NodeId
		}
	}
	newNode->writeMask 	= writeMask;

	((UA_MethodNode*)newNode)->executable = TRUE;
	((UA_MethodNode*)newNode)->attachedMethod = openaasOPCUAInterface_interface_MethodCallbackModelmanager;
	((UA_MethodNode*)newNode)->methodHandle = tmpElement.elemunion.pop->v_identifier;

	// References
	size_t size_references = 0;

	// Input/OutputArgument
	size_references = size_references + 2;

	size_references = size_references + 2;// For Parent&Type
	newNode->references = UA_calloc(size_references, sizeof(UA_ReferenceNode));
	if (!newNode->references){
		result = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
		UA_free(newNode);
		return result;
	}
	newNode->referencesSize = size_references;

	// ParentNode
	newNode->references[0].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
	newNode->references[0].isInverse = UA_TRUE;
	len = 0;
	plist = NULL;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "||", &len);
	newNode->references[0].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, plist[0]);
	ov_string_setvalue(&tmpString, NULL);
	ov_string_freelist(plist);

	// Type
	newNode->references[1].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	newNode->references[1].isInverse = UA_FALSE;

	// InputArguments
	newNode->references[2].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[2].isInverse = UA_FALSE;

	// OutputArguments
	newNode->references[3].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[3].isInverse = UA_FALSE;


	if (ov_string_compare(plist[1], "createAAS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEAAS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEAAS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEAAS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "deleteAAS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEAAS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEAAS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEAAS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "createLCE") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATELCE);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATELCE_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATELCE_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "deleteLCE") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETELCE);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETELCE_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETELCE_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "getLCE") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLCE);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLCE_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLCE_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "setLCE") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETLCE);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETLCE_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETLCE_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "createPVSL") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVSL);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVSL_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVSL_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "deletePVSL") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVSL);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVSL_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVSL_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "createPVS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_CREATEPVS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "deletePVS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_DELETEPVS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "getPVS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETPVS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETPVS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETPVS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "setPVS") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETPVS);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETPVS_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_SETPVS_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "getAASNodeId") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETAASNODEID);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETAASNODEID_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETAASNODEID_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "getLastLCEs") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLASTLCES);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLASTLCES_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_GETLASTLCES_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "triggerGetCoreData") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_TRIGGERGETCOREDATA);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_TRIGGERGETCOREDATA_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_TRIGGERGETCOREDATA_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "startGetAssetLCEData") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STARTGETASSETLCEDATA);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STARTGETASSETLCEDATA_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STARTGETASSETLCEDATA_OUTPUTARGUMENTS);
	}else if (ov_string_compare(plist[1], "stopGetAssetLCEData") == OV_STRCMP_EQUAL){
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STOPGETASSETLCEDATA);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STOPGETASSETLCEDATA_INPUTARGUMENTS);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(pinterface->v_modelnamespace.index, UA_NS2ID_STOPGETASSETLCEDATA_OUTPUTARGUMENTS);
	}else{
		newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(0,0);
		newNode->references[2].targetId = UA_EXPANDEDNODEID_NUMERIC(0,0);
		newNode->references[3].targetId = UA_EXPANDEDNODEID_NUMERIC(0,0);
	}
*/
	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}
