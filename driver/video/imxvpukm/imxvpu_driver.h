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

//
// Macros to be used for proper PAGED/NON-PAGED code placement
//

#define IMXVPU_NONPAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    //__pragma(code_seg(.text))

#define IMXVPU_NONPAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_PAGED_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("PAGE"))

#define IMXVPU_PAGED_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_INIT_SEGMENT_BEGIN \
    __pragma(code_seg(push)) \
    __pragma(code_seg("INIT"))

#define IMXVPU_INIT_SEGMENT_END \
    __pragma(code_seg(pop))

#define IMXVPU_ASSERT_MAX_IRQL(Irql) NT_ASSERT(KeGetCurrentIrql() <= (Irql))

EXTERN_C_START

//
// VPU KM pool allocation tags
//
enum VPU_KM_ALLOC_TAG {

//	VPU_KM_ALLOC_TAG_TEMP = '0VPU', // Temporary be freed in the same routine
	VPU_KM_ALLOC_TAG_WDF = '@VPU'  // Allocations WDF makes on our behalf

}; // enum VPU_KM_ALLOC_TAG

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;

EVT_WDF_OBJECT_CONTEXT_CLEANUP OnDriverContextCleanup;

EVT_WDF_DEVICE_PREPARE_HARDWARE ImxVpuEvtDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE ImxVpuEvtDeviceReleaseHardware;
EVT_WDF_DRIVER_DEVICE_ADD ImxVpuEvtDeviceAdd;
EVT_WDF_DRIVER_UNLOAD ImxVpuDriverUnload;
EVT_WDF_DEVICE_FILE_CREATE ImxVpuEvtDeviceFileCreate;
EVT_WDF_FILE_CLOSE ImxVpuEvtFileClose;

EXTERN_C_END

#endif