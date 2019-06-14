/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_identificationOPCUAInterface
#define OV_COMPILE_LIBRARY_identificationOPCUAInterface
#endif

#include "identificationOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "ua_identification_generated.h"
#include "ua_identification_generated_handling.h"


static void identificationOPCUAInterface_interface_deleteNodestore(void *context){

}

static void identificationOPCUAInterface_interface_deleteNode(void *context, UA_Node *node){
	if (node){
		if (node->nodeClass == UA_NODECLASS_METHOD){
			ov_string_setvalue((OV_STRING*) (&(((UA_MethodNode*) node)->context)),	NULL);
		}
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}
static void identificationOPCUAInterface_interface_releaseNode(void *context, const UA_Node *node){
	identificationOPCUAInterface_interface_deleteNode(context, (UA_Node*)node);
}

static UA_Node * identificationOPCUAInterface_interface_newNode(void *context, UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * identificationOPCUAInterface_interface_getNode(void *context, const UA_NodeId *nodeId){
	UA_Node * tmpNode = NULL;
	UA_Node* opcuaNode = NULL;
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;

	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &tmpString);


	pobj = ov_path_getobjectpointer(tmpString, 2);
	ov_string_setvalue(&tmpString, NULL);
	if (pobj == NULL){
		return NULL;
	}

	if (Ov_CanCastTo(identification_Identification, pobj)){
		if (identificationOPCUAInterface_interface_ovIdentificationNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}


	return tmpNode;
}
static UA_StatusCode identificationOPCUAInterface_interface_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) identificationOPCUAInterface_interface_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode identificationOPCUAInterface_interface_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode identificationOPCUAInterface_interface_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_helpers_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return identificationOPCUAInterface_interface_insert(handle, node, parrentNode);
}
static UA_StatusCode identificationOPCUAInterface_interface_replaceNode(void *context, UA_Node *node){
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;

	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(node->nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	result = opcua_helpers_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), &pobj, &pVtblObj, &access);
	ov_memstack_unlock();
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	if (pobj){
		if (Ov_CanCastTo(identification_Identification, pobj)){
			OV_ELEMENT tmpPart;
			tmpPart.elemtype = OV_ET_NONE;
			OV_ELEMENT tmpParrent;
			tmpParrent.pobj = pobj;
			tmpParrent.elemtype = OV_ET_OBJECT;
			do {
				ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
				if (tmpPart.elemtype == OV_ET_NONE)
					break;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Value") == OV_STRCMP_EQUAL)
					opcua_helpers_UAVariantToOVAny(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value), (OV_ANY*)tmpPart.pvalue);
			} while(TRUE);
		}
	}
	return result;
}
static void identificationOPCUAInterface_interface_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){

}


UA_NodestoreInterface* identificationOPCUAInterface_interface_ovNodeStoreInterfaceIdentificationNew(OV_INSTPTR_identificationOPCUAInterface_interface context) {
	UA_NodestoreInterface* trafo = UA_malloc(sizeof(UA_NodestoreInterface));
	if(trafo == NULL)
		return NULL;
	trafo->context =          context;
	trafo->newNode =       	  identificationOPCUAInterface_interface_newNode;
	trafo->deleteNode =    	  identificationOPCUAInterface_interface_deleteNode;
	trafo->deleteNodestore =  identificationOPCUAInterface_interface_deleteNodestore;

	trafo->getNode =          identificationOPCUAInterface_interface_getNode;
	trafo->releaseNode =      identificationOPCUAInterface_interface_releaseNode;

	trafo->getNodeCopy =      identificationOPCUAInterface_interface_getCopyNode;
	trafo->insertNode =       identificationOPCUAInterface_interface_insertNode;
	trafo->replaceNode =      identificationOPCUAInterface_interface_replaceNode;

	trafo->removeNode =       identificationOPCUAInterface_interface_removeNode;

	trafo->iterate =          identificationOPCUAInterface_interface_iterate;
	return trafo;
}

void identificationOPCUAInterface_interface_ovNodeStoreInterfaceIdentificationDelete(UA_NodestoreInterface * store){
	store->context = NULL;
	UA_free(store);
}