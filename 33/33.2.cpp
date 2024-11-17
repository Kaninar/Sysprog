#include <windows.h>
#include <iostream>
using namespace std;

char[] serviceName = "kaninarService";

SERVICE_STATUS serviceStatus;

SERVICE_STATUS_HANDLE serviceStatusHandle;

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

    // error = InitService();

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

int _tmain(int argc, char *argv[])
{

    SERVICE_TABLE_ENTRY ServiceTable[1];

    ServiceTable[0].lpServiceName = serviceName;

    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    StartServiceCtrlDispatcher(ServiceTable);
}