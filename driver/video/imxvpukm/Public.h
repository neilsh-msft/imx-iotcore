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
