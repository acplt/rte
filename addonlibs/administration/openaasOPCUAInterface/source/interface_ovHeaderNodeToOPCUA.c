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

extern OV_INSTPTR_openaasOPCUAInterface_interface pinterface;



OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_ovHeaderNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	UA_NodeClass 			nodeClass;
	OV_ELEMENT				element;

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(element), pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	nodeClass = UA_NODECLASS_OBJECT;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_ObjectNode));


	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(pobj->v_identifier);
	qName.namespaceIndex = pinterface->v_interfacenamespace.index;
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &(element));
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
	lText.text = UA_String_fromChars(pobj->v_identifier);
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

	((UA_ObjectNode*)newNode)->eventNotifier = 0;

	// References
	OV_INSTPTR_ov_object pchild = NULL;
	size_t size_references = 0;
	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,pobj), pchild) {
		size_references++;
	}

	size_references = size_references + 1;// For Config

	size_references = size_references + 2;// For Parent&TypeNode
	newNode->references = UA_calloc(size_references, sizeof(UA_ReferenceNode));
	if (!newNode->references){
		result = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
		UA_free(newNode);
		return result;
	}
	newNode->referencesSize = size_references;

	// Parent Node
	newNode->references[0].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
	newNode->references[0].isInverse = UA_TRUE;
	OV_UINT len = 0;
	OV_STRING *plist = NULL;
	OV_STRING tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, ".", &len);
	ov_string_setvalue(&tmpString, NULL);
	for (OV_UINT i = 0; i < len-1; i++){
		if (i != 0)
			ov_string_append(&tmpString, ".");
		ov_string_append(&tmpString, plist[i]);
	}
	newNode->references[0].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, tmpString);
	ov_string_freelist(plist);
	ov_string_setvalue(&tmpString, NULL);

	// TypeNode
	newNode->references[1].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	newNode->references[1].isInverse = UA_FALSE;
	newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);

	// Config
	newNode->references[2].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	newNode->references[2].isInverse = UA_FALSE;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	ov_string_append(&tmpString, ".Config");
	newNode->references[2].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, tmpString);
	ov_string_setvalue(&tmpString, NULL);

	size_t i = 2;
	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,pobj), pchild) {
		i++;
		if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatementList, pchild)){
			newNode->references[i].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
			newNode->references[i].isInverse = UA_FALSE;
			OV_INSTPTR_propertyValueStatement_PropertyValueStatementList pref =
									Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatementList,pchild);
			tmpString = NULL;
			copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
			ov_string_append(&tmpString, "/");
			ov_string_append(&tmpString, pref->v_identifier);
			newNode->references[i].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, tmpString);
			ov_string_setvalue(&tmpString, NULL);
		}
	}

	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}

