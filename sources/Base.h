#ifndef __BASE_H__
#define __BASE_H__

#ifdef __cplusplus
extern "C" 
{
#endif

#include <ntifs.h>
#include <ntddk.h>

#ifdef __cplusplus
}
#endif

//=====================================================================================//
//Name: KSYSTEM_SERVICE_TABLE 和 KSERVICE_TABLE_DESCRIPTOR					           //
//                                                                                     //
//Descripion: 用来定义 SSDT 结构 for 32										  	   //
//            				                            						       //
//=====================================================================================//
typedef struct _KSYSTEM_SERVICE_TABLE
{
	PULONG  ServiceTableBase;					// SSDT (System Service Dispatch Table)的基地址
	PULONG  ServiceCounterTableBase;			// 用于 checked builds, 包含 SSDT 中每个服务被调用的次数
	ULONG   NumberOfService;					// 服务函数的个数, NumberOfService * 4 就是整个地址表的大小
	PVOID   ParamTableBase;						// SSPT(System Service Parameter Table)的基地址  
} KSYSTEM_SERVICE_TABLE, *PKSYSTEM_SERVICE_TABLE;

typedef struct _KSERVICE_TABLE_DESCRIPTOR
{
	KSYSTEM_SERVICE_TABLE   ntoskrnl;			// ntoskrnl.exe 的服务函数
	KSYSTEM_SERVICE_TABLE   win32k;				// win32k.sys 的服务函数(GDI32.dll/User32.dll 的内核支持)
	KSYSTEM_SERVICE_TABLE   notUsed1;
	KSYSTEM_SERVICE_TABLE   notUsed2;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;


#define MAX_SYSTEM_SERVICE_NUMBER                 1024
#define MAX_SEARCH_FUNTION_NUMBER				  4096

__declspec(dllimport) KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable; 

#endif	// __BASE_H__
