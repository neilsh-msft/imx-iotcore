/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

Module Name:

    imxvpu_driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#ifndef __IMXVPU_DRIVER_H
#define __IMXVPU_DRIVER_H

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

//
// VPU KM pool allocation tags
//
enum VPU_KM_ALLOC_TAG {

	VPU_KM_ALLOC_TAG_TEMP = '0VPU', // Temporary be freed in the same routine
	VPU_KM_ALLOC_TAG_WDF = '@VPU'  // Allocations WDF makes on our behalf

}; // enum VPU_KM_ALLOC_TAG

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD OnDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP OnDriverContextCleanup;

EXTERN_C_END

#endif