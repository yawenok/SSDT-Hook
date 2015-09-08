#ifndef __ROOTKIT_H
#define __ROOTKIT_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <ntifs.h>
#include <ntddk.h>
#include <tdiinfo.h>
#include <tdistat.h>
#include <devioctl.h> 

#ifdef __cplusplus
}
#endif

#include <stdlib.h>
#include "SSDTHook.h"
#include "ShadowHook.h"
#include "HookFile.h"

#define DEVICE_NAME_PROCESS				L"\\Device\\RootKit_Grd"
#define SYMBOLINK_NAME_PROCESS			L"\\??\\RootKit_Grd"

#define	ROOTKIT_DEVICE_TYPE				FILE_DEVICE_UNKNOWN

#define	IO_INSERT_ADD_PROTECTED			(ULONG) CTL_CODE(ROOTKIT_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IO_INSERT_DEL_PROTECTED			(ULONG) CTL_CODE(ROOTKIT_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IO_INSERT_QUE_PROTECTED			(ULONG) CTL_CODE(ROOTKIT_DEVICE_TYPE, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)


NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING  pRegistryPath);

VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject);

NTSTATUS Create(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS Close(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS Read(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS Write(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS Dispatch(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS IoControl(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);


#endif	// __ROOTKIT_H
