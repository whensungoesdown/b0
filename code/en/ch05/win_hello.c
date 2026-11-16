/* Windows version: print a line with the Win32 API */
#include <windows.h>

int main(void)
{
    /* get the handle of "standard output" */
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    const char msg[] = "Hello, Win32!\r\n";   /* newline: \r\n */
    DWORD written;                            /* bytes written */

    WriteFile(out, msg, sizeof(msg) - 1, &written, NULL);
    return 0;
}
