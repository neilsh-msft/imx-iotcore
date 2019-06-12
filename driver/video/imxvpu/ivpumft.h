#pragma once

#include <Mfidl.h>

// {B47516A2-FA53-4B38-B1FA-CAFD12D030A8}
DEFINE_GUID(IID_IVPUMFT,
0xb47516a2, 0xfa53, 0x4b38, 0xb1, 0xfa, 0xca, 0xfd, 0x12, 0xd0, 0x30, 0xa8);

class IVpuMft:
    public IUnknown
{
public:
    virtual HRESULT __stdcall   DecodeInputFrame(
                                            IMFSample*  pInputSample
                                            )  = 0;
};