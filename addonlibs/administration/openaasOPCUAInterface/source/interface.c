
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


#ifndef OV_COMPILE_LIBRARY_openaasOPCUAInterface
#define OV_COMPILE_LIBRARY_openaasOPCUAInterface
#endif


#include "openaasOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "nodeset_openaas.h"

OV_INSTPTR_openaasOPCUAInterface_interface pinterface = NULL;

OV_DLLFNCEXPORT OV_RESULT openaasOPCUAInterface_interface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_openaasOPCUAInterface_interface pinst = Ov_StaticPtrCast(openaasOPCUAInterface_interface, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;
    OV_INSTPTR_identificationOPCUAInterface_interface pidentificationOPCUAInterface = NULL;
    OV_INSTPTR_lifeCycleEntryOPCUAInterface_interface plifeCycleEntryOPCUAInterface = NULL;
    OV_INSTPTR_propertyValueStatementOPCUAInterface_interface ppropertyValueStatementOPCUAInterface = NULL;
    OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pDomOPCUA = NULL;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_openaasOPCUAInterface_interface, pOtherObject){
		if(pOtherObject != pobj){
			ov_logfile_error("%s: cannot instantiate - interface instance already exists", pinst->v_identifier);
			return OV_ERR_ALREADYEXISTS;
		}
	}
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
    // create identificationOPCUAInterface
  	Ov_ForEachChildEx(ov_instantiation, pclass_identificationOPCUAInterface_interface, pidentificationOPCUAInterface, identificationOPCUAInterface_interface){
  		break;
  	}
  	if(!pidentificationOPCUAInterface){
  		result = Ov_CreateObject(identificationOPCUAInterface_interface, pidentificationOPCUAInterface, pDomOPCUA, "IdentificationOPCUAInterface");
  		if(Ov_Fail(result)){
  			ov_logfile_error("Fatal: could not create IdentificationOPCUAInterface");
  			return result;
  		}
  	}
  	// create lifeCycleEntryOPCUAInterface
	Ov_ForEachChildEx(ov_instantiation, pclass_lifeCycleEntryOPCUAInterface_interface, plifeCycleEntryOPCUAInterface, lifeCycleEntryOPCUAInterface_interface){
		break;
	}
	if(!plifeCycleEntryOPCUAInterface){
		result = Ov_CreateObject(lifeCycleEntryOPCUAInterface_interface, plifeCycleEntryOPCUAInterface, pDomOPCUA, "LifeCycleEntryOPCUAInterface");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create LifeCycleEntryOPCUAInterface");
			return result;
		}
	}
	// create propertyValueStatementOPCUAInterface
	Ov_ForEachChildEx(ov_instantiation, pclass_propertyValueStatementOPCUAInterface_interface, ppropertyValueStatementOPCUAInterface, propertyValueStatementOPCUAInterface_interface){
		break;
	}
	if(!ppropertyValueStatementOPCUAInterface){
		result = Ov_CreateObject(propertyValueStatementOPCUAInterface_interface, ppropertyValueStatementOPCUAInterface, pDomOPCUA, "PropertyValueStatementOPCUAInterface");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create PropertyValueStatementOPCUAInterface");
			return result;
		}
	}

	pinst->v_modelnamespaceIndexIdentification = pidentificationOPCUAInterface->v_modelnamespace.index;
	pinst->v_modelnamespaceIndexPropertyValueStatement = ppropertyValueStatementOPCUAInterface->v_modelnamespace.index;
	pinst->v_modelnamespaceIndexlifeCycleEntry = plifeCycleEntryOPCUAInterface->v_modelnamespace.index;

	pinterface = pinst;

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void openaasOPCUAInterface_interface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    //OV_INSTPTR_openaas_nodeStoreFunctions pinst = Ov_StaticPtrCast(openaas_nodeStoreFunctions, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}


OV_DLLFNCEXPORT void openaasOPCUAInterface_interface_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_openaasOPCUAInterface_interface pinst = Ov_StaticPtrCast(openaasOPCUAInterface_interface, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */

    UA_String tmpNamespaceName = UA_String_fromChars("http://acplt.org/openaas/");
    UA_Namespace_init(&pinst->v_modelnamespace, &tmpNamespaceName);
    UA_String_deleteMembers(&tmpNamespaceName);

    tmpNamespaceName = UA_String_fromChars("http://acplt.org/openaas/Ov");
	UA_Namespace_init(&pinst->v_interfacenamespace, &tmpNamespaceName);
	UA_String_deleteMembers(&tmpNamespaceName);
	pinst->v_interfacenamespace.nodestore = openaasOPCUAInterface_interface_ovNodeStoreInterfaceOpenAASNew();

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


OV_DLLFNCEXPORT OV_ACCESS openaasOPCUAInterface_interface_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*
    *   local variables
    */
	return (OV_ACCESS)OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE;
}

OV_DLLFNCEXPORT UA_NodeId openaasOPCUAInterface_interface_getAASNodeId(IdentificationType aasId) {
    return UA_NODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, openaas_modelmanager_AASConvertListGet(aasId));;
}

