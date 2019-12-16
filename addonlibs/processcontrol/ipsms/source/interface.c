/******************************************************************************
 *
 *   FILE
 *   ----
 *   uaInterface.c
 *
 *   History
 *   -------
 *   2019-02-26   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_ipsms
#define OV_COMPILE_LIBRARY_ipsms
#endif

#include "ipsms.h"
#include "libov/ov_macros.h"

#include "ipsms_trafo.h"
#include "opcua_helpers.h"

OV_DLLFNCEXPORT OV_RESULT ipsms_interface_constructor(
		OV_INSTPTR_ov_object pobj) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ipsms_interface pinst = Ov_StaticPtrCast(ipsms_interface,
			pobj);
	OV_RESULT result;

	/* do what the base class does first */
	result = ov_object_constructor(pobj);
	if (Ov_Fail(result))
		return result;

	/* do what */
	pinst->v_store = NULL;
	pinst->v_dataTypes = NULL;
	pinst->v_index = 0;
	ov_string_setvalue(&pinst->v_uri, OPCUA_DEFAULT_APPLICATIONURI);
	ov_string_append(&pinst->v_uri, "ipsms/");
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ipsms_interface_startup(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ipsms_interface pinst = Ov_StaticPtrCast(ipsms_interface, pobj);

	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	pinst->v_trafo = ipsms_trafo_new(pinst);

	return;
}

OV_DLLFNCEXPORT void ipsms_interface_shutdown(
		OV_INSTPTR_ov_object 	pobj) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ipsms_interface pinst = Ov_StaticPtrCast(ipsms_interface, pobj);

	/* do what */
	ov_string_setvalue(&pinst->v_uri, NULL);
	if (pinst->v_trafo) {
		ipsms_trafo_delete(pinst->v_trafo);
		pinst->v_trafo = NULL;
	}

	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}


OV_DLLFNCEXPORT OV_BOOL ipsms_interface_checkNode(
		OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode,
		OV_STRING virtualNodePath, void *context) {
	OV_INSTPTR_ipsms_interface pinst = Ov_StaticPtrCast(ipsms_interface, pobj);
	if ( pinst->v_SIProfile != IPSMS_PROFILE_SI_UNKNOWN && Ov_CanCastTo(fb_controlchart, pNode) ) {
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL ipsms_interface_checkReference(
		OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex,
		OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
	if (Ov_CanCastTo(fb_controlchart, pNode)) {
		if (parentRef) {
			parentRef->isForward = TRUE; //TODO make default
			parentRef->referenceTypeId = UA_NODEID_NUMERIC(0,
					UA_NS0ID_ORGANIZES);
			//parentRef->sourceNodeId
			parentRef->targetNodeClass = UA_NODECLASS_OBJECT;
			ov_memstack_lock();
			parentRef->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(applicationIndex,
					ov_path_getcanonicalpath(pNode, 2));
			ov_memstack_unlock();
			//parentRef->targetServerUri
		}
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ipsms_interface_SIProfile_set(
    OV_INSTPTR_ipsms_interface          pobj,
    const OV_INT  value
) {
	if(		value == IPSMS_PROFILE_SI_UNKNOWN ||
			value == IPSMS_PROFILE_SI_CMD ||
			value == IPSMS_PROFILE_SI_OPERATIONS){
	    pobj->v_SIProfile = value;
	    return OV_ERR_OK;
	} else {
		return OV_ERR_NOTIMPLEMENTED;
	}

}
