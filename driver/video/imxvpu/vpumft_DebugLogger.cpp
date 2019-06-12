#include "vpumft_DebugLogger.h"
#ifdef  _HMFTLOGGING_
#include <evntrace.h>
#include <strsafe.h>

REGHANDLE    g_ETWHandle    = NULL;
BOOL         g_bEnabled     = FALSE;
UCHAR        g_ucLevel      = 0;

void NTAPI EventRegisterCallback(
    LPCGUID                     lpcguidSourceId,
    ULONG                       ulIsEnabled,
    UCHAR                       ucLevel,
    ULONGLONG                   ullMatchAnyKeyword,
    ULONGLONG                   ullMatchAllKeywords,
    PEVENT_FILTER_DESCRIPTOR    pefdFilterData,
    PVOID                       pvCallbackContext)
{
    switch(ulIsEnabled)
    {
    case EVENT_CONTROL_CODE_ENABLE_PROVIDER:  
        g_bEnabled  = TRUE;
        g_ucLevel   = ucLevel;
        break;
    case EVENT_CONTROL_CODE_DISABLE_PROVIDER:  
        g_bEnabled  = FALSE;
        g_ucLevel   = 0;
        break;
    default:
        // Nothing to do
        break;
    };
}
#endif

void TraceInitialize(void)
{
#ifdef  _HMFTLOGGING_
    if(g_bEnabled == FALSE)
    {
        // Provider ID: {2183964F-3782-48A6-9E93-52580016156E}
        static const GUID guidTrace =
            { 0x2183964f, 0x3782, 0x48a6, { 0x9e, 0x93, 0x52, 0x58, 0x0, 0x16, 0x15, 0x6e } };

        EventRegister(
            &guidTrace,
            &EventRegisterCallback,
            NULL,
            &g_ETWHandle
            );
    }
#endif
}

void TraceUninitialize(void)
{
#ifdef  _HMFTLOGGING_
    if(g_ETWHandle != NULL)
    {
        EventUnregister(g_ETWHandle);
    }

    g_ETWHandle = NULL;
    g_bEnabled  = FALSE;
    g_ucLevel   = 0;
#endif
}


void TraceString(
    const UCHAR ucLevel,
    const WCHAR* pwszFormat, 
    ...)
{
#ifdef  _HMFTLOGGING_
    do
    {
        if(g_bEnabled == FALSE)
        {
            // Do not trace
            break;
        }

        if( (ucLevel > g_ucLevel)   &&
            (g_ucLevel != 0)        )
        {
            // Do not trace
            break;
        }

        WCHAR   pwszTrace[1024] = {0};

        va_list args;
        va_start(args, pwszFormat);

        StringCchVPrintfW(
            pwszTrace,
            ARRAYSIZE(pwszTrace),
            pwszFormat,
            args
            );

        va_end(args);

        EventWriteString(g_ETWHandle, ucLevel, 0, pwszTrace);
    }while(false);
#endif
}