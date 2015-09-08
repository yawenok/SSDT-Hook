#include "HookProcess.h"

typedef NTSTATUS (* PNtTerminateProcess)(
	IN HANDLE ProcessHandle,
	IN NTSTATUS ExitStatus
	);

NTSTATUS HookNtTerminateProcess(
								IN HANDLE ProcessHandle,
								IN NTSTATUS ExitStatus
								)
{

	NTSTATUS  rtStatus	   = STATUS_SUCCESS;
	PEPROCESS pEProcess	   = NULL;
	PCHAR	  pStrProcName = NULL;
	ULONG	  uPID		   = 0;

	// 通过进程句柄来获得该进程所对应的 FileObject 对象
	rtStatus = ObReferenceObjectByHandle(ProcessHandle, FILE_READ_DATA, NULL, KernelMode, &pEProcess, NULL);
	if(!NT_SUCCESS(rtStatus))
	{
		return rtStatus;
	}

	// pOldNtTerminateProcess = (PNtTerminateProcess)GetBackupSysServiceAddr((ULONG)ZwTerminateProcess);

	uPID = (ULONG)PsGetProcessId(pEProcess);

	pStrProcName = (PCHAR)PsGetProcessImageFileName(pEProcess);

	DbgPrint("HookNtTerminateProcess:%s\n",pStrProcName);

	PsGetProcessId(PsGetCurrentProcess());

	// rtStatus = pOldNtTerminateProcess(ProcessHandle, ExitStatus);

	return rtStatus;
}

ULONG TerminateProcess(HANDLE hProcess)   
{   
	HANDLE			  hJob     = NULL;
	NTSTATUS		  rtStatus = STATUS_SUCCESS;
	OBJECT_ATTRIBUTES objOa;

	RtlZeroMemory(&objOa,sizeof(OBJECT_ATTRIBUTES)); 
	objOa.Length = sizeof(OBJECT_ATTRIBUTES);  
	rtStatus = ZwCreateJobObject(&hJob, 0, &objOa);
	if(NT_SUCCESS(rtStatus))   
	{   
		ZwAssignProcessToJobObject(hJob,(HANDLE)hProcess);   
		ZwTerminateJobObject((HANDLE)hJob,0);   
		ZwClose(hJob);   
		ZwClose((HANDLE)hProcess);   
		return TRUE;
	}   
	return FALSE;
}   

NTSTATUS OpenProcess(
					 IN ULONG uPID, 
					 OUT PHANDLE pHandle,
					 IN ACCESS_MASK DesiredAccess)  
{   
	NTSTATUS       rtStatus       = STATUS_SUCCESS;   
	PEPROCESS      pEProcess      = NULL;   
	HANDLE         hTagProcess    = NULL;   
	PULONG         uPsProcessType = 0;   
	UNICODE_STRING StrType;

	rtStatus = PsLookupProcessByProcessId((HANDLE)uPID, &pEProcess);   
	if (NT_SUCCESS(rtStatus))   
	{    
		RtlInitUnicodeString(&StrType, L"PsProcessType");   
		uPsProcessType = (PULONG)MmGetSystemRoutineAddress(&StrType);   
		if (uPsProcessType)   
		{   
			rtStatus = ObOpenObjectByPointer(pEProcess, 0, 0, DesiredAccess, (POBJECT_TYPE)*uPsProcessType, UserMode, &hTagProcess);   
			if (NT_SUCCESS(rtStatus))   
			{   
				*pHandle = hTagProcess;   
			}   
		}   
		ObfDereferenceObject(pEProcess);   
	}    
	return rtStatus;   
}   

ULONG KillProcess(ULONG uPID)   
{   
	HANDLE   hProcess = NULL;   
	NTSTATUS rtStatus = OpenProcess(uPID,&hProcess,1); 

	if(rtStatus != STATUS_SUCCESS)
		return FALSE;

	return TerminateProcess(hProcess);   
}