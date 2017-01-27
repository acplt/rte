
/******************************************************************************
*
*   FILE
*   ----
*   testObject.c
*
*   History
*   -------
*   2016-10-27   File created
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
#include "opcua_helpers.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"


OV_DLLFNCEXPORT OV_RESULT opcua_testObject_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_opcua_testObject pinst = Ov_StaticPtrCast(opcua_testObject, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    OV_INSTPTR_opcua_methodNode Node1 = NULL;
    OV_INSTPTR_ov_domain ptr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
    result = Ov_CreateObject(opcua_methodNode, Node1, ptr, "Node99");
    if (Node1 == NULL)
		return result;
    // BaseNodeAttribute
    OV_STRING tmpString = NULL;
    tmpString = "N99bn";
    ov_string_setvalue(&Node1->p_browseName.v_name, tmpString);
    tmpString = "N99desloc";
    ov_string_setvalue(&Node1->p_description.v_locale, tmpString);
    tmpString = "N99destxt";
    ov_string_setvalue(&Node1->p_description.v_text, tmpString);
    tmpString = "N99disloc";
    ov_string_setvalue(&Node1->p_displayName.v_locale, tmpString);
    tmpString = "N99distxt";
    ov_string_setvalue(&Node1->p_displayName.v_text, tmpString);
    Node1->p_nodeId.v_namespaceIndex = 99;
    Node1->p_nodeId.v_identifierType = UA_NODEIDTYPE_NUMERIC;
    Node1->p_nodeId.v_id.value.vartype = OV_VT_INT;
    Node1->p_nodeId.v_id.value.valueunion.val_uint = 99;
    Node1->v_nodeClass = 8;
    Node1->v_userWriteMask = 9;
    Node1->v_writeMask = 10;
    // specificAttributes
    Node1->v_executable = TRUE;
    Node1->v_userExecutable = TRUE;




    OV_INSTPTR_opcua_methodNode Node2 = NULL;
	OV_INSTPTR_ov_domain ptr2 = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
	result = Ov_CreateObject(opcua_methodNode, Node2, ptr2, "Node299");
	if (Node2 == NULL)
		return result;
	OV_STRING tmpString2 = NULL;
	tmpString2 = "N299";
	ov_string_setvalue(&Node2->p_browseName.v_name, tmpString2);
	tmpString2 = "N299desloc";
	ov_string_setvalue(&Node2->p_description.v_locale, tmpString2);
	tmpString2 = "N299destxt";
	ov_string_setvalue(&Node2->p_description.v_text, tmpString2);
	tmpString2 = "N299disloc";
	ov_string_setvalue(&Node2->p_displayName.v_locale, tmpString);
	tmpString2 = "N299distxt";
	ov_string_setvalue(&Node2->p_displayName.v_text, tmpString2);
	Node2->p_nodeId.v_namespaceIndex = 299;
	Node2->p_nodeId.v_identifierType = UA_NODEIDTYPE_NUMERIC;
	Node2->p_nodeId.v_id.value.vartype = OV_VT_INT;
	Node2->p_nodeId.v_id.value.valueunion.val_uint = 299;
	Node2->v_nodeClass = 28;
	Node2->v_userWriteMask = 29;
	Node2->v_writeMask = 210;
    // specificAttributes
//    Node2->v_valuerank 					= 211;
//    Node2->v_arrayDimensionsSize 		= 0;
//    OV_UINT_VEC tmpVec2;
//    tmpVec2.veclen = 0;
//    tmpVec2.value = NULL;
//    Node2->v_arrayDimensions			= tmpVec2;
//    Node2->v_valueSource 				= 212;
//    Node2->v_accessLevel 				= 213;
//    Node2->v_userAccessLevel 			= 214;
//    Node2->v_minimumSamplingInterval	= 215.1;
//    Node2->v_historizing 				= TRUE;
//    Node2->p_dataType.v_namespaceIndex 	= 216;
//    Node2->p_dataType.v_identifierType 	= UA_NODEIDTYPE_NUMERIC;
//    Node2->p_dataType.v_id.value.vartype = OV_VT_INT;
//    Node2->p_dataType.v_id.value.valueunion.val_uint = 216;
//    Node2->v_value.value.vartype = OV_VT_STRING;
//	ov_string_setvalue(&Node2->v_value.value.valueunion.val_string, "Variable2");
	Node1->v_executable = TRUE;
	Node1->v_userExecutable = TRUE;

	OV_INSTPTR_opcua_reference reference1 = NULL;
	result = createOpcuaReferenceNode(&Node1->p_references, &reference1, "reference1");
	if (reference1 == NULL)
		return result;
	reference1->p_referenceTypeNodeId.v_namespaceIndex = Node1->p_nodeId.v_namespaceIndex;
	reference1->p_referenceTypeNodeId.v_identifierType = Node1->p_nodeId.v_identifierType;
	reference1->p_referenceTypeNodeId.v_id.value.valueunion.val_uint = Node1->p_nodeId.v_id.value.valueunion.val_uint;
	reference1->p_targetNodeId.p_nodeId.v_namespaceIndex = Node2->p_nodeId.v_namespaceIndex;
	reference1->p_targetNodeId.p_nodeId.v_identifierType = Node2->p_nodeId.v_identifierType;
	reference1->p_targetNodeId.p_nodeId.v_id.value.valueunion.val_uint = Node2->p_nodeId.v_id.value.valueunion.val_uint;
	OV_INSTPTR_opcua_reference reference2 = NULL;
	result = createOpcuaReferenceNode(&Node1->p_references, &reference2, "reference2");
	if (reference2 == NULL)
		return result;
	reference2->p_referenceTypeNodeId.v_namespaceIndex = Node2->p_nodeId.v_namespaceIndex;
	reference2->p_referenceTypeNodeId.v_identifierType = Node2->p_nodeId.v_identifierType;
	reference2->p_referenceTypeNodeId.v_id.value.valueunion.val_uint = Node2->p_nodeId.v_id.value.valueunion.val_uint;
	reference2->p_targetNodeId.p_nodeId.v_namespaceIndex = Node1->p_nodeId.v_namespaceIndex;
	reference2->p_targetNodeId.p_nodeId.v_identifierType = Node1->p_nodeId.v_identifierType;
	reference2->p_targetNodeId.p_nodeId.v_id.value.valueunion.val_uint = Node1->p_nodeId.v_id.value.valueunion.val_uint;

	OV_INSTPTR_opcua_reference reference3 = NULL;
	result = createOpcuaReferenceNode(&Node2->p_references, &reference3, "reference3");
	if (reference3 == NULL)
		return result;
	reference3->p_referenceTypeNodeId.v_namespaceIndex = Node1->p_nodeId.v_namespaceIndex;
	reference3->p_referenceTypeNodeId.v_identifierType = Node1->p_nodeId.v_identifierType;
	reference3->p_referenceTypeNodeId.v_id.value.valueunion.val_uint = Node1->p_nodeId.v_id.value.valueunion.val_uint;
	reference3->p_targetNodeId.p_nodeId.v_namespaceIndex = Node2->p_nodeId.v_namespaceIndex;
	reference3->p_targetNodeId.p_nodeId.v_identifierType = Node2->p_nodeId.v_identifierType;
	reference3->p_targetNodeId.p_nodeId.v_id.value.valueunion.val_uint = Node2->p_nodeId.v_id.value.valueunion.val_uint;


    return result;
}
OV_DLLFNCEXPORT void opcua_testObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {

// test ovBaseNode->opcuaBaseNode->ovBaseNode
	UA_MethodNode *uaNode = NULL;
	OV_INSTPTR_opcua_methodNode ovNode = NULL;

	OV_INSTPTR_ov_domain ptr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
	if(!ptr)
		return;
	ptr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, ptr, "Node99"));
	if(!ptr)
		return;

	if(opcua_nodeStoreFunctions_ovMethodNodeToOPCUA(NULL,Ov_StaticPtrCast(opcua_methodNode,ptr), &uaNode)!=OV_ERR_OK){
		//copy object failed
		return;
	}


	if(opcua_nodeStoreFunctions_opcuaMethodNodeToOv(NULL,uaNode, &ovNode, NULL)!=OV_ERR_OK){
		//copy object failed
			return;
		}

	UA_MethodNode *uaNode2 = NULL;
	OV_INSTPTR_opcua_methodNode ovNode2 = NULL;

	OV_INSTPTR_ov_domain ptr2 = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
	if(!ptr2)
		return;
	ptr2 = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, ptr2, "Node299"));
	if(!ptr2)
		return;

	if(opcua_nodeStoreFunctions_ovMethodNodeToOPCUA(NULL,Ov_StaticPtrCast(opcua_methodNode,ptr2), &uaNode2)!=OV_ERR_OK){
		//copy object failed
		return;
	}


	if(opcua_nodeStoreFunctions_opcuaMethodNodeToOv(NULL,uaNode2, &ovNode2, NULL)!=OV_ERR_OK){
		//copy object failed
			return;
		}

    return;
}

OV_DLLFNCEXPORT void opcua_testObject_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_opcua_testObject pinst = Ov_StaticPtrCast(opcua_testObject, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}


