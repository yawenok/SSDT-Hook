#include "SSDTHook.h"

#define GetSSDTIndex(ServiceFunction)             (*(PULONG)((PUCHAR)ServiceFunction + 1))

#define GetAddressFromSSDT(ServiceFunction)       KeServiceDescriptorTable.ntoskrnl.ServiceTableBase[GetSSDTIndex(ServiceFunction)]

static ULONG sg_oldSSDT[MAX_SYSTEM_SERVICE_NUMBER];

VOID RestoreWriteProtectBit(ULONG oldAttr)
{
	_asm
	{
		push eax;
		mov  eax, oldAttr;
		mov  cr0, eax;
		pop  eax;
		sti;
	}
}

VOID DisableWriteProtect(PULONG pOldAttr)
{
	ULONG uAttr = 0; 

	_asm 
	{ 
		cli;
		push eax;
		mov  eax,   cr0; 
		mov  uAttr, eax; 
		and  eax,   0xFFFEFFFF; // CR0 16 BIT = 0 
		mov  cr0,   eax; 
		pop  eax;
	}; 

	//保存原有的 CRO 属性 
	*pOldAttr = uAttr; 
}

BOOLEAN InitSSDTHook()
{
	ULONG i = 0;

	KdPrint(("SSDT NumbeOfService:%d\r\n",KeServiceDescriptorTable.ntoskrnl.NumberOfService));

 	for (i = 0; (i < KeServiceDescriptorTable.ntoskrnl.NumberOfService) && (i < MAX_SYSTEM_SERVICE_NUMBER); i++)
 	{
 		sg_oldSSDT[i] = KeServiceDescriptorTable.ntoskrnl.ServiceTableBase[i];
 		KdPrint(("SSDT - Function Information { Number: 0x%04X , Address: %08X}\r\n", i, sg_oldSSDT[i]));
 	}

	return TRUE;
}

ULONG GetAddressFromBackupSSDT(ULONG Service)
{
	return sg_oldSSDT[GetSSDTIndex(Service)];
}

NTSTATUS InstallSSDTHook(ULONG oldService, ULONG newService)
{
	ULONG uOldAttr = 0;

	DisableWriteProtect(&uOldAttr);
	InterlockedExchange(&GetAddressFromSSDT(oldService),newService);
	RestoreWriteProtectBit(uOldAttr);

	return STATUS_SUCCESS;
}

NTSTATUS UnInstallSSDTHook(ULONG oldService)
{
	ULONG uOldAttr = 0;

	DisableWriteProtect(&uOldAttr);
	InterlockedExchange(&GetAddressFromSSDT(oldService),GetAddressFromBackupSSDT(oldService));
	RestoreWriteProtectBit(uOldAttr);

	return STATUS_SUCCESS;
}
