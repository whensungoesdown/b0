/* 写法：do { } while (FALSE)（Windows 内核驱动，需要 WDK 编译） */
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

        // 1. 打开文件
        status = ZwCreateFile(&file, GENERIC_READ | SYNCHRONIZE, &oa, &iosb,
                              NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ,
                              FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT,
                              NULL, 0);
        if (!NT_SUCCESS(status)) {
            break;
        }

        // 2. 查询文件长度
        status = ZwQueryInformationFile(file, &iosb, &info, sizeof(info),
                                        FileStandardInformation);
        if (!NT_SUCCESS(status)) {
            break;
        }
        if (info.EndOfFile.QuadPart == 0) {
            status = STATUS_END_OF_FILE;        // 自己判断的错误，自己设置状态
            break;
        }
        if (info.EndOfFile.QuadPart > CFG_MAX_SIZE) {
            status = STATUS_FILE_TOO_LARGE;
            break;
        }
        size = (ULONG)info.EndOfFile.QuadPart;

        // 3. 分配缓冲区
        data = ExAllocatePool2(POOL_FLAG_PAGED, size, CFG_TAG);
        if (data == NULL) {
            status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        // 4. 读取文件内容
        status = ZwReadFile(file, NULL, NULL, NULL, &iosb, data, size,
                            NULL, NULL);
        if (!NT_SUCCESS(status)) {
            break;
        }

        // 全部成功：把缓冲区交给调用者，然后置空
        *Buffer = data;
        *Length = (ULONG)iosb.Information;
        data    = NULL;
    } while (FALSE);

    if (!NT_SUCCESS(status)) {
        KdPrint(("ReadConfigFile: %wZ failed, status 0x%08X\n", Path, status));
    }

    // 统一清理：按与获取相反的顺序释放
    if (data != NULL) {
        ExFreePoolWithTag(data, CFG_TAG);
    }
    if (file != NULL) {
        ZwClose(file);
    }
    return status;
}
