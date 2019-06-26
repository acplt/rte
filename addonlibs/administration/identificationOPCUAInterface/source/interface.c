
/******************************************************************************
*
*   FILE
*   ----
*   interface.c
*
*   History
*   -------
*   2017-05-03   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_identificationOPCUAInterface
#define OV_COMPILE_LIBRARY_identificationOPCUAInterface
#endif


#include "identificationOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "nodeset_identification.h"
#include "ua_identification_generated.h"

OV_INSTPTR_identificationOPCUAInterface_interface pinterface = NULL;

OV_DLLFNCEXPORT OV_RESULT identificationOPCUAInterface_interface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_identificationOPCUAInterface_interface pinst = Ov_StaticPtrCast(identificationOPCUAInterface_interface, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_library pLibOPCUA = NULL;
    OV_INSTPTR_ov_object pOtherObject = NULL;
    OV_INSTPTR_opcua_uaServer puaServer = NULL;
    OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pDomOPCUA = NULL;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_identificationOPCUAInterface_interface, pOtherObject){
		if(pOtherObject != pobj){
			ov_logfile_error("%s: cannot instantiate - interface instance already exists", pinst->v_identifier);
			return OV_ERR_ALREADYEXISTS;
		}
	}

    Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, pLibOPCUA, ov_library){
		if(ov_string_compare(pLibOPCUA->v_identifier, "opcua") == OV_STRCMP_EQUAL){
			break;
		}
	 }
	 if(!pLibOPCUA){
		result = Ov_CreateObject(ov_library, pLibOPCUA, &(pdb->acplt), "opcua");
		if(Ov_Fail(result)){
			ov_memstack_lock();
			ov_logfile_error("identificationOPCUAInterface: Fatal: Couldn't load dependency Library opcua Reason: %s", ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
	 }

	// create ua Server
	Ov_ForEachChildEx(ov_instantiation, pclass_opcua_uaServer, puaServer, opcua_uaServer){
		break;
	}
	if(!puaServer){
		pcommunication = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "communication"));
		if(!pcommunication) {
			result = Ov_CreateObject(ov_domain, pcommunication, &(pdb->root), "communication");
			if(Ov_Fail(result)) {
				ov_logfile_error("Fatal: Could not create Object 'communication'");
				return result;
			}
		}
		else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication)){
			ov_logfile_error("Fatal: communication object found but not domain (or derived)");
			return OV_ERR_GENERIC;
		}

		pDomOPCUA = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "opcua"));
		if(!pDomOPCUA) {
			result = Ov_CreateObject(ov_domain, pDomOPCUA, pcommunication, "opcua");
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create opcua domain");
				return result;
			}
		}
		else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pDomOPCUA)){
			ov_logfile_error("Fatal: opcua object found but not domain (or derived)");
			return OV_ERR_GENERIC;
		}
		result = Ov_CreateObject(opcua_uaServer, puaServer, pDomOPCUA, "uaServer");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create uaServer");
			return result;
		}
	}

	pinterface = pinst;

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void identificationOPCUAInterface_interface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    //OV_INSTPTR_identification_nodeStoreFunctions pinst = Ov_StaticPtrCast(identification_nodeStoreFunctions, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}


OV_DLLFNCEXPORT void identificationOPCUAInterface_interface_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_identificationOPCUAInterface_interface pinst = Ov_StaticPtrCast(identificationOPCUAInterface_interface, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */

    UA_String tmpNamespaceName = UA_String_fromChars("http://acplt.org/identification/");
    UA_Namespace_init(&pinst->v_modelnamespace, &tmpNamespaceName);
    UA_String_deleteMembers(&tmpNamespaceName);
    pinst->v_modelnamespace.dataTypes = UA_IDENTIFICATION;
    pinst->v_modelnamespace.dataTypesSize = UA_IDENTIFICATION_COUNT;

    tmpNamespaceName = UA_String_fromChars("http://acplt.org/identification/Ov");
	UA_Namespace_init(&pinst->v_interfacenamespace, &tmpNamespaceName);
	UA_String_deleteMembers(&tmpNamespaceName);
	pinst->v_interfacenamespace.nodestore = opcua_nodeStoreFunctions_ovNodeStoreInterface2New();

    UA_Namespace** nsArray = UA_malloc(2 * sizeof(UA_Namespace*));
    nsArray[0] = &pinst->v_modelnamespace;
    nsArray[1] = &pinst->v_interfacenamespace;

    OV_STRING startFolder[2];
    startFolder[0] = NULL;
    startFolder[1] = NULL;

    // Add InformationModel & NodeStoreInterface & Reference to AASFolder
	UA_StatusCode ret = UA_STATUSCODE_GOOD;
	ret = opcua_uaServer_addInformationModel(nsArray, 2, startFolder, nodeset_returnNamespaces);
	startFolder[1] = NULL;
	if (ret != UA_STATUSCODE_GOOD){
		ov_logfile_error("openaas: Fatal: Couldn't add InformationModel");
		UA_Namespace_deleteMembers(&(pinst->v_modelnamespace));
		UA_Namespace_deleteMembers(&(pinst->v_interfacenamespace));
		return;
	}
	UA_free(nsArray);

    return;
}


OV_DLLFNCEXPORT OV_ACCESS identificationOPCUAInterface_interface_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*
    *   local variables
    */

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
