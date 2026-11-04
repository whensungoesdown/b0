/* Windows 版：弹出一个对话框（在 Windows 中双击 win_msgbox.exe 运行） */
#include <windows.h>

int main(void)
{
    int answer;

    answer = MessageBoxW(NULL, L"今天学 C 语言了吗？", L"来自 Win32 的问候",
                         MB_YESNO | MB_ICONQUESTION);
    if (answer == IDYES) {
        MessageBoxW(NULL, L"真棒！", L"来自 Win32 的问候", MB_OK);
    } else {
        MessageBoxW(NULL, L"那还不快去！", L"来自 Win32 的问候", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
