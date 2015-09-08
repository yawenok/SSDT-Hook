#ifndef __HOOKFILE_H
#define __HOOKFILE_H

#include "SSDTHook.h"

NTSTATUS
HookNtDeleteFile(
				 IN POBJECT_ATTRIBUTES  ObjectAttributes
				 );

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
			   );

NTSTATUS 
HookNtSetInformationFile(
						 IN HANDLE  FileHandle,
						 OUT PIO_STATUS_BLOCK  IoStatusBlock,
						 IN PVOID  FileInformation,
						 IN ULONG  Length,
						 IN FILE_INFORMATION_CLASS  FileInformationClass
						 );


#endif	// __HOOKFILE_H
