#include "HookFile.h"

typedef NTSTATUS
(* PHookNtDeleteFile)(
					  IN POBJECT_ATTRIBUTES  ObjectAttributes
					  );

typedef NTSTATUS 
(* PHookNtReadFile)(
			   IN HANDLE  FileHandle,
			   IN HANDLE  Event  OPTIONAL,
			   IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
			   IN PVOID  ApcContext  OPTIONAL,
			   OUT PIO_STATUS_BLOCK  IoStatusBlock,
			   OUT PVOID  Buffer,
			   IN ULONG  Length,
			   IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
			   IN PULONG  Key  OPTIONAL
			   );

typedef NTSTATUS 
(* PHookNtSetInformationFile)(
							  IN HANDLE  FileHandle,
							  OUT PIO_STATUS_BLOCK  IoStatusBlock,
							  IN PVOID  FileInformation,
							  IN ULONG  Length,
							  IN FILE_INFORMATION_CLASS  FileInformationClass
							  );

NTSTATUS
HookNtDeleteFile(
				 IN POBJECT_ATTRIBUTES  ObjectAttributes
				 )
{
	PHookNtDeleteFile pReal = (PHookNtDeleteFile)GetAddressFromBackupSSDT((ULONG)ZwDeleteFile);

	return pReal(ObjectAttributes);
}

NTSTATUS 
HookNtReadFile(
			   IN HANDLE  FileHandle,
			   IN HANDLE  Event  OPTIONAL,
			   IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
			   IN PVOID  ApcContext  OPTIONAL,
			   OUT PIO_STATUS_BLOCK  IoStatusBlock,
			   OUT PVOID  Buffer,
			   IN ULONG  Length,
			   IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
			   IN PULONG  Key  OPTIONAL
			   )
{
	PFILE_OBJECT	pfile_Obj;
	NTSTATUS		rtStatus  = ObReferenceObjectByHandle(FileHandle, GENERIC_READ, *IoFileObjectType, KernelMode, (PVOID*)&pfile_Obj, 0);
	PHookNtReadFile pReal	  = (PHookNtReadFile)GetAddressFromBackupSSDT((ULONG)ZwReadFile);

	KdPrint(("==>:%s\r\n",__FUNCTION__));

	if (NT_SUCCESS(rtStatus))
	{
		if (NT_SUCCESS(rtStatus))
		{
			KdPrint(("FileName:%ws\r\n",pfile_Obj->FileName.Buffer));
		}
	}

	KdPrint(("<==:%s\r\n",__FUNCTION__));

	return pReal(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,Buffer,Length,ByteOffset,Key);
}

NTSTATUS 
HookNtSetInformationFile(
						 IN HANDLE  FileHandle,
						 OUT PIO_STATUS_BLOCK  IoStatusBlock,
						 IN PVOID  FileInformation,
						 IN ULONG  Length,
						 IN FILE_INFORMATION_CLASS  FileInformationClass
						 )
{
	PFILE_OBJECT			  pfile_Obj;
	NTSTATUS				  rtStatus  = ObReferenceObjectByHandle(FileHandle, GENERIC_READ, *IoFileObjectType, KernelMode, (PVOID*)&pfile_Obj, 0);
	PHookNtSetInformationFile pReal	    = (PHookNtSetInformationFile)GetAddressFromBackupSSDT((ULONG)ZwSetInformationFile);

	KdPrint(("==>:%s\r\n",__FUNCTION__));

	if (NT_SUCCESS(rtStatus))
	{
		KdPrint(("FileName:%ws\r\n",pfile_Obj->FileName.Buffer));
	}

	KdPrint(("<==:%s\r\n",__FUNCTION__));

	return pReal(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
}
