/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    Kernel-mode Driver Framework

*/

#include "imxvpu_driver.h"
#include "device.tmh"

IMXVPU_PAGED_SEGMENT_BEGIN; //==================================================

_Use_decl_annotations_
NTSTATUS
ImxVpuEvtDevicePrepareHardware(
	WDFDEVICE WdfDevice,
	WDFCMRESLIST ResourcesRaw,
	WDFCMRESLIST ResourcesTranslated
)
{
	PAGED_CODE();
	IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);
	(void *)ResourcesRaw;
	const CM_PARTIAL_RESOURCE_DESCRIPTOR* memResourcePtr = NULL;
	ULONG interruptResourceCount = 0;

	//
	// Look for single memory and interrupt resource.
	//
	const ULONG resourceCount = WdfCmResourceListGetCount(ResourcesTranslated);
	for (ULONG i = 0; i < resourceCount; ++i) {
		const CM_PARTIAL_RESOURCE_DESCRIPTOR* resourcePtr =
			WdfCmResourceListGetDescriptor(ResourcesTranslated, i);

		switch (resourcePtr->Type) {
		case CmResourceTypeMemory:
			if (memResourcePtr != NULL) {
				IMXVPU_LOG_ERROR(
					"Received unexpected memory resource. (resourcePtr = 0x%p)",
					resourcePtr);

				return STATUS_DEVICE_CONFIGURATION_ERROR;
			}

			memResourcePtr = resourcePtr;
			break;

		case CmResourceTypeInterrupt:
			if (interruptResourceCount > 3) {
				IMXVPU_LOG_ERROR(
					"Received unexpected interrupt resource. "
					"(interruptResourceCount = %lu, resourcePtr = 0x%p)",
					interruptResourceCount,
					resourcePtr);

				return STATUS_DEVICE_CONFIGURATION_ERROR;
			}

			++interruptResourceCount;
			break;
		}
	}

	if ((memResourcePtr == NULL) || (interruptResourceCount != 3)) {
		IMXVPU_LOG_ERROR(
			"Did not receive required memory resource and interrupt resource. "
			"(memResourcePtr = 0x%p, interruptResourceCount = %lu)",
			memResourcePtr,
			interruptResourceCount);

		return STATUS_DEVICE_CONFIGURATION_ERROR;
	}

	if (memResourcePtr->u.Memory.Length < sizeof(IMXVPU_REGISTERS)) {
		IMXVPU_LOG_ERROR(
			"Memory resource is too small. "
			"(memResourcePtr->u.Memory.Length = %lu, "
			"sizeof(IMXVPU_REGISTERS) = %lu)",
			memResourcePtr->u.Memory.Length,
			sizeof(IMXVPU_REGISTERS));

		return STATUS_DEVICE_CONFIGURATION_ERROR;
	}

	//
	// ReleaseHardware is ALWAYS called, even if PrepareHardware fails, so
	// the cleanup of registersPtr is handled there
	//
	IMXVPU_DEVICE_CONTEXT* deviceContextPtr =
		ImxVpuGetDeviceContext(WdfDevice);

	NT_ASSERT(memResourcePtr->Type == CmResourceTypeMemory);
	deviceContextPtr->RegistersPtr = (IMXVPU_REGISTERS*)
		MmMapIoSpaceEx(
			memResourcePtr->u.Memory.Start,
			sizeof(IMXVPU_REGISTERS),
			PAGE_READWRITE | PAGE_NOCACHE);

	if (deviceContextPtr->RegistersPtr == NULL) {
		IMXVPU_LOG_LOW_MEMORY(
			"MmMapIoSpaceEx(...) failed. "
			"(memResourcePtr->u.Memory.Start = 0x%llx, "
			"sizeof(IMXVPU_REGISTERS) = %lu)",
			memResourcePtr->u.Memory.Start.QuadPart,
			sizeof(IMXVPU_REGISTERS));

		return STATUS_INSUFFICIENT_RESOURCES;
	}

	return STATUS_SUCCESS;
}

_Use_decl_annotations_
NTSTATUS
ImxVpuEvtDeviceReleaseHardware(
	WDFDEVICE WdfDevice,
	WDFCMRESLIST ResourcesTranslated
)
{
	PAGED_CODE();
	IMXVPU_ASSERT_MAX_IRQL(PASSIVE_LEVEL);

	(void *)ResourcesTranslated;
	IMXVPU_DEVICE_CONTEXT* deviceContextPtr =
		ImxVpuGetDeviceContext(WdfDevice);

	if (deviceContextPtr->RegistersPtr != NULL) {
		MmUnmapIoSpace(
			deviceContextPtr->RegistersPtr,
			sizeof(IMXVPU_REGISTERS));

		deviceContextPtr->RegistersPtr = NULL;
	}

	return STATUS_SUCCESS;
}


NTSTATUS
OnCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    IMXVPU_DEVICE_CONTEXT* deviceContext;
    WDFDEVICE device;
    NTSTATUS status;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, IMXVPU_DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = ImxVpuGetDeviceContext(device);

        //
        // Initialize the context.
        //
        memset(deviceContext, 0, sizeof(IMXVPU_DEVICE_CONTEXT));

        //
        // Create a device interface so that applications can find and talk
        // to us.
        //
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_imxvpukm,
            NULL // ReferenceString
            );

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
            status = OnQueueInitialize(device);
        }
    }

    return status;
}
