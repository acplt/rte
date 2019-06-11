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

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#define OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#endif

#include "lifeCycleEntryOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "nodeset_lifeCycleEntry.h"
#include "ua_lifeCycleEntry_generated.h"
#include "ua_lifeCycleEntry_generated_handling.h"
#include "opcua_ovTrafo.h"
#include "opcua_ovStore.h"

OV_DLLFNCEXPORT UA_StatusCode lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntryNodeToOPCUA(
		void *context, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	UA_NodeClass 			nodeClass;
	OV_ELEMENT				element;
	OV_INSTPTR_lifeCycleEntryOPCUAInterface_interface 	pinterface = Ov_StaticPtrCast(lifeCycleEntryOPCUAInterface_interface, context);

	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_helpers_getVtblPointerAndCheckAccess(&(element), &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	nodeClass = UA_NODECLASS_VARIABLE;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_VariableNode));


	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(pobj->v_identifier);
	qName.namespaceIndex = nodeId->namespaceIndex;
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &element);
	UA_LocalizedText lText;
	UA_LocalizedText_init(&lText);
	lText.locale = UA_String_fromChars("en");
	if(tempString){
		lText.text = UA_String_fromChars(tempString);
	} else {
		lText.text = UA_String_fromChars("");
	}
	UA_LocalizedText_copy(&lText,&newNode->description);
	UA_LocalizedText_deleteMembers(&lText);

	// DisplayName
	UA_LocalizedText displayName;
	UA_LocalizedText_init(&displayName);
	displayName.locale = UA_String_fromChars("en");
	displayName.text = UA_String_fromChars(pobj->v_identifier);
	UA_LocalizedText_copy(&displayName, &newNode->displayName);
	UA_LocalizedText_deleteMembers(&displayName);

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

	// Variable specific attributes
	// arrayDemensions
	((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
	((UA_VariableNode*)newNode)->arrayDimensions = NULL; // UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/

	// valuerank
	((UA_VariableNode*)newNode)->valueRank = -1;	/*	scalar	*/


	// value
	OV_ELEMENT tmpPart;
	tmpPart.elemtype = OV_ET_NONE;
	OV_ELEMENT tmpParrent;
	tmpParrent.pobj = pobj;
	tmpParrent.elemtype = OV_ET_OBJECT;
	UA_LifeCycleEntry tmpLifeCycleEntry;
	UA_LifeCycleEntry_init(&tmpLifeCycleEntry);
	do {
		ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
		if (tmpPart.elemtype == OV_ET_NONE)
			break;
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceIdString") == OV_STRCMP_EQUAL){
			if (*(OV_STRING*)tmpPart.pvalue != NULL)
				tmpLifeCycleEntry.creatingInstance.idSpec = UA_String_fromChars(*(OV_STRING*)tmpPart.pvalue);
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceIdType") == OV_STRCMP_EQUAL){
			tmpLifeCycleEntry.creatingInstance.idType = *(UA_UInt32*)tmpPart.pvalue;
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceIdString") == OV_STRCMP_EQUAL){
			if (*(OV_STRING*)tmpPart.pvalue != NULL)
				tmpLifeCycleEntry.writingInstance.idSpec = UA_String_fromChars(*(OV_STRING*)tmpPart.pvalue);
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceIdType") == OV_STRCMP_EQUAL){
			tmpLifeCycleEntry.writingInstance.idType = *(UA_UInt32*)tmpPart.pvalue;
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Data") == OV_STRCMP_EQUAL){
			opcua_helpers_ovAnyToUAVariant((OV_ANY*)tmpPart.pvalue, &tmpLifeCycleEntry.data.value);
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "TimeStamp") == OV_STRCMP_EQUAL){
			tmpLifeCycleEntry.data.sourceTimestamp = ov_ovTimeTo1601nsTime(*(OV_TIME*)tmpPart.pvalue);
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Subject") == OV_STRCMP_EQUAL){
			if (*(OV_STRING*)tmpPart.pvalue != NULL)
				tmpLifeCycleEntry.subject = UA_String_fromChars(*(OV_STRING*)tmpPart.pvalue);
			continue;
		}
		if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "EventClass") == OV_STRCMP_EQUAL){
			if (*(OV_STRING*)tmpPart.pvalue != NULL)
				tmpLifeCycleEntry.eventClass = UA_String_fromChars(*(OV_STRING*)tmpPart.pvalue);
			continue;
		}else{
			tmpLifeCycleEntry.id =  atoi(tmpPart.pobj->v_identifier);
			break;
		}
	} while(TRUE);


	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY];
	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_LifeCycleEntry_new();
	if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
		UA_LifeCycleEntry_deleteMembers(&tmpLifeCycleEntry);
		return result;
	}
	((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
	((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
	UA_LifeCycleEntry_copy(&tmpLifeCycleEntry, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
	UA_LifeCycleEntry_deleteMembers(&tmpLifeCycleEntry);


	// accessLevel
	UA_Byte accessLevel = 0;
	if(access & OV_AC_READ){
		accessLevel |= (1<<0);
	}
	if(access & OV_AC_WRITE){
		accessLevel |= (1<<1);
	}
	((UA_VariableNode*)newNode)->accessLevel = accessLevel;
	// minimumSamplingInterval
	((UA_VariableNode*)newNode)->minimumSamplingInterval = -1;
	// historizing
	((UA_VariableNode*)newNode)->historizing = UA_FALSE;
	// dataType
	((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(pinterface->v_index, UA_NSLIFECYCLEENTRYID_LIFECYCLEENTRY);


	// References
	OV_UINT direction = OPCUA_OVTRAFO_ADDHASPROPERTY_FORWARD
						|	OPCUA_OVTRAFO_ADDHASPROPERTY_BACKWARD
						|	OPCUA_OVTRAFO_ADDHASCOMPONENT_FORWARD
						|	OPCUA_OVTRAFO_ADDHASCOMPONENT_BACKWARD
						|	OPCUA_OVTRAFO_ADDORGANIZES_FORWARD
						|	OPCUA_OVTRAFO_ADDORGANIZES_BACKWARD
						|	OPCUA_OVTRAFO_ADDHASSUBTYPE_FORWARD
						|	OPCUA_OVTRAFO_ADDHASSUBTYPE_BACKWARD;
	opcua_ovTrafo_addReferences(Ov_StaticPtrCast(opcua_interface, context), newNode, direction);

//	// References
//	OV_STRING	tmpString = NULL;
	UA_ExpandedNodeId NodeId;
//	// Parent
//	OV_INSTPTR_ov_object pParent = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobj));
//	ov_memstack_lock();
//	if (Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pParent)){
//		ov_string_setvalue(&tmpString, ov_path_getcanonicalpath(pParent, 2));
//		ov_string_append(&tmpString, "|||LifeCyleEntries");
//		NodeId = UA_EXPANDEDNODEID_STRING_ALLOC(OPCUA_OVTRAFO_DEFAULTNSINDEX, tmpString);
//		opcua_helpers_addReference(newNode, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
//					NodeId, UA_NODECLASS_OBJECT, UA_FALSE);
//		UA_ExpandedNodeId_deleteMembers(&NodeId);
//		ov_string_setvalue(&tmpString, NULL);
//	}else{
//		tmpString = ov_path_getcanonicalpath(pParent, 2);
//		NodeId = UA_EXPANDEDNODEID_STRING_ALLOC(OPCUA_OVTRAFO_DEFAULTNSINDEX, tmpString);
//		opcua_helpers_addReference(newNode, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
//					NodeId, UA_NODECLASS_OBJECT, UA_FALSE);
//		UA_ExpandedNodeId_deleteMembers(&NodeId);
//	}
//	ov_memstack_unlock();



	// Type
	NodeId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE);
	opcua_helpers_addReference(newNode, NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
			NodeId, UA_NODECLASS_OBJECTTYPE,
			UA_TRUE);

	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}

