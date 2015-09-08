#ifndef __HOOKPROCESS_H
#define __HOOKPROCESS_H

#include "SSDTHook.h"

NTSTATUS HookNtTerminateProcess(
								IN HANDLE ProcessHandle,
								IN NTSTATUS ExitStatus
								);

NTSTATUS OpenProcess(
					 IN ULONG uPID, 
					 OUT PHANDLE pHandle,
					 IN ACCESS_MASK DesiredAccess);

ULONG TerminateProcess(HANDLE hProcess);

ULONG KillProcess(ULONG uPID);

#endif	// __HOOKPROCESS_H
