/* Style: do { } while (FALSE) (Windows kernel driver, needs the WDK) */
#include <ntddk.h>

#define CFG_TAG       'gfCR'
#define CFG_MAX_SIZE  (1024 * 1024)

NTSTATUS
ReadConfigFile(
    _In_  PUNICODE_STRING Path,
    _Out_ PVOID          *Buffer,
    _Out_ PULONG          Length
    )
{
    NTSTATUS                  status = STATUS_UNSUCCESSFUL;
    HANDLE                    file   = NULL;
    PVOID                     data   = NULL;
    OBJECT_ATTRIBUTES         oa;
    IO_STATUS_BLOCK           iosb;
    FILE_STANDARD_INFORMATION info;
    ULONG                     size;

    *Buffer = NULL;
    *Length = 0;

    do {
        InitializeObjectAttributes(&oa, Path,
                                   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                   NULL, NULL);

        // 1. Open the file
        status = ZwCreateFile(&file, GENERIC_READ | SYNCHRONIZE, &oa, &iosb,
                              NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ,
                              FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT,
                              NULL, 0);
        if (!NT_SUCCESS(status)) {
            break;
        }

        // 2. Query the file length
        status = ZwQueryInformationFile(file, &iosb, &info, sizeof(info),
                                        FileStandardInformation);
        if (!NT_SUCCESS(status)) {
            break;
        }
        if (info.EndOfFile.QuadPart == 0) {
            status = STATUS_END_OF_FILE;        // our own check
            break;
        }
        if (info.EndOfFile.QuadPart > CFG_MAX_SIZE) {
            status = STATUS_FILE_TOO_LARGE;
            break;
        }
        size = (ULONG)info.EndOfFile.QuadPart;

        // 3. Allocate the buffer
        data = ExAllocatePool2(POOL_FLAG_PAGED, size, CFG_TAG);
        if (data == NULL) {
            status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        // 4. Read the contents
        status = ZwReadFile(file, NULL, NULL, NULL, &iosb, data, size,
                            NULL, NULL);
        if (!NT_SUCCESS(status)) {
            break;
        }

        // All done: hand the buffer to the caller, then forget it
        *Buffer = data;
        *Length = (ULONG)iosb.Information;
        data    = NULL;
    } while (FALSE);

    if (!NT_SUCCESS(status)) {
        KdPrint(("ReadConfigFile: %wZ failed, status 0x%08X\n", Path, status));
    }

    // One cleanup: free in the reverse order of acquiring
    if (data != NULL) {
        ExFreePoolWithTag(data, CFG_TAG);
    }
    if (file != NULL) {
        ZwClose(file);
    }
    return status;
}
