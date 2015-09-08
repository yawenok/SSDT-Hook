#ifndef __SHADOWHOOK_H__
#define __SHADOWHOOK_H__

#include "Base.h"

BOOLEAN InitShadowHook();

NTSTATUS InstallShadowHook(ULONG oldService, ULONG newService);

NTSTATUS UnInstallShadowHook(ULONG oldService);

#endif	// __SHADOWHOOK_H__
