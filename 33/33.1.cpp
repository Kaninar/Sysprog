#include <windows.h>
#include <fstream>
#include <iostream>
using namespace std;

char service_name[] = "DemoService"; // имя сервиса

SERVICE_STATUS service_status;
SERVICE_STATUS_HANDLE hServiceStatus;

// ofstream cout; // выходной файл для протокола работы сервиса
int nCount; // счетчик

VOID WINAPI ServiceCtrlHandler(DWORD dwControl)
{
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP: // остановить сервис
        // записываем конечное значение счетчика
        cout << "Count = " << nCount << endl;
        cout << "The service is finished." << endl
             << flush;
        // закрываем файл

        // устанавливаем состояние остановки
        service_status.dwCurrentState = SERVICE_STOPPED;
        // изменить состояние сервиса
        SetServiceStatus(hServiceStatus, &service_status);
        break;

    case SERVICE_CONTROL_SHUTDOWN: // завершить сервис
        service_status.dwCurrentState = SERVICE_STOPPED;
        // изменить состояние сервиса
        SetServiceStatus(hServiceStatus, &service_status);
        break;

    default:

        // увеличиваем значение контрольной точки
        ++service_status.dwCheckPoint;
        // оставляем состояние сервиса без изменения
        SetServiceStatus(hServiceStatus, &service_status);
        break;
    }

    return;
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    // регистрируем обработчик управляющих команд для сервиса
    hServiceStatus = RegisterServiceCtrlHandler(
        service_name,      // имя сервиса
        ServiceCtrlHandler // обработчик управляющих команд
    );
    if (!hServiceStatus)
    {
        cout << "Register service control handler failed.";

        return;
    }

    // инициализируем структуру состояния сервиса
    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwCurrentState = SERVICE_START_PENDING;
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP |

                                        SERVICE_ACCEPT_SHUTDOWN;
    service_status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
    service_status.dwServiceSpecificExitCode = 0;
    service_status.dwCheckPoint = 0;
    service_status.dwWaitHint = 5000;

    // устанавливаем состояние сервиса
    if (!SetServiceStatus(hServiceStatus, &service_status))
    {
        cout << "Set service status 'SERVICE_START_PENDING' failed.";

        return;
    }

    // определяем сервис как работающий
    service_status.dwCurrentState = SERVICE_RUNNING;
    // нет ошибок
    service_status.dwWin32ExitCode = NO_ERROR;
    // устанавливаем новое состояние сервиса
    if (!SetServiceStatus(hServiceStatus, &service_status))
    {
        cout << "Set service status 'START_PENDING' failed.";
        return;
    }

    // открываем файл протокола работы сервиса
    cout << "The service is started." << endl
         << flush;
    cout << "My name is: " << lpszArgv[0] << endl
         << flush;
    for (int i = 1; i < (int)dwArgc; ++i)
        cout << "My " << i << " parameter =  " << lpszArgv[i] << endl
             << flush;

    // рабочий цикл сервиса
    while (service_status.dwCurrentState == SERVICE_RUNNING)

    {
        ++nCount;
        Sleep(3000);
    }
}

// главная функция приложения
int main()
{
    //  инициализируем структуру сервисов
    SERVICE_TABLE_ENTRY service_table[] =
        {
            {(LPSTR)service_name, (LPSERVICE_MAIN_FUNCTION)ServiceMain}, // имя сервиса и функция сервиса
            {NULL, NULL}                                                 // больше сервисов нет
        };

    // запускаем диспетчер сервиса
    if (!StartServiceCtrlDispatcher(service_table))
    {
        cout << "Start service control dispatcher failed." << endl;
        cout << GetLastError() << endl;
        cin.get();

        return 0;
    }
    cin.get();
    return 0;
}