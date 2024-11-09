#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    char ConsoleTitleBuffer[80]; // указатель на буфер с заголовком
    DWORD dwBufferSize = 80;     // размер буфера для заголовка
    DWORD dwTitleSize;           // длина заголовка

    // читаем заголовок консоли
    dwTitleSize = GetConsoleTitle(ConsoleTitleBuffer, dwBufferSize);

    // выводим на консоль результат
    cout << "Title length = " << dwTitleSize << endl;
    cout << "The window title = " << ConsoleTitleBuffer << endl;

    cout << "Input new title: ";
    cin.getline(ConsoleTitleBuffer, 80);
    // устанавливаем новый заголовок консоли
    if (!SetConsoleTitle(ConsoleTitleBuffer))
        cout << "Set console title failed." << endl;

    cout << "The title was changed." << endl;
    cout << "Press any key to exit.";
    cin.get();

    return 0;
}