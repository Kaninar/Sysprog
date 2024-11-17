#include <windows.h>
#include <iostream>
#include "res.h"
using namespace std;

int main()
{
    char service_name[] = SERVICE_NAME; // имя сервиса
    SC_HANDLE hServiceControlManager, hService;

    // указатель на буфер для информации о конфигурации сервиса
    QUERY_SERVICE_CONFIG *lpQueryServiceConfig;
    // переменная для размера структуры в случае неудачи функции
    DWORD dwBytesNeeded;

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

    // открываем сервис
    hService = OpenService(
        hServiceControlManager, // дескриптор менеджера сервисов
        service_name,           // имя сервиса
        SERVICE_ALL_ACCESS      // любой доступ к сервису
    );
    if (hService == NULL)
    {
        cout << "Open service failed." << endl
             << "The last error code: " << GetLastError() << endl
             << "Press any key to exit." << endl;
        cin.get();

        // закрываем дескриптор менеджера сервисов
        CloseServiceHandle(hServiceControlManager);

        return 0;
    }

    cout << "Service is opened." << endl
         << "Press any key to continue." << endl;
    cin.get();

    // захватываем память под буфер
    lpQueryServiceConfig = (LPQUERY_SERVICE_CONFIG) new char[4096];

    // определям конфигурацию сервиса
    QueryServiceConfig(hService, lpQueryServiceConfig, 4096,
                       &dwBytesNeeded);
    // распечатываем информацию о режиме запуска сервиса

    cout
        << "Old start type: " << lpQueryServiceConfig->dwStartType
        << endl;

    // изменяем режим запуска сервиса
    if (!ChangeServiceConfig(
            hService,                                // дескриптор сервиса
            SERVICE_NO_CHANGE,                       // тип сервиса не изменяем
            SERVICE_AUTO_START,                      // запуск во время загрузки системы
            SERVICE_NO_CHANGE,                       // режим обработки ошибок не изменяем
            NULL, NULL, NULL, NULL, NULL, NULL, NULL // все остальные параметры
                                                     // конфигурации не изменяем
            ))
    {
        cout << "Change service configuration failed." << endl
             << "The last error code: " << GetLastError() << endl
             << "Press any key to exit." << endl;
        cin.get();

        // закрываем дескрипторы
        CloseServiceHandle(hServiceControlManager);
        CloseServiceHandle(hService);

        return 0;
    }

    // определям конфигурацию сервиса
    QueryServiceConfig(hService, lpQueryServiceConfig, 4096,
                       &dwBytesNeeded);
    // распечатываем информацию о режиме запуска сервиса
    cout << "New start type: " << lpQueryServiceConfig->dwStartType
         << endl;

    // восстанавливаем режим запуска сервиса
    ChangeServiceConfig(
        hService,                                // дескриптор сервиса
        SERVICE_NO_CHANGE,                       // тип сервиса не изменяем
        SERVICE_DEMAND_START,                    // запуск по требованию
        SERVICE_NO_CHANGE,                       // режим обработки ошибок не изменяем
        NULL, NULL, NULL, NULL, NULL, NULL, NULL // все остальные параметры

        // конфигурации не изменяем
    );
    // определям конфигурацию сервиса
    QueryServiceConfig(hService, lpQueryServiceConfig, 4096,
                       &dwBytesNeeded);
    // распечатываем информацию о режиме запуска сервиса
    cout << "Old start type: " << lpQueryServiceConfig->dwStartType
         << endl;

    // освобождаем память
    delete[] lpQueryServiceConfig;

    cout << "Press any key to exit." << endl;
    cin.get();

    // закрываем дескрипторы
    CloseServiceHandle(hServiceControlManager);
    CloseServiceHandle(hService);

    return 0;
}