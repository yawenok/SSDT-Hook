#ifndef __SSDTHOOK_H__
#define __SSDTHOOK_H__

#include "Base.h"

BOOLEAN InitSSDTHook();

NTSTATUS InstallSSDTHook(ULONG oldService, ULONG newService);

NTSTATUS UnInstallSSDTHook(ULONG oldService);

#endif	// __SSDTHOOK_H__
