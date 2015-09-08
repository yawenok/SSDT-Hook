#include "ShadowHook.h"

BOOLEAN __declspec(dllimport) _stdcall KeAddSystemServiceTable(PVOID, BOOLEAN, PVOID, PVOID, PVOID); 

ULONG GetCsrssProcessId()
{

	return 0;
}

ULONG GetAddressOfShadowTable()   
{   
	ULONG  uAddress = 0;
	ULONG  i		= 0;
	PULONG pAddress = (PULONG)KeAddSystemServiceTable;   

	for (i = 0; i < 4096; i++, pAddress++)   
	{   
		__try   
		{   
			uAddress = *pAddress;   
		}   
		__except(EXCEPTION_EXECUTE_HANDLER)   
		{   
			return 0;   
		}   

		if (MmIsAddressValid((PVOID)uAddress))   
		{   
			if (RtlEqualMemory((PVOID)uAddress, &KeServiceDescriptorTable, sizeof(ULONG)))   
			{   
				if ((PVOID)uAddress == &KeServiceDescriptorTable)   
				{   
					continue;   
				}
				return uAddress;   
			}   
		}   
	}   

	return 0;   
}

BOOLEAN InitShadowHook()
{
	return TRUE;
}

NTSTATUS InstallShadowHook(ULONG oldService, ULONG newService)
{
	return STATUS_SUCCESS;
}

NTSTATUS UnInstallShadowHook(ULONG oldService)
{
	return STATUS_SUCCESS;
}
