
/******************************************************************************
*
*   FILE
*   ----
*   modelmanager.c
*
*   History
*   -------
*   2016-12-22   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif


#include "openaas.h"
#include "libov/ov_macros.h"
#include "openaas_helpers.h"

extern OV_INSTPTR_openaas_InterfaceDiscoveryServer pInterfaceDiscoveryServer;

void openaas_modelmanager_AASConvertListAdd(IdentificationType aasId, OV_STRING aasName){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;

	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType)
			return;
	}

	OV_STRING tmpString = NULL;
	ov_string_print(&tmpString, "%i", pmodelmanager->v_ListCount+1);
	if (Ov_Fail(Ov_CreateObject(openaas_AASConvertListType, pListElement, Ov_StaticPtrCast(ov_domain, pmodelmanager), tmpString))){
		ov_database_free(tmpString);
		return;
	}
	ov_database_free(tmpString);
	pmodelmanager->v_ListSize += 1;
	pmodelmanager->v_ListCount += 1;
	ov_string_setvalue(&pListElement->v_AASIdString, aasId.IdSpec);
	pListElement->v_AASIdType = aasId.IdType;
	tmpString = NULL;
	ov_string_setvalue(&tmpString, "/TechUnits/openAAS/AASFolder/");
	ov_string_append(&tmpString, aasName);
	ov_string_setvalue(&pListElement->v_AASPath, tmpString);
	ov_database_free(tmpString);

	return;
}


void openaas_modelmanager_AASConvertListDelete(IdentificationType aasId){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;

	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType){
			if (Ov_Fail(Ov_DeleteObject(pListElement))){
				return;
			}
			pmodelmanager->v_ListSize -= 1;
			return;
		}
	}
	return;
}

OV_STRING openaas_modelmanager_AASConvertListGet(IdentificationType aasId){
	OV_INSTPTR_openaas_modelmanager pmodelmanager = NULL;
	OV_INSTPTR_openaas_AASConvertListType pListElement = NULL;
	pmodelmanager = Ov_StaticPtrCast(openaas_modelmanager, Ov_GetFirstChild(ov_instantiation, pclass_openaas_modelmanager));

	if (!pmodelmanager){
		return "";
	}

	if (pmodelmanager->v_ListSize == 0){
		return "";
	}

	Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, pmodelmanager), pListElement, openaas_AASConvertListType){
		if (ov_string_compare(pListElement->v_AASIdString, aasId.IdSpec) == OV_STRCMP_EQUAL && pListElement->v_AASIdType == aasId.IdType){
			return pListElement->v_AASPath;
		}
	}
	return "";
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_ovresultToAASStatusCode(OV_RESULT result) {
	switch(result){
	case OV_ERR_OK:
		return AASSTATUSCODE_GOOD;
	default:
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
}




OV_DLLFNCEXPORT OV_ACCESS openaas_modelmanager_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*
    *   local variables
    */
    return (OV_ACCESS)(OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE);
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_openaas_modelmanager pinst = Ov_StaticPtrCast(openaas_modelmanager, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;
	OV_INSTPTR_openaas_aas pComCo = NULL;
    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_LCEValue.value.valueunion.val_int = 0;
	pinst->v_LCEValue.value.vartype = OV_VT_INT;

    Ov_ForEachChild(ov_instantiation, pclass_openaas_modelmanager, pOtherObject){
		if(pOtherObject != pobj){
			ov_logfile_error("%s: cannot instantiate - modelmanager instance already exists", pinst->v_identifier);
			return OV_ERR_ALREADYEXISTS;
		}
	}

	 OV_INSTPTR_ov_domain pdata = NULL;
	 OV_INSTPTR_ov_domain popenAAS = NULL;
	 pdata = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "data"));
	 if(!pdata) {
	 	result = Ov_CreateObject(ov_domain, pdata, &(pdb->root), "data");
	 	if(Ov_Fail(result)) {
	 		ov_logfile_error("Fatal: Could not create Object 'data'");
	 		return result;
	 	}
	 }
	 else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pdata))	{
	 	ov_logfile_error("Fatal: pdata object found but not domain (or derived)");
	 	return OV_ERR_GENERIC;
	 }
	 popenAAS = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pdata, "openAAS"));
	 if(!popenAAS) {
	 	result = Ov_CreateObject(ov_domain, popenAAS, pdata, "openAAS");
	 	if(Ov_Fail(result))	{
	 		ov_logfile_error("Fatal: could not create openAAS domain");
	 		return result;
	 	}
	 }
	 else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) popenAAS)){
	 	ov_logfile_error("Fatal: openAAS object found but not domain (or derived)");
	 	return OV_ERR_GENERIC;
	 }

	// create InterfaceDiscoveryServer
	if(!pInterfaceDiscoveryServer){
		result = Ov_CreateObject(openaas_InterfaceDiscoveryServer, pInterfaceDiscoveryServer, popenAAS, "InterfaceDiscoveryServer");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create InterfaceDiscoveryServer object - reason: %s", ov_result_getresulttext(result));
			return result;
		}
	}

	// Create Folder for AAS
	OV_INSTPTR_ov_domain pTechUnits = NULL;
	pTechUnits = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "TechUnits"));
	if(!pTechUnits) {
		result = Ov_CreateObject(ov_domain, pTechUnits, &(pdb->root), "TechUnits");
		if(Ov_Fail(result)) {
			ov_logfile_error("Fatal: Could not create Object 'pTechUnits'");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pTechUnits))	{
		ov_logfile_error("Fatal: pTechUnits object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	OV_INSTPTR_ov_domain popenAASFolder = NULL;
	popenAASFolder = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pTechUnits, "openAAS"));
	if(!popenAASFolder) {
		result = Ov_CreateObject(ov_domain, popenAASFolder, pTechUnits, "openAAS");
		if(Ov_Fail(result))	{
			ov_logfile_error("Fatal: could not create openAAS domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) popenAASFolder)){
		ov_logfile_error("Fatal: openAAS object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	OV_INSTPTR_ov_domain pAASFolder = NULL;
	pAASFolder = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, popenAASFolder, "AASFolder"));
	if(!pAASFolder) {
		result = Ov_CreateObject(ov_domain, pAASFolder, popenAASFolder, "AASFolder");
		if(Ov_Fail(result))	{
			ov_logfile_error("Fatal: could not create AASFolder domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pAASFolder)){
		ov_logfile_error("Fatal: AASFolder object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	// create ComCo
	Ov_ForEachChildEx(ov_instantiation, pclass_openaas_aas, pComCo, openaas_aas){
		if(ov_string_compare(pComCo->v_identifier, "ComCo") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if(!pComCo){
		IdentificationType aasId;
		aasId.IdSpec = NULL;
		ov_string_setvalue(&aasId.IdSpec, "http://acplt.org/ComCo");
		aasId.IdType = 0;

		IdentificationType assetId;
		assetId.IdSpec = NULL;
		ov_string_setvalue(&assetId.IdSpec, "http://acplt.org/Manager");
		assetId.IdType = 0;

		/*if (AASSTATUSCODE_FAIL(openaas_modelmanager_createAAS(aasId, "ComCo", assetId))){
			ov_string_setvalue(&aasId.IdSpec, NULL);
			ov_string_setvalue(&assetId.IdSpec, NULL);
			ov_logfile_error("Fatal: Could not create Object 'ComCo'");
			return OV_ERR_GENERIC;
		}
		*/
		ov_string_setvalue(&aasId.IdSpec, NULL);
		ov_string_setvalue(&assetId.IdSpec, NULL);
	}
    return OV_ERR_OK;
}
