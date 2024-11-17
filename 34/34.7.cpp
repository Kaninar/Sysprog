#include <windows.h>
#include <iostream>
#include "res.h"
using namespace std;

int main()
{
    char service_name[] = SERVICE_NAME; // имя сервиса
    SC_HANDLE hServiceControlManager;   // дескриптор базы данных сервисов

    // указатель на буфер для внутренненего имени сервиса
    char *lpszServiceName;
    // переменная для длины буфера в случае неудачи функции
    DWORD dwBufferSize;

    // связываемся с менеджером сервисов
    hServiceControlManager = OpenSCManager(
        NULL,              // локальная машина
        NULL,              // активная база данных сервисов
        SC_MANAGER_CONNECT // соединение с менеджером сервисов
    );
    if (hServiceControlManager == NULL)
    {
        cout << "Open service control manager failed." << endl
             << "The last error code: " << GetLastError() << endl
             << "Press any key to continue." << endl;
        cin.get();

        return 0;
    }

    cout << "Service control manager is opened." << endl
         << "Press any key to continue." << endl;
    cin.get();

    // захватываем память под буфер
    dwBufferSize = 256;
    lpszServiceName = (char *)new char[dwBufferSize];

    // определение внешнего имени сервиса
    if (!GetServiceDisplayName(
            hServiceControlManager, // дескриптор базы данных сервисов
            service_name,           // внутреннее имя сервиса
            lpszServiceName,        // буфер для имени сервиса
            &dwBufferSize           // количество необходимых байтов
            ))
    {
        cout << "Get service display name failed." << endl
             << "The last error code: " << GetLastError() << endl
             << "Press any key to exit." << endl;
        cin.get();

        // закрываем дескриптор базы данных сервисов
        CloseServiceHandle(hServiceControlManager);

        return 0;
    }

    // выводим на консоль внутреннее имя сервиса
    cout << "Service display name: " << lpszServiceName << endl;

    cout << "Press any key to exit." << endl;
    cin.get();

    // закрываем дескриптор базы данных сервисов
    CloseServiceHandle(hServiceControlManager);

    return 0;
}