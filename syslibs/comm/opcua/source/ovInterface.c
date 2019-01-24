
/******************************************************************************
*
*   FILE
*   ----
*   ovInterface.c
*
*   History
*   -------
*   2019-01-24   File created
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
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT opcua_ovInterface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovInterface pinst = Ov_StaticPtrCast(opcua_ovInterface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_ovInterface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovInterface pinst = Ov_StaticPtrCast(opcua_ovInterface, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

