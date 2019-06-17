/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

EXTERN_C_START

typedef struct {
	ULONG regs[128];
} IMXVPU_REGISTERS;

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct {
	IMXVPU_REGISTERS* RegistersPtr;
	WDFDEVICE WdfDevice;
	WDFINTERRUPT WdfInterrupt;
	WDFREQUEST CurrentRequest;
	WDFSTRING DeviceInterfaceSymlinkName;
	UNICODE_STRING DeviceInterfaceSymlinkNameWsz;

	//
	// HW State
	//

	//
	// A lock to protect the vpu
	//
	WDFWAITLOCK ControllerLock;

} IMXVPU_DEVICE_CONTEXT; // struct IMXVPU_DEVICE_CONTEXT

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(IMXVPU_DEVICE_CONTEXT, ImxVpuGetDeviceContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
OnCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

EXTERN_C_END
