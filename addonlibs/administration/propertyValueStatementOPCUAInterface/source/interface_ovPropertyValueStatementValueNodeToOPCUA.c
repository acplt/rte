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

#ifndef OV_COMPILE_LIBRARY_propertyValueStatementOPCUAInterface
#define OV_COMPILE_LIBRARY_propertyValueStatementOPCUAInterface
#endif

#include "propertyValueStatementOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "nodeset_propertyValueStatement.h"

extern OV_INSTPTR_propertyValueStatementOPCUAInterface_interface pinterface;




OV_DLLFNCEXPORT UA_StatusCode propertyValueStatementOPCUAInterface_interface_ovPropertyValueStatementValueNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	UA_NodeClass 			nodeClass;
	OV_STRING 				tmpString = NULL;
	OV_UINT 				len = 0;
	OV_STRING 				*plist = NULL;
	OV_ELEMENT				element;

	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, ".", &len);

	ov_memstack_lock();

	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	nodeClass = UA_NODECLASS_VARIABLE;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_VariableNode));

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(plist[len-1]);
	qName.namespaceIndex = pinterface->v_interfacenamespace.index;
	newNode->browseName = qName;

	// Description
	UA_LocalizedText lText;
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars("");
	newNode->description = lText;

	// DisplayName
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars(plist[len-1]);
	newNode->displayName = lText;

	// NodeId
	UA_NodeId_copy(nodeId, &newNode->nodeId);

	// NodeClass
	newNode->nodeClass 	= nodeClass;

	// WriteMask
	newNode->writeMask 	= 0;

	// Variable specific attributes
	// arrayDemensions
	((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
	((UA_VariableNode*)newNode)->arrayDimensions = NULL; // UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/

	// valuerank
	((UA_VariableNode*)newNode)->valueRank = 1;	/*	one dimension	*/


	// value

	switch(((OV_ANY*)element.pvalue)->value.vartype & OV_VT_KSMASK){
		case OV_VT_BOOL:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_BOOLEAN];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Boolean_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_Boolean_copy((UA_Boolean*)&((OV_ANY*)element.pvalue)->value.valueunion.val_bool, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_BOOLEAN);
			break;
		case OV_VT_INT:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_INT32];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Int32_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_Int32_copy(&((OV_ANY*)element.pvalue)->value.valueunion.val_int, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_INT32);
			break;
		case OV_VT_UINT:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_UINT32];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_UInt32_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_UInt32_copy(&((OV_ANY*)element.pvalue)->value.valueunion.val_uint, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_UINT32);
			break;
		case OV_VT_SINGLE:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_FLOAT];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Float_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_Float_copy(&((OV_ANY*)element.pvalue)->value.valueunion.val_single, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_FLOAT);
			break;
		case OV_VT_DOUBLE:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_DOUBLE];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Double_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_Double_copy(&((OV_ANY*)element.pvalue)->value.valueunion.val_double, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_DOUBLE);
			break;
		case OV_VT_STRING:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_STRING];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_String_new();
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			UA_String tmpUAString = UA_String_fromChars(((OV_ANY*)element.pvalue)->value.valueunion.val_string);
			UA_String_copy(&tmpUAString, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
			UA_String_deleteMembers(&tmpUAString);
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_STRING);
			break;
		case OV_VT_BOOL_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_BOOLEAN];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_bool_vec.veclen, &UA_TYPES[UA_TYPES_BOOLEAN]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_bool_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_bool_vec.veclen; i++){
				((UA_Boolean*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] = ((OV_ANY*)element.pvalue)->value.valueunion.val_bool_vec.value[i];
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_BOOLEAN);
			break;
		case OV_VT_INT_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_INT32];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_int_vec.veclen, &UA_TYPES[UA_TYPES_INT32]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_int_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_int_vec.veclen; i++){
				((UA_Int32*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] = ((OV_ANY*)element.pvalue)->value.valueunion.val_int_vec.value[i];
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_INT32);
			break;
		case OV_VT_UINT_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_UINT32];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_uint_vec.veclen, &UA_TYPES[UA_TYPES_UINT32]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_uint_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_uint_vec.veclen; i++){
				((UA_UInt32*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] = ((OV_ANY*)element.pvalue)->value.valueunion.val_uint_vec.value[i];
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_UINT32);
			break;
		case OV_VT_SINGLE_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_FLOAT];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_single_vec.veclen, &UA_TYPES[UA_TYPES_FLOAT]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_single_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_single_vec.veclen; i++){
				((UA_Float*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] = ((OV_ANY*)element.pvalue)->value.valueunion.val_single_vec.value[i];
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_FLOAT);
			break;
		case OV_VT_DOUBLE_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_DOUBLE];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_double_vec.veclen, &UA_TYPES[UA_TYPES_DOUBLE]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_double_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;
			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_double_vec.veclen; i++){
				((UA_Double*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] = ((OV_ANY*)element.pvalue)->value.valueunion.val_double_vec.value[i];
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_DOUBLE);
			break;
		case OV_VT_STRING_VEC:
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_TYPES[UA_TYPES_STRING];
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Array_new(((OV_ANY*)element.pvalue)->value.valueunion.val_string_vec.veclen, &UA_TYPES[UA_TYPES_STRING]);
			((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->arrayLength = ((OV_ANY*)element.pvalue)->value.valueunion.val_string_vec.veclen;
			if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
				result = UA_STATUSCODE_BADOUTOFMEMORY;
				return result;
			}
			((UA_VariableNode*)newNode)->value.data.value.hasValue = TRUE;
			((UA_VariableNode*)newNode)->valueSource = UA_VALUESOURCE_DATA;

			for (OV_UINT i = 0; i < ((OV_ANY*)element.pvalue)->value.valueunion.val_string_vec.veclen; i++){
				((UA_String*)((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data)[i] =  UA_String_fromChars(((OV_ANY*)element.pvalue)->value.valueunion.val_string_vec.value[i]);
			}
			// dataType
			((UA_VariableNode*)newNode)->dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_STRING);
			break;
		default:
			break;
	}

	// accessLevel
	UA_Byte accessLevel = 0;
	((UA_VariableNode*)newNode)->accessLevel = accessLevel;
	// minimumSamplingInterval
	((UA_VariableNode*)newNode)->minimumSamplingInterval = -1;
	// historizing
	((UA_VariableNode*)newNode)->historizing = UA_FALSE;


	// References
	addReference(newNode);
	UA_NodeId tmpNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	for (size_t i = 0; i < newNode->referencesSize; i++){
		if (UA_NodeId_equal(&newNode->references[i].referenceTypeId, &tmpNodeId)){
			newNode->references[i].targetId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE);
			break;
		}
	}
	UA_NodeId_deleteMembers(&tmpNodeId);

	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}

