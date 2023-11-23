#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;
int main() {
    SetConsoleOutputCP(65001);
    string s; cout << "Nhập văn bản cần spam: "; getline(cin, s);
    int n; cout << "Số lần spam: "; cin >> n;
    cout << "Bat dau sau: ";
    for(int i = 5; i >= 1; i--){
        cout << i << "s ";
        Sleep(1000);
    }
    for(int i = 1; i <= n; i++){
        for (char c : s) {
            INPUT input;
            input.type = INPUT_KEYBOARD;
            input.ki.wScan = 0;
            input.ki.time = 0;
            input.ki.dwExtraInfo = 0;
            input.ki.wVk = VkKeyScan(c);
            input.ki.dwFlags = 0;

            SendInput(1, &input, sizeof(INPUT));

            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
            

        }
        keybd_event(VK_RETURN, 0, 0, 0);
        keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0); 
    }
    return 0;
}
