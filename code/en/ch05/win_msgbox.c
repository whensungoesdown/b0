/* Windows version: pop up a dialog box
 * (double-click win_msgbox.exe in Windows to run it) */
#include <windows.h>

int main(void)
{
    int answer;

    answer = MessageBoxW(NULL, L"Did you learn some C today?",
                         L"Greetings from Win32",
                         MB_YESNO | MB_ICONQUESTION);
    if (answer == IDYES) {
        MessageBoxW(NULL, L"Well done!", L"Greetings from Win32", MB_OK);
    } else {
        MessageBoxW(NULL, L"Then off you go!", L"Greetings from Win32",
                    MB_OK | MB_ICONWARNING);
    }
    return 0;
}
