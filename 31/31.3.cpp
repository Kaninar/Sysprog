#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    HMODULE hDll;    // дескриптор DLL
    int (*Add)(int); // указатель на функцию Add из DLL
    int (*Sub)(int); // указатель на функцию Sub из DLL
    int *pCount;     // указатель на переменную count из DLL

    // загружаем динамически подключаемую библиотеку
    hDll = LoadLibrary("dll/31.1.dll");
    if (!hDll)
    {
        cerr << "Load library failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }

    // настраиваем адрес переменной count из DLL
    pCount = (int *)GetProcAddress(hDll, MAKEINTRESOURCE(3));
    if (!pCount)
    {
        cerr << "Get variable address failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }
    // выводим начальное значение переменной count
    cout << "Initial count = " << (*pCount) << endl;
    // настраиваем адреса функций
    Add = (int (*)(int))GetProcAddress(hDll, MAKEINTRESOURCE(1));
    Sub = (int (*)(int))GetProcAddress(hDll, MAKEINTRESOURCE(2));
    if (!Add || !Sub)
    {
        cerr << "Get procedure address failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }
    // изменяем значение счетчика
    cout << "count = " << Add(20) << endl;
    cout << "count = " << Sub(15) << endl;
    // отключаем библиотеку
    if (!FreeLibrary(hDll))
    {
        cerr << "Free library failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }
    // выходим из программы
    cout << "Press any key to exit";
    cin.get();

    return 0;
}