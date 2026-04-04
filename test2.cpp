#include <windows.h>
int main() {
    auto l = [](LPVOID param) WINAPI -> DWORD { return 0; };
    CreateThread(NULL, 0, l, NULL, 0, NULL);
    return 0;
}
