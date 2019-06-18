/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_imxvpukm,
    0xada9253b,0x628c,0x40ce,0xb2,0xc1,0x19,0xf4,0x89,0xa0,0xf3,0xda);
// {ada9253b-628c-40ce-b2c1-19f489a0f3da}

// read # of Cores : out Buffer=(ULONG)
#define IOCTL_VPU_MC_CORES     CTL_CODE(FILE_DEVICE_DEVAPI, 0x00, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define IOCTL_VPU_MC_OFFSETS   CTL_CODE(FILE_DEVICE_DEVAPI, 0x01, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Claim core ID for passed ClientType : in buffer=(ULONG), out return code core ID (negative on error)
#define IOCTL_VPU_CORE_ID     CTL_CODE(FILE_DEVICE_DEVAPI, 0x02, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Get ASIC id for core ID : in/out buffer=(ULONG)  input = core_id, output=asic_id
#define IOCTL_VPU_ASIC_ID     CTL_CODE(FILE_DEVICE_DEVAPI, 0x03, METHOD_BUFFERED, FILE_ANY_ACCESS)

// HW_OFFSET
// HWIO_SIZE
// IOC_MC_OFFSETS

EXTERN_C_START

typedef struct {
	ULONG regs[128];
} IMXVPU_REGISTERS;

EXTERN_C_END

