
/******************************************************************************
*
*   FILE
*   ----
*   modelmanager.c
*
*   History
*   -------
*   2017-08-25   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_propertyValueStatement
#define OV_COMPILE_LIBRARY_propertyValueStatement
#endif


#include "propertyValueStatement.h"
#include "libov/ov_macros.h"
#include "identification_helpers.h"
#include "propertyValueStatement_helpers.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"



OV_DLLFNCEXPORT PVSStatusCode pvs_modelmanager_ovresultToPVSStatusCode(OV_RESULT result) {
	switch(result){
	case OV_ERR_OK:
		return PVSSTATUSCODE_GOOD;
	default:
		return PVSSTATUSCODE_BADUNEXPECTEDERROR;
	}
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLParentIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLParentIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLParentIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLParentIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLName_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLName,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLMask_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLMask = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLCarrierIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLCarrierIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLCarrierIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLCarrierIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLExpressionLogic_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSLExpressionLogic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLExpressionSemantic_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSLExpressionSemantic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLPropertyIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLPropertyIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLPropertyIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLPropertyIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLView_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSLView = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLVisibility_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSLVisibility = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSLIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLCreate_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_BOOL  value
) {
	pobj->v_PVSLCreate = value;
	PVSStatusCode result = PVSSTATUSCODE_GOOD;
	if (pobj->v_PVSLCreate == TRUE){
		IdentificationType parentId;
		parentId.IdSpec = pobj->v_PVSLParentIdString;
		parentId.IdType = pobj->v_PVSLParentIdType;

		IdentificationType carrierId;
		carrierId.IdSpec = pobj->v_PVSLCarrierIdString;
		carrierId.IdType = pobj->v_PVSLCarrierIdType;

		IdentificationType propertyId;
		propertyId.IdSpec = pobj->v_PVSLPropertyIdString;
		propertyId.IdType = pobj->v_PVSLPropertyIdType;

		result = propertyValueStatement_modelmanager_createPVSL(parentId, pobj->v_PVSLName, pobj->v_PVSLMask, carrierId, pobj->v_PVSLExpressionLogic, pobj->v_PVSLExpressionSemantic, propertyId, pobj->v_PVSLView, pobj->v_PVSLVisibility);
	}
	pobj->v_PVSLCreate = FALSE;
	pobj->v_PVSLStatus = result;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSLDelete_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_BOOL  value
) {
	pobj->v_PVSLDelete = value;
	PVSStatusCode result = PVSSTATUSCODE_GOOD;
	if (pobj->v_PVSLDelete == TRUE){
		IdentificationType pvslId;
		pvslId.IdSpec = pobj->v_PVSLIdString;
		pvslId.IdType = pobj->v_PVSLIdType;

		result = propertyValueStatement_modelmanager_deletePVSL(pvslId);
	}
	pobj->v_PVSLDelete = FALSE;
	pobj->v_PVSLStatus = result;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSListIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSListIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSListIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSListIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSName_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSName,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSMask_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSMask = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSCarrierIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSCarrierIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSCarrierIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSCarrierIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSExpressionLogic_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSExpressionLogic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSExpressionSemantic_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSExpressionSemantic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSValue_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_PVSValue, value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSPropertyIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSPropertyIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSPropertyIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSPropertyIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSView_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSView = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSVisibility_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSVisibility = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSIdString_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSIdType_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSCreate_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_BOOL  value
) {
	PVSStatusCode result = PVSSTATUSCODE_GOOD;
	pobj->v_PVSCreate = value;
	if (pobj->v_PVSCreate == TRUE){
		IdentificationType listId;
		listId.IdSpec = pobj->v_PVSListIdString;
		listId.IdType = pobj->v_PVSListIdType;

		IdentificationType carrierId;
		carrierId.IdSpec = pobj->v_PVSCarrierIdString;
		carrierId.IdType = pobj->v_PVSCarrierIdType;

		IdentificationType propertyId;
		propertyId.IdSpec = pobj->v_PVSPropertyIdString;
		propertyId.IdType = pobj->v_PVSPropertyIdType;

		result = propertyValueStatement_modelmanager_createPVS(listId, pobj->v_PVSName, pobj->v_PVSValue, pobj->v_PVSMask, carrierId, pobj->v_PVSExpressionLogic, pobj->v_PVSExpressionSemantic, propertyId, pobj->v_PVSView, pobj->v_PVSVisibility);
	}
	pobj->v_PVSCreate = FALSE;
	pobj->v_PVSStatus = result;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_PVSDelete_set(
    OV_INSTPTR_propertyValueStatement_modelmanager          pobj,
    const OV_BOOL  value
) {
	PVSStatusCode result = PVSSTATUSCODE_GOOD;
	pobj->v_PVSDelete = value;
	if (pobj->v_PVSDelete == TRUE){
		IdentificationType pvsId;
		pvsId.IdSpec = pobj->v_PVSIdString;
		pvsId.IdType = pobj->v_PVSIdType;

		result = propertyValueStatement_modelmanager_deletePVS(pvsId);
	}
	pobj->v_PVSDelete = FALSE;
	pobj->v_PVSStatus = result;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_createPVSL(IdentificationType parentId, OV_STRING pvslName, OV_UINT mask, IdentificationType carrier, ExpressionLogicEnum expressionLogic, ExpressionSemanticEnum expressionSemantic, IdentificationType propertyId, ViewEnum view, VisibilityEnum visibility) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierId = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionLogic pExpressionLogic = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionSemantic pExpressionSemantic = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyId pPropertyId = NULL;
	OV_INSTPTR_propertyValueStatement_View pView = NULL;
	OV_INSTPTR_propertyValueStatement_Visibility pVisibility = NULL;

	if (parentId.IdType != URI){
		return PVSSTATUSCODE_BADPARENTID;
	}
	ptr = ov_path_getobjectpointer(parentId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPARENTID;

	ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, ptr), pvslName));
	if (ppvsl){
		return PVSSTATUSCODE_BADPVSLNAME;
	}

	result = Ov_CreateObject(propertyValueStatement_PropertyValueStatementList, ppvsl, Ov_StaticPtrCast(ov_domain, ptr), pvslName);
	if(Ov_Fail(result)){
		ov_logfile_error("Fatal: could not create PVSL object - reason: %s", ov_result_getresulttext(result));
		return pvs_modelmanager_ovresultToPVSStatusCode(result);
	}

	if (mask & 0x01){ //CarrierID
		result = Ov_CreateObject(propertyValueStatement_CarrierId, pCarrierId, Ov_StaticPtrCast(ov_domain, ppvsl), "CarrierId");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create Carrier object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		ov_string_setvalue(&pCarrierId->v_IdSpec, carrier.IdSpec);
		pCarrierId->v_IdType = carrier.IdType;
	}
	if (mask & 0x02){ //ExpressionLogic
		result = Ov_CreateObject(propertyValueStatement_ExpressionLogic, pExpressionLogic, Ov_StaticPtrCast(ov_domain, ppvsl), "ExpressionLogic");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create ExpressionLogic object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		pExpressionLogic->v_ExpressionLogicEnum = expressionLogic;
	}
	if (mask & 0x04){ //ExpressionSemantic
		result = Ov_CreateObject(propertyValueStatement_ExpressionSemantic, pExpressionSemantic, Ov_StaticPtrCast(ov_domain, ppvsl), "ExpressionSemantic");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create ExpressionSemantic object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		pExpressionSemantic->v_ExpressionSemanticEnum = expressionSemantic;
	}
	if (mask & 0x08){ //PropertyID
		result = Ov_CreateObject(propertyValueStatement_PropertyId, pPropertyId, Ov_StaticPtrCast(ov_domain, ppvsl), "PropertyID");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create PropertyID object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		ov_string_setvalue(&pPropertyId->v_IdSpec, propertyId.IdSpec);
		pPropertyId->v_IdType = propertyId.IdType;
	}
	if (mask & 0x10){ //View
		result = Ov_CreateObject(propertyValueStatement_View, pView, Ov_StaticPtrCast(ov_domain, ppvsl), "View");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create View object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		pView->v_ViewEnum = view;
	}
	if (mask & 0x20){ //Visibility
		result = Ov_CreateObject(propertyValueStatement_Visibility, pVisibility, Ov_StaticPtrCast(ov_domain, ppvsl), "Visibility");
		if(Ov_Fail(result)){
			ov_logfile_error("Fatal: could not create Visibility object - reason: %s", ov_result_getresulttext(result));
			return pvs_modelmanager_ovresultToPVSStatusCode(result);
		}
		pVisibility->v_VisibilityEnum = visibility;
	}

    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_deletePVSL(IdentificationType pvslId) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;

	if (pvslId.IdType != URI){
		return PVSSTATUSCODE_BADPVSLID;
	}
	ptr = ov_path_getobjectpointer(pvslId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPVSLID;

	ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_StaticPtrCast(ov_domain, ptr));
	if (!ppvsl){
		return PVSSTATUSCODE_BADPVSLID;
	}
	result = Ov_DeleteObject(ptr);
	if(Ov_Fail(result)){
		ov_logfile_error("Fatal: could not delete PVSL object - reason: %s", ov_result_getresulttext(result));
		return pvs_modelmanager_ovresultToPVSStatusCode(result);
	}
    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_createPVS(IdentificationType listId, OV_STRING pvsName, OV_ANY value, OV_UINT mask, IdentificationType carrier, ExpressionLogicEnum expressionLogic, ExpressionSemanticEnum expressionSemantic, IdentificationType propertyId, ViewEnum view, VisibilityEnum visibility) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierId = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionLogic pExpressionLogic = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionSemantic pExpressionSemantic = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyId pPropertyId = NULL;
	OV_INSTPTR_propertyValueStatement_View pView = NULL;
	OV_INSTPTR_propertyValueStatement_Visibility pVisibility = NULL;
	OV_INSTPTR_ov_object pchild = NULL;

	if (listId.IdType != URI){
		return PVSSTATUSCODE_BADPVSLID;
	}
	ptr = ov_path_getobjectpointer(listId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPVSLID;

	ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, ptr);
	if (!ppvsl){
		return PVSSTATUSCODE_BADPVSLID;
	}

	result = Ov_CreateObject(propertyValueStatement_PropertyValueStatement, ppvs, Ov_StaticPtrCast(ov_domain, ppvsl), pvsName);
	if(Ov_Fail(result)){
		ov_logfile_error("Fatal: could not create PVS object - reason: %s", ov_result_getresulttext(result));
		return pvs_modelmanager_ovresultToPVSStatusCode(result);
	}

	ov_variable_setanyvalue(&ppvs->v_Value, &value);

	Ov_ForEachChild(ov_containment, ppvs, pchild){
		if (Ov_CanCastTo(propertyValueStatement_CarrierId, pchild) && mask & 0x01){
			pCarrierId = Ov_StaticPtrCast(propertyValueStatement_CarrierId, pchild);
			ov_string_setvalue(&pCarrierId->v_IdSpec, carrier.IdSpec);
			pCarrierId->v_IdType = carrier.IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pchild) && mask & 0x02){
			pExpressionLogic = Ov_StaticPtrCast(propertyValueStatement_ExpressionLogic, pchild);
			pExpressionLogic->v_ExpressionLogicEnum = expressionLogic;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pchild) && mask & 0x04){
			pExpressionSemantic = Ov_StaticPtrCast(propertyValueStatement_ExpressionSemantic, pchild);
			pExpressionSemantic->v_ExpressionSemanticEnum = expressionSemantic;
		}else if(Ov_CanCastTo(propertyValueStatement_PropertyId, pchild) && mask & 0x08){
			pPropertyId = Ov_StaticPtrCast(propertyValueStatement_PropertyId, pchild);
			ov_string_setvalue(&pPropertyId->v_IdSpec, propertyId.IdSpec);
			pPropertyId->v_IdType = propertyId.IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_View, pchild) && mask & 0x10){
			pView = Ov_StaticPtrCast(propertyValueStatement_View, pchild);
			pView->v_ViewEnum = view;
		}else if(Ov_CanCastTo(propertyValueStatement_Visibility, pchild) && mask & 0x20){
			pVisibility = Ov_StaticPtrCast(propertyValueStatement_Visibility, pchild);
			pVisibility->v_VisibilityEnum = visibility;
		}
	}

    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_deletePVS(IdentificationType pvsId) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;

	if (pvsId.IdType != URI){
		return PVSSTATUSCODE_BADPVSID;
	}
	ptr = ov_path_getobjectpointer(pvsId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPVSID;

	ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, Ov_StaticPtrCast(ov_domain, ptr));
	if (!ppvs){
		return PVSSTATUSCODE_BADPVSID;
	}
	result = Ov_DeleteObject(ptr);
	if(Ov_Fail(result)){
		ov_logfile_error("Fatal: could not delete PVS object - reason: %s", ov_result_getresulttext(result));
		return pvs_modelmanager_ovresultToPVSStatusCode(result);
	}
    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_getPVS(IdentificationType pvsId, OV_STRING *pvsName, IdentificationType *carrier, ExpressionLogicEnum *expressionLogic, ExpressionSemanticEnum *expressionSemantic, IdentificationType *propertyId, ViewEnum *view, VisibilityEnum *visibility, OV_ANY *value) {
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierId = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionLogic pExpressionLogic = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionSemantic pExpressionSemantic = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyId pPropertyId = NULL;
	OV_INSTPTR_propertyValueStatement_View pView = NULL;
	OV_INSTPTR_propertyValueStatement_Visibility pVisibility = NULL;
	OV_INSTPTR_ov_object pchild = NULL;
	OV_INSTPTR_ov_domain pparent = NULL;

	if (pvsId.IdType != URI){
		return PVSSTATUSCODE_BADPVSID;
	}
	ptr = ov_path_getobjectpointer(pvsId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPVSID;

	ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, ptr);
	if (!ppvs){
		return PVSSTATUSCODE_BADPVSID;
	}
	pparent = Ov_GetParent(ov_containment, ptr);
	Ov_ForEachChild(ov_containment, pparent, pchild){
		if (Ov_CanCastTo(propertyValueStatement_CarrierId, pchild)){
			pCarrierId = Ov_StaticPtrCast(propertyValueStatement_CarrierId, pchild);
			ov_string_setvalue(&carrier->IdSpec, pCarrierId->v_IdSpec);
			carrier->IdType = pCarrierId->v_IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pchild)){
			pExpressionLogic = Ov_StaticPtrCast(propertyValueStatement_ExpressionLogic, pchild);
			*expressionLogic = pExpressionLogic->v_ExpressionLogicEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pchild)){
			pExpressionSemantic = Ov_StaticPtrCast(propertyValueStatement_ExpressionSemantic, pchild);
			*expressionSemantic = pExpressionSemantic->v_ExpressionSemanticEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_PropertyId, pchild)){
			pPropertyId = Ov_StaticPtrCast(propertyValueStatement_PropertyId, pchild);
			ov_string_setvalue(&propertyId->IdSpec, pPropertyId->v_IdSpec);
			propertyId->IdType = pPropertyId->v_IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_View, pchild)){
			pView = Ov_StaticPtrCast(propertyValueStatement_View, pchild);
			*view = pView->v_ViewEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_Visibility, pchild)){
			pVisibility = Ov_StaticPtrCast(propertyValueStatement_Visibility, pchild);
			*visibility = pVisibility->v_VisibilityEnum;
		}
	}
	Ov_ForEachChild(ov_containment, ppvs, pchild){
		if (Ov_CanCastTo(propertyValueStatement_CarrierId, pchild)){
			pCarrierId = Ov_StaticPtrCast(propertyValueStatement_CarrierId, pchild);
			ov_string_setvalue(&carrier->IdSpec, pCarrierId->v_IdSpec);
			carrier->IdType = pCarrierId->v_IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pchild)){
			pExpressionLogic = Ov_StaticPtrCast(propertyValueStatement_ExpressionLogic, pchild);
			*expressionLogic = pExpressionLogic->v_ExpressionLogicEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pchild)){
			pExpressionSemantic = Ov_StaticPtrCast(propertyValueStatement_ExpressionSemantic, pchild);
			*expressionSemantic = pExpressionSemantic->v_ExpressionSemanticEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_PropertyId, pchild)){
			pPropertyId = Ov_StaticPtrCast(propertyValueStatement_PropertyId, pchild);
			ov_string_setvalue(&propertyId->IdSpec, pPropertyId->v_IdSpec);
			propertyId->IdType = pPropertyId->v_IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_View, pchild)){
			pView = Ov_StaticPtrCast(propertyValueStatement_View, pchild);
			*view = pView->v_ViewEnum;
		}else if(Ov_CanCastTo(propertyValueStatement_Visibility, pchild)){
			pVisibility = Ov_StaticPtrCast(propertyValueStatement_Visibility, pchild);
			*visibility = pVisibility->v_VisibilityEnum;
		}
	}
	ov_string_setvalue(pvsName, ppvs->v_identifier);
	ov_variable_setanyvalue(value, &ppvs->v_Value);

    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT PVSStatusCode propertyValueStatement_modelmanager_setPVS(IdentificationType pvsId, OV_UINT mask, OV_STRING pvsName, IdentificationType carrier, ExpressionLogicEnum expressionLogic, ExpressionSemanticEnum expressionSemantic, IdentificationType propertyId, ViewEnum view, VisibilityEnum visibility, OV_ANY value) {
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierId = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionLogic pExpressionLogic = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionSemantic pExpressionSemantic = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyId pPropertyId = NULL;
	OV_INSTPTR_propertyValueStatement_View pView = NULL;
	OV_INSTPTR_propertyValueStatement_Visibility pVisibility = NULL;
	OV_INSTPTR_ov_object pchild = NULL;
	OV_INSTPTR_ov_domain pparent = NULL;
	OV_BOOL foundName = FALSE;

	if (pvsId.IdType != URI){
		return PVSSTATUSCODE_BADPVSID;
	}
	ptr = ov_path_getobjectpointer(pvsId.IdSpec, 2);
	if (!ptr)
		return PVSSTATUSCODE_BADPVSID;

	ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, ptr);
	if (!ppvs){
		return PVSSTATUSCODE_BADPVSID;
	}
	if (mask & 0x01){
		pparent = Ov_GetParent(ov_containment, ptr);
		Ov_ForEachChild(ov_containment, pparent, pchild){
			if (ov_string_compare(pchild->v_identifier, pvsName) == OV_STRCMP_EQUAL){
				foundName = TRUE;
			}
		}
		if (foundName == FALSE){
			ov_string_setvalue(&ppvs->v_identifier, pvsName);
		}
	}
	Ov_ForEachChild(ov_containment, ppvs, pchild){
		if (Ov_CanCastTo(propertyValueStatement_CarrierId, pchild) && mask & 0x02){
			pCarrierId = Ov_StaticPtrCast(propertyValueStatement_CarrierId, pchild);
			ov_string_setvalue(&pCarrierId->v_IdSpec, carrier.IdSpec);
			pCarrierId->v_IdType = carrier.IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pchild) && mask & 0x04){
			pExpressionLogic = Ov_StaticPtrCast(propertyValueStatement_ExpressionLogic, pchild);
			pExpressionLogic->v_ExpressionLogicEnum = expressionLogic;
		}else if(Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pchild) && mask & 0x08){
			pExpressionSemantic = Ov_StaticPtrCast(propertyValueStatement_ExpressionSemantic, pchild);
			pExpressionSemantic->v_ExpressionSemanticEnum = expressionSemantic;
		}else if(Ov_CanCastTo(propertyValueStatement_PropertyId, pchild) && mask & 0x10){
			pPropertyId = Ov_StaticPtrCast(propertyValueStatement_PropertyId, pchild);
			ov_string_setvalue(&pPropertyId->v_IdSpec, propertyId.IdSpec);
			pPropertyId->v_IdType = propertyId.IdType;
		}else if(Ov_CanCastTo(propertyValueStatement_View, pchild) && mask & 0x20){
			pView = Ov_StaticPtrCast(propertyValueStatement_View, pchild);
			pView->v_ViewEnum = view;
		}else if(Ov_CanCastTo(propertyValueStatement_Visibility, pchild) && mask & 0x40){
			pVisibility = Ov_StaticPtrCast(propertyValueStatement_Visibility, pchild);
			pVisibility->v_VisibilityEnum = visibility;
		}
	}
	if(mask & 0x80){
		ov_variable_setanyvalue(&ppvs->v_Value, &value);
	}
    return (PVSStatusCode)0;
}

OV_DLLFNCEXPORT OV_ACCESS propertyValueStatement_modelmanager_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */

    return (OV_ACCESS)(OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE);
}

OV_DLLFNCEXPORT OV_RESULT propertyValueStatement_modelmanager_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_propertyValueStatement_modelmanager pinst = Ov_StaticPtrCast(propertyValueStatement_modelmanager, pobj);
    OV_RESULT    result;
    OV_INSTPTR_ov_object pOtherObject = NULL;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    Ov_ForEachChild(ov_instantiation, pclass_propertyValueStatement_modelmanager, pOtherObject){
		if(pOtherObject != pobj){
			ov_logfile_error("%s: cannot instantiate - modelmanager instance already exists", pinst->v_identifier);
			return OV_ERR_ALREADYEXISTS;
		}
	}

    return OV_ERR_OK;
}

