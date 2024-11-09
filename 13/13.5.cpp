#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
    DWORD dwNumber; // для количества кнопок у мыши

    // подсчитываем количество кнопок у мыши
    if (!GetNumberOfConsoleMouseButtons(&dwNumber))
    {
        cout << "Get number of console mouse buttons failed." << endl;
        return GetLastError();
    }
    // выводим количество кнопок у мыши
    cout << "Number of console mouse buttons = " << dwNumber << endl;

    _cputs("\nPress any key to finish.\n");
    _getch();

    return 0;
}