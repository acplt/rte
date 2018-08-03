
/******************************************************************************
*
*   FILE
*   ----
*   SQLite3.c
*
*   History
*   -------
*   2018-04-25   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_Databases
#define OV_COMPILE_LIBRARY_Databases
#endif


#include "Databases.h"
#include "libov/ov_macros.h"
#include "sqlite3.h"
#include "libov/ov_logfile.h"
#include <stdio.h>


int rc;

// callback function
static int callback(void* data, int argc, char **argv, char **col_name) {

	Ov_SetDynamicVectorLength((OV_STRING_VEC*)data, ((OV_STRING_VEC*)data)->veclen + argc, STRING);

	for(int i = 0; i<argc; i++) {
		ov_string_setvalue(&(((OV_STRING_VEC*)data)->value[((OV_STRING_VEC*)data)->veclen - argc + i]), argv[i]);
		//ov_logfile_info("%-10s : %-s", col_name[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}

// callback function
static int callbackFittingStatements(void* data, int argc, char **argv, char **col_name) {

	Ov_SetDynamicVectorLength((OV_STRING_VEC*)data, ((OV_STRING_VEC*)data)->veclen + argc / 7, STRING);

	for(int i = 0; i<(argc/7); i++) {
		OV_STRING tmpString = NULL;
		ov_string_setvalue(&tmpString, "\"CarrierID\":\"");
		ov_string_append(&tmpString, argv[1+i*7]);
		ov_string_append(&tmpString, "\", \"PropertyID\":\"");
		ov_string_append(&tmpString, argv[2+i*7]);
		ov_string_append(&tmpString, "\", \"ExpressionSemantic\":\"");
		ov_string_append(&tmpString, argv[3+i*7]);
		ov_string_append(&tmpString, "\", \"Relation\":\"");
		ov_string_append(&tmpString, argv[4+i*7]);
		ov_string_append(&tmpString, "\", \"Value\":\"");
		ov_string_append(&tmpString, argv[5+i*7]);
		ov_string_append(&tmpString, "\", \"SubModel\":\"");
		ov_string_append(&tmpString, argv[6+i*7]);
		ov_string_append(&tmpString, "\"");
		ov_string_setvalue(&(((OV_STRING_VEC*)data)->value[((OV_STRING_VEC*)data)->veclen - (argc/7) + i]), tmpString);
		ov_string_setvalue(&tmpString, NULL);
		//ov_logfile_info("%-10s : %-s", col_name[i], argv[i] ? argv[i] : "NULL");
	}
	return 0;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_connect(OV_INSTPTR_openAASDiscoveryServer_Wrapper this) {
	OV_STRING err_msg = NULL;

	// open database
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	rc = sqlite3_open(pinst->v_Endpoint , &pinst->v_db);

	// check first if db handle allocated
	if(pinst->v_db == NULL) {
		ov_string_setvalue(&pinst->v_ErrorMessage, "Could not allocate memory for DB handle!");
		pinst->v_ErrorFlag = TRUE;
		pinst->v_State = 3;
		return OV_ERR_GENERIC;
	}
	// check then if error occured
	if( rc != SQLITE_OK) {
		// since v_db is not null we can retrieve sqlite3_errmsg()
		ov_string_setvalue(&err_msg,  (OV_STRING)sqlite3_errmsg(pinst->v_db));
		ov_string_setvalue(&pinst->v_ErrorMessage, err_msg);
		pinst->v_ErrorFlag = TRUE;
		sqlite3_close(pinst->v_db);
		pinst->v_State = 3;
		return OV_ERR_GENERIC;
	}
	err_msg = NULL;

	// reset error message
	// TODO: what if db is already in bad state?
	if(!pinst->v_ErrorFlag) {
		pinst->v_ErrorFlag = FALSE;
		ov_string_setvalue(&pinst->v_ErrorMessage, NULL);
	}
	pinst->v_State = 1;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_disconnect(OV_INSTPTR_openAASDiscoveryServer_Wrapper this) {
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	sqlite3_close(pinst->v_db);
	pinst->v_State = 2;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_insertData(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING table, const OV_STRING* fields, OV_UINT fieldsLen,
													   const OV_STRING* values, OV_UINT valuesLen) {

	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);

	if(fieldsLen != valuesLen) {
		return OV_ERR_BADPARAM;
	}
	// build up INSERT query
	OV_STRING query = NULL;
	ov_string_setvalue(&query, "INSERT INTO ");
	ov_string_print(&query, "%s%s ", query, table);
	ov_string_print(&query, "%s%s", query, "(");
	for(int i = 0; i < fieldsLen; i++) {
		if(i == fieldsLen-1) {
			ov_string_print(&query, "%s%s) ", query, fields[i]);
		} else {
			ov_string_print(&query, "%s%s, ", query, fields[i]);
		}
	}
	ov_string_print(&query, "%s%s", query, "VALUES (");
	for(int i = 0; i < fieldsLen; i++) {
		if(i == fieldsLen-1) {
			ov_string_print(&query, "%s%s); ", query, values[i]);
		} else {
			ov_string_print(&query, "%s%s, ", query, values[i]);
		}
	}
	//ov_logfile_info("%s", query);

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, NULL, NULL, &err_msg);

	if (rc == SQLITE_CONSTRAINT){
		ov_string_setvalue(&query, NULL);
		sqlite3_free(err_msg);
		return OV_ERR_BADPARAM;
	}
	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query, NULL);
		return OV_ERR_GENERIC;
	}

	ov_string_setvalue(&query, NULL);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_selectData(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING table, const OV_STRING* fields, OV_UINT fieldsLen, const OV_STRING* whereFields,
													   OV_UINT whereFieldsLen, OV_STRING* whereValues, OV_UINT whereValuesLen, OV_STRING_VEC* result) {
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	if(whereFieldsLen != whereValuesLen) {
		return OV_ERR_BADPARAM;
	}
	// build up SELECT query
	OV_STRING query = NULL;
	ov_string_setvalue(&query, "SELECT DISTINCT");
	for(int i = 0; i < fieldsLen; i++) {
		if(i != fieldsLen-1) {
			ov_string_print(&query, "%s %s,", query, fields[i]);
		} else {
			ov_string_print(&query, "%s %s", query, fields[i]);
		}
	}
	if(!fieldsLen) ov_string_print(&query, "%s %s ", query, "*");
	ov_string_print(&query, "%s %s", query, "FROM");
	ov_string_print(&query, "%s %s", query, table);
	if (fieldsLen){
		ov_string_print(&query, "%s %s", query, "WHERE");
		for(int i = 0; i < fieldsLen; i++) {
			if(i != fieldsLen-1) {
				ov_string_print(&query, "%s %s IS NOT NULL AND", query, fields[i]);
				ov_string_print(&query, "%s %s != \"\" AND", query, fields[i]);
			} else {
				ov_string_print(&query, "%s %s IS NOT NULL AND", query, fields[i]);
				if (whereFieldsLen)
					ov_string_print(&query, "%s %s != \"\" AND", query, fields[i]);
				else
					ov_string_print(&query, "%s %s != \"\" ", query, fields[i]);
			}
		}
	}
	if(whereFieldsLen) {
		if (!fieldsLen)	ov_string_print(&query, "%s %s", query, "WHERE");
		for(int i = 0; i < whereFieldsLen; i++) {
			if(i != whereFieldsLen-1) {
				ov_string_print(&query, "%s %s = %s AND", query, whereFields[i], whereValues[i]);
			} else {
				ov_string_print(&query, "%s %s = %s", query, whereFields[i], whereValues[i]);
			}
		}
	}
	ov_string_print(&query, "%s%s", query, ";");

	//ov_logfile_info("%s", query);

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, callback, result, &err_msg);

	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query, NULL);
		return OV_ERR_BADPARAM;
	}

	ov_string_setvalue(&query, NULL);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_deleteData(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING table, const OV_STRING* fields, OV_UINT fieldsLen, const OV_STRING* values, OV_UINT valuesLen) {
	// build up DELETE query
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	OV_STRING query = NULL;
	ov_string_setvalue(&query, "DELETE FROM ");
	ov_string_print(&query, "%s%s ", query, table);
	if(fieldsLen) {
		ov_string_print(&query, "%s%s ", query, "WHERE");
		for(int i = 0; i < fieldsLen; i++) {
			if(i != fieldsLen-1) {
				ov_string_print(&query, "%s%s = %s, ", query, fields[i], values[i]);
			} else {
				ov_string_print(&query, "%s%s = %s;", query, fields[i], values[i]);
			}
		}
	}

	//ov_logfile_info("%s", query);

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, NULL, NULL, &err_msg);

	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query, NULL);
		return OV_ERR_BADPARAM;
	}

	ov_string_setvalue(&query, NULL);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_updateData(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING table, const OV_STRING* fields, OV_UINT fieldsLen, const OV_STRING* fieldValues, OV_UINT fieldValuesLen, const OV_STRING* whereFields, OV_UINT whereFieldsLen, OV_STRING* whereValues, OV_UINT whereValuesLen) {
	// build up UPDATE query
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	OV_STRING query = NULL;
	ov_string_setvalue(&query, "UPDATE");
	ov_string_print(&query, "%s %s", query, table);
	if(fieldsLen) {
		ov_string_print(&query, "%s %s", query, "SET");
		for(int i = 0; i < fieldsLen; i++) {
			if(i != fieldsLen-1) {
				ov_string_print(&query, "%s %s = %s,", query, fields[i], fieldValues[i]);
			} else {
				ov_string_print(&query, "%s %s = %s", query, fields[i], fieldValues[i]);
			}
		}
	}
	if(whereFieldsLen) {
		ov_string_print(&query, "%s %s", query, "WHERE");
		for(int i = 0; i < fieldsLen; i++) {
			if(i != fieldsLen-1) {
				ov_string_print(&query, "%s %s = %s,", query, whereFields[i], whereValues[i]);
			} else {
				ov_string_print(&query, "%s %s = %s", query, whereFields[i], whereValues[i]);
			}
		}
	}
	ov_string_print(&query, "%s%s", query, ";");

	//ov_logfile_info("%s", query);

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, NULL, NULL, &err_msg);

	if (rc == SQLITE_CONSTRAINT){
		ov_string_setvalue(&query, NULL);
		sqlite3_free(err_msg);
		return OV_ERR_BADPARAM;
	}
	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query,NULL);
		return OV_ERR_GENERIC;
	}

	ov_string_setvalue(&query,NULL);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_getComponentID(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING_VEC* table, const DB_QUERY* db_query, OV_UINT querySize, OV_STRING_VEC* result) {
	// TODO: Check Veclen => Errorhandling
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	if(querySize == 0) {
		return OV_ERR_BADPARAM;
	}

	OV_STRING query = NULL;
	OV_BOOL firstWhere = TRUE;
	OV_BOOL firstOr = FALSE;
	OV_BOOL valueExist = FALSE;
	ov_string_setvalue(&query, " ");
    for(OV_UINT i = 0; i < querySize; i++) {
    	for (OV_UINT l = 0; l < table[i].veclen; l++){
			ov_string_print(&query, "%s SELECT DISTINCT ComponentID FROM %s", query, table[i].value[l]);
			for(OV_UINT j = 0; j < db_query[i].value.veclen; j++) {
				if (db_query[i].value.value[j] == NULL)
					continue;
				valueExist = TRUE;
				if (firstWhere == TRUE){
					ov_string_print(&query, "%s  WHERE (%s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_relation.value[j], db_query[i].value.value[j]);
					firstWhere = FALSE;
				}else{
					ov_string_print(&query, "%s AND %s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_relation.value[j], db_query[i].value.value[j]);
				}
			}
			if (valueExist == TRUE)
				ov_string_append(&query, ")");
			for(OV_UINT k = 0; k < (db_query[i].value_optional.veclen)/(db_query[i].column.veclen); k++) {
				if (k == 0 && valueExist == FALSE){
					// Do nothing
				}else {
					ov_string_append(&query, " OR ");
				}
				for(OV_UINT j = 0; j < db_query[i].column.veclen; j++) {
					if (db_query[i].value_optional.value[j + k * db_query[i].column.veclen] == NULL)
						continue;
					if (firstWhere == TRUE){
						ov_string_print(&query, "%s  WHERE (%s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
						firstWhere = FALSE;
					}else if (firstOr == TRUE){
						ov_string_print(&query, "%s (%s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
						firstOr = FALSE;
					}else{
						ov_string_print(&query, "%s AND %s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
					}
				}
				ov_string_append(&query, ")");
				firstOr = TRUE;
			}
			firstWhere = TRUE;
			if (l < table[i].veclen - 1)
				ov_string_append(&query, " UNION ");
    	}
    	if (i < querySize - 1)
    		ov_string_append(&query, " INTERSECT ");
    	firstWhere = TRUE;
    }
	ov_string_append(&query, ";");


	FILE *fp;
	fp = fopen("Sql_log_getComponentID.txt", "a");
	fprintf(fp, "%s", query);
	fclose(fp);

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, callback, result, &err_msg);

	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query, NULL);
		return OV_ERR_BADPARAM;
	}

	ov_string_setvalue(&query, NULL);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_getFittingStatements(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING_VEC* table, const OV_STRING ComponentID, const DB_QUERY* db_query, OV_UINT querySize, OV_STRING_VEC* result) {
	// TODO: Check Veclen => Errorhandling
	OV_INSTPTR_Databases_SQLite3 pinst = Ov_StaticPtrCast(Databases_SQLite3, this);
	if(querySize == 0) {
		return OV_ERR_BADPARAM;
	}
	OV_STRING query = NULL;
	OV_BOOL firstWhere = TRUE;
	OV_BOOL firstOr = FALSE;
	OV_BOOL valueExist = FALSE;
	ov_string_setvalue(&query, "SELECT DISTINCT * FROM (");

	for(OV_UINT i = 0; i < querySize; i++) {
		for (OV_UINT l = 0; l < table[i].veclen; l++){
			ov_string_print(&query, "%s SELECT * FROM %s", query, table[i].value[l]);
			for(OV_UINT j = 0; j < db_query[i].value.veclen; j++) {
				if (db_query[i].value.value[j] == NULL){
					if (firstWhere == TRUE){
						ov_string_print(&query, "%s  WHERE (ComponentID='%s'", query, ComponentID);
						firstWhere = FALSE;
					}
					continue;
				}
				valueExist = TRUE;
				if (firstWhere == TRUE){
					ov_string_print(&query, "%s  WHERE (ComponentID='%s' AND %s %s '%s'", query, ComponentID, db_query[i].column.value[j], db_query[i].value_relation.value[j], db_query[i].value.value[j]);
					firstWhere = FALSE;
				}else{
					ov_string_print(&query, "%s AND %s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_relation.value[j], db_query[i].value.value[j]);
				}
			}
			if (db_query[i].value.veclen > 0)
				ov_string_append(&query, ")");
			for(OV_UINT k = 0; k < (db_query[i].value_optional.veclen)/(db_query[i].column.veclen); k++) {
				if (k == 0 && valueExist == FALSE){
					// Do nothing
				}else {
					ov_string_append(&query, " OR ");
				}
				for(OV_UINT j = 0; j < db_query[i].column.veclen; j++) {
					if (db_query[i].value_optional.value[j + k * db_query[i].column.veclen] == NULL){
						if (firstWhere == TRUE){
							ov_string_print(&query, "%s  WHERE (ComponentID='%s'", query, ComponentID);
							firstWhere = FALSE;
						}
						continue;
					}
					if (firstWhere == TRUE){
						ov_string_print(&query, "%s  WHERE (ComponentID='%s' AND %s %s '%s'", query, ComponentID, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
						firstWhere = FALSE;
					}else if (firstOr == TRUE){
						ov_string_print(&query, "%s (ComponentID='%s' AND %s %s '%s'", query, ComponentID, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
						firstOr = FALSE;
					}else{
						ov_string_print(&query, "%s AND %s %s '%s'", query, db_query[i].column.value[j], db_query[i].value_optional_relation.value[j + k * db_query[i].column.veclen], db_query[i].value_optional.value[j + k * db_query[i].column.veclen]);
					}
				}
				ov_string_append(&query, ")");
				firstOr = TRUE;
			}
			if (l < table[i].veclen - 1)
				ov_string_append(&query, " UNION ");
			firstWhere = TRUE;
		}
		if (i < querySize - 1)
			ov_string_append(&query, " UNION ");
		firstWhere = TRUE;
	}
	ov_string_append(&query, ");");

	char* err_msg = NULL;
	rc = sqlite3_exec(pinst->v_db, query, callbackFittingStatements, result, &err_msg);

	if(rc != SQLITE_OK) {
		ov_logfile_info("SQL Error: %s", err_msg);
		sqlite3_free(err_msg);
		ov_string_setvalue(&query, NULL);
		return OV_ERR_BADPARAM;
	}

	ov_string_setvalue(&query, NULL);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_execQuery(OV_INSTPTR_openAASDiscoveryServer_DBWrapper this, const OV_STRING query) {

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_query_set(
    OV_INSTPTR_Databases_SQLite3          pobj,
    const OV_STRING  value
) {
	if(!value) {
		const char* data = "Callback funtion called";
		char* err_msg = NULL;

		ov_string_setvalue(&pobj->v_query,value);
		//ov_logfile_info("%s", pobj->v_query);
		rc = sqlite3_exec(pobj->v_db, pobj->v_query, callback, (void*)data, &err_msg);

		if( rc != SQLITE_DONE ) {
			ov_logfile_info("SQL error: %s", err_msg);
			return OV_ERR_GENERIC;
		}
	}

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT Databases_SQLite3_io_set(
    OV_INSTPTR_Databases_SQLite3          pobj,
    const OV_BOOL  value
) {
	pobj->v_io = value;
/*
	if(pobj->v_io) {
		Databases_SQLite3_readData();
	}
	else {
		Databases_SQLite3_writeData();
	}*/

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT void Databases_SQLite3_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */

    /* do what */
	OV_INSTPTR_openAASDiscoveryServer_Wrapper pinst = Ov_StaticPtrCast(openAASDiscoveryServer_Wrapper, pobj);
    Databases_SQLite3_disconnect(pinst);

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT void Databases_SQLite3_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */

    /* do what the base class does first */
    ov_object_startup(pobj);

    OV_INSTPTR_openAASDiscoveryServer_Wrapper pinst = Ov_StaticPtrCast(openAASDiscoveryServer_Wrapper, pobj);
    Databases_SQLite3_connect(pinst);

    return;
}
