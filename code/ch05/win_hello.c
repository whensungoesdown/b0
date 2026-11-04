/* Windows 版：用 Win32 API 打印一行字 */
#include <windows.h>

int main(void)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);   /* 拿到“标准输出”的句柄 */
    const char msg[] = "Hello, Win32!\r\n";         /* Windows 的换行是 \r\n */
    DWORD written;                                  /* 实际写了几个字节 */

    WriteFile(out, msg, sizeof(msg) - 1, &written, NULL);
    return 0;
}
