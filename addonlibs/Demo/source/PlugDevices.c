
/******************************************************************************
*
*   FILE
*   ----
*   PlugDevices.c
*
*   History
*   -------
*   2017-11-20   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_Demo
#define OV_COMPILE_LIBRARY_Demo
#endif


#include "Demo.h"
#include "libov/ov_macros.h"



OV_DLLFNCEXPORT OV_RESULT Demo_PlugDevices_CallMethod(      
  OV_INSTPTR_services_Service pobj,       
  OV_UINT numberofInputArgs,       
  const void **packedInputArgList,       
  OV_UINT numberofOutputArgs,      
  void **packedOutputArgList,
  OV_UINT *typeArray       
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_Demo_PlugDevices pPlugDevices = Ov_DynamicPtrCast(Demo_PlugDevices, pobj);
	OV_INSTPTR_Demo_CallApp pCallApp = Ov_DynamicPtrCast(Demo_CallApp, &pPlugDevices->p_CallApp);
	Demo_CallApp_START_set(pCallApp, TRUE);
    return OV_ERR_OK;
}
