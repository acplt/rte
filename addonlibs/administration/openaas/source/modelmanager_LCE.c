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

#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif

#include "openaas.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "openaas_helpers.h"
#include "lifeCycleEntry.h"


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEAASIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEAASIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEAASIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEAASIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEId_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEId = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreatingInstanceIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCECreatingInstanceIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreatingInstanceIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCECreatingInstanceIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEWritingInstanceIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEWritingInstanceIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEWritingInstanceIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEWritingInstanceIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEEventClass_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEEventClass,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCESubject_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCESubject,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEValue_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_LCEValue, value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCETimeStamp_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_TIME  *value
) {
    pobj->v_LCETimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreate_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_LCECreate = value;
	if (pobj->v_LCECreate == TRUE){
		IdentificationType tmpAASId;
		IdentificationType_init(&tmpAASId);
		tmpAASId.IdSpec = pobj->v_LCEAASIdString;
		tmpAASId.IdType = pobj->v_LCEAASIdType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);
		lce.creatingInstance.IdSpec = pobj->v_LCECreatingInstanceIdString;
		lce.creatingInstance.IdType = pobj->v_LCECreatingInstanceIdType;
		lce.writingInstance.IdSpec = pobj->v_LCEWritingInstanceIdString;
		lce.writingInstance.IdType = pobj->v_LCEWritingInstanceIdType;
		lce.data = pobj->v_LCEValue;
		lce.data.time = pobj->v_LCETimeStamp;
		lce.eventClass = pobj->v_LCEEventClass;
		lce.lceId = pobj->v_LCEId;
		lce.subject = pobj->v_LCESubject;

		result = openaas_modelmanager_createLCE(tmpAASId, lce);
	}
	pobj->v_LCECreate = FALSE;
	pobj->v_LCEStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEDelete_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_LCEDelete = value;
	if (pobj->v_LCEDelete == TRUE){
		IdentificationType tmpAASId;
		tmpAASId.IdSpec = pobj->v_LCEAASIdString;
		tmpAASId.IdType = pobj->v_LCEAASIdType;

		result = openaas_modelmanager_deleteLCE(tmpAASId, pobj->v_LCEId);
	}
	pobj->v_LCEDelete = FALSE;
	pobj->v_LCEStatus = result;
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createLCE(IdentificationType aasId, LifeCycleEntry lce) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (lifeCycleEntry_LifeCycleArchive_createLCE(&paas->p_LifeCycleArchive, lce) != LCESTATUSCODE_GOOD)
			result = AASSTATUSCODE_BADLCEERROR;
	}else{
		result = AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return result;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_deleteLCE(IdentificationType aasId, OV_UINT64 lceId) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (lifeCycleEntry_LifeCycleArchive_deleteLCE(&paas->p_LifeCycleArchive, lceId) != LCESTATUSCODE_GOOD)
			result = AASSTATUSCODE_BADLCEERROR;
	}else{
		result = AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return result;
}



OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getLCE(IdentificationType aasId, OV_UINT64 lceId, LifeCycleEntry *lce) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (lifeCycleEntry_LifeCycleArchive_getLCE(&paas->p_LifeCycleArchive, lceId, lce) != LCESTATUSCODE_GOOD)
			return AASSTATUSCODE_BADLCEERROR;
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getLastLCEs(IdentificationType aasId, OV_UINT count, LifeCycleEntry **lce, OV_UINT *arrayDimension) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (lifeCycleEntry_LifeCycleArchive_getLastLCEs(&paas->p_LifeCycleArchive, count, lce, arrayDimension) != LCESTATUSCODE_GOOD)
			return AASSTATUSCODE_BADLCEERROR;
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_setLCE(IdentificationType aasId, LifeCycleEntry lce) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (lifeCycleEntry_LifeCycleArchive_setLCE(&paas->p_LifeCycleArchive, lce) != LCESTATUSCODE_GOOD)
			return AASSTATUSCODE_BADLCEERROR;
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}
