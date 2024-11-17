#include <windows.h>
#include <iostream>
using namespace std;

LPTSTR serviceName = "kaninarService";
LPTSTR servicePath = "C:/users/user/desktop/vsc/sysprog_listings/33/bin/33.3.exe";

SERVICE_STATUS serviceStatus;

SERVICE_STATUS_HANDLE serviceStatusHandle;

int addLogMessage(const char *text)
{
    return printf(text);
}

void ControlHandler(DWORD request)
{

    switch (request)

    {

    case SERVICE_CONTROL_STOP:

        addLogMessage("Stopped.");

        serviceStatus.dwWin32ExitCode = 0;

        serviceStatus.dwCurrentState = SERVICE_STOPPED;

        SetServiceStatus(serviceStatusHandle, &serviceStatus);

        return;

    case SERVICE_CONTROL_SHUTDOWN:

        addLogMessage("Shutdown.");

        serviceStatus.dwWin32ExitCode = 0;

        serviceStatus.dwCurrentState = SERVICE_STOPPED;

        SetServiceStatus(serviceStatusHandle, &serviceStatus);

        return;

    default:

        break;
    }

    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    return;
}

int InstallService()
{

    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (!hSCManager)
    {

        cout << "Error: Can't open Service Control Manager";

        return -1;
    }

    SC_HANDLE hService = CreateService(

        hSCManager,

        serviceName,

        serviceName,

        SERVICE_ALL_ACCESS,

        SERVICE_WIN32_OWN_PROCESS,

        SERVICE_DEMAND_START,

        SERVICE_ERROR_NORMAL,

        servicePath,

        NULL, NULL, NULL, NULL, NULL

    );

    if (!hService)
    {

        int err = GetLastError();

        switch (err)
        {

        case ERROR_ACCESS_DENIED:

            cout << ("Error: ERROR_ACCESS_DENIED");

            break;

        case ERROR_CIRCULAR_DEPENDENCY:

            cout << ("Error: ERROR_CIRCULAR_DEPENDENCY");

            break;

        case ERROR_DUPLICATE_SERVICE_NAME:

            cout << ("Error: ERROR_DUPLICATE_SERVICE_NAME");

            break;

        case ERROR_INVALID_HANDLE:

            cout << ("Error: ERROR_INVALID_HANDLE");

            break;

        case ERROR_INVALID_NAME:

            cout << ("Error: ERROR_INVALID_NAME");

            break;

        case ERROR_INVALID_PARAMETER:

            cout << ("Error: ERROR_INVALID_PARAMETER");

            break;

        case ERROR_INVALID_SERVICE_ACCOUNT:

            cout << ("Error: ERROR_INVALID_SERVICE_ACCOUNT");

            break;

        case ERROR_SERVICE_EXISTS:

            cout << ("Error: ERROR_SERVICE_EXISTS");

            break;

        default:

            cout << ("Error: Undefined");
        }

        CloseServiceHandle(hSCManager);

        return -1;
    }

    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);

    cout << "Success install service!";

    return 0;
}

int RemoveService()
{

    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!hSCManager)
    {

        cout << ("Error: Can't open Service Control Manager");

        return -1;
    }

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP | DELETE);

    if (!hService)
    {

        cout << ("Error: Can't remove service");

        CloseServiceHandle(hSCManager);

        return -1;
    }

    DeleteService(hService);

    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);

    cout << ("Success remove service!");

    return 0;
}

int StartService()
{

    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);

    if (!StartService(hService, 0, NULL))
    {

        CloseServiceHandle(hSCManager);

        cout << "Error: Can't start service";

        return -1;
    }

    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);

    return 0;
}

void ServiceMain(int argc, char **argv)
{

    int error;

    int i = 0;

    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

    serviceStatus.dwCurrentState = SERVICE_START_PENDING;

    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

    serviceStatus.dwWin32ExitCode = 0;

    serviceStatus.dwServiceSpecificExitCode = 0;

    serviceStatus.dwCheckPoint = 0;

    serviceStatus.dwWaitHint = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler);

    if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
    {

        return;
    }

    error = GetLastError();

    if (error)
    {

        serviceStatus.dwCurrentState = SERVICE_STOPPED;

        serviceStatus.dwWin32ExitCode = -1;

        SetServiceStatus(serviceStatusHandle, &serviceStatus);

        return;
    }

    serviceStatus.dwCurrentState = SERVICE_RUNNING;

    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    while (serviceStatus.dwCurrentState == SERVICE_RUNNING)

    {

        char buffer[255];

        sprintf_s(buffer, "%u", i);

        int result = addLogMessage(buffer);

        if (result)
        {

            serviceStatus.dwCurrentState = SERVICE_STOPPED;

            serviceStatus.dwWin32ExitCode = -1;

            SetServiceStatus(serviceStatusHandle, &serviceStatus);

            return;
        }

        i++;
    }

    return;
}

int _tmain(int argc, TCHAR *argv[])
{

    servicePath = LPTSTR(argv[0]);

    if (argc - 1 == 0)
    {

        SERVICE_TABLE_ENTRY ServiceTable[1];

        ServiceTable[0].lpServiceName = serviceName;

        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

        if (!StartServiceCtrlDispatcher(ServiceTable))
        {

            cout << "Error: StartServiceCtrlDispatcher";
        }
    }
    else if (strcmp(argv[argc - 1], TEXT("install")) == 0)
    {

        InstallService();
    }
    else if (strcmp(argv[argc - 1], TEXT("remove")) == 0)
    {

        RemoveService();
    }
    else if (strcmp(argv[argc - 1], TEXT("start")) == 0)
    {

        StartService();
    }
}