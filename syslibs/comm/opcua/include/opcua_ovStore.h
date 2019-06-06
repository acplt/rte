/*
 * ovStore.h
 *
 *  Created on: 23.02.2019
 *      Author: Julian Grothoff
 *
 */

#ifndef opcua_ovStore_H_
#define opcua_ovStore_H_

#include "opcua.h"

#define OPCUA_OVSTORE_DEFAULTNSINDEX 1
#define OPCUA_OVSTORE_DEFAULTNSURI_PREFIX OPCUA_DEFAULT_APPLICATIONURI
#define OPCUA_OVSTORE_DEFAULTNSURI_SUFFIX "/types"

OV_BOOL opcua_ovStore_addReferenceToSpecificObject(OV_INSTPTR_opcua_server pServer, OV_INSTPTR_ov_object pobj, UA_Node* node);
UA_StatusCode opcua_ovStore_addReferences(OV_INSTPTR_opcua_interface pInterface, UA_Node *node);
UA_NodestoreInterface* opcua_ovStore_new(OV_INSTPTR_opcua_ovInterface context);
void opcua_ovStore_delete(UA_NodestoreInterface * nodestoreInterface);

#endif /* opcua_ovStore_H_ */
