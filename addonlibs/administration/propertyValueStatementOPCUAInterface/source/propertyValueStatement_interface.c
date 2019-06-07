
/******************************************************************************
*
*   FILE
*   ----
*   interface.c
*
*   History
*   -------
*   2019-06-05   File created
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
#include "ua_propertyValueStatement_generated.h"
#include "nodeset_propertyValueStatement.h"


OV_DLLFNCEXPORT OV_RESULT propertyValueStatementOPCUAInterface_interface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_propertyValueStatementOPCUAInterface_interface pinst = Ov_StaticPtrCast(propertyValueStatementOPCUAInterface_interface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_index = 0;
	ov_string_setvalue(&pinst->v_uri, "acplt.org/propertyValueStatement/"); //Will be overwritten by config->applicationDescription.applicationUri
	UA_Nodestore_Default_Interface_new(&pinst->v_store);
	UA_DataTypeArray *pPropertyValueStatmentTypes = (UA_DataTypeArray*)UA_malloc(sizeof(UA_DataTypeArray));
	UA_DataTypeArray propertyValueStatmentTypes = {NULL, UA_PROPERTYVALUESTATEMENT_COUNT, UA_PROPERTYVALUESTATEMENT};
	memcpy(pPropertyValueStatmentTypes, &propertyValueStatmentTypes, sizeof(propertyValueStatmentTypes));
	pinst->v_dataTypes = pPropertyValueStatmentTypes;
	pinst->v_trafo = propertyValueStatementOPCUAInterface_interface_ovNodeStoreInterfacePropertyValueStatementNew(pinst);

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void propertyValueStatementOPCUAInterface_interface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_propertyValueStatementOPCUAInterface_interface pinst = Ov_StaticPtrCast(propertyValueStatementOPCUAInterface_interface, pobj);

    /* do what */
    if (pinst->v_store){
		pinst->v_store->deleteNodestore(pinst->v_store->context);
		UA_free(pinst->v_store);
	}
	UA_free(pinst->v_dataTypes);
	propertyValueStatementOPCUAInterface_interface_ovNodeStoreInterfacePropertyValueStatementDelete(pinst->v_trafo);
    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT OV_BOOL propertyValueStatementOPCUAInterface_interface_checkNode(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, OV_STRING virtualNodePath, void *context) {
    /*    
    *   local variables
    */

	if(Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_CarrierId, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_PropertyId, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_View, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_Visibility, pNode) ||
	   Ov_CanCastTo(propertyValueStatement_PropertyValueStatementList, pNode)){
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL propertyValueStatementOPCUAInterface_interface_checkReference(OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
    /*    
    *   local variables
    */

	return FALSE;
}

OV_DLLFNCEXPORT UA_StatusCode propertyValueStatementOPCUAInterface_interface_nodeset(UA_Server* server) {
    /*    
    *   local variables
    */

	return nodeset_propertyValueStatement(server);
}

