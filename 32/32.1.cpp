#include <windows.h>
#include <iostream>
using namespace std;

DWORD thread(void)
{
    char c;
    DWORD dwFirstTlsIndex, dwPrevTlsIndex, dwNextTlsIndex;

    // распределяем первый TLS-индекс
    dwFirstTlsIndex = TlsAlloc();
    if (dwFirstTlsIndex == -1)
    {
        cerr << "Tls allocation failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }
    // устанавливаем предыдущий TLS-индекс равным первому индексу
    dwPrevTlsIndex = dwFirstTlsIndex;

    // двигаемся дальше по TLS-индексам
    for (;;)
    {
        // выводим на экран предыдущий TLS-индекс
        cout << "Alloc tls index = " << dwPrevTlsIndex << endl;
        Sleep(50);
        // распределяем следующий TLS-индекс
        dwNextTlsIndex = TlsAlloc();
        // если больше индексов нет, то выходим из цикла
        if (dwNextTlsIndex == -1)
            break;
        // запомним следующий TLS-индекс в предыдущем индексе
        if (!TlsSetValue(dwPrevTlsIndex, (LPVOID)dwNextTlsIndex))
        {
            cerr << "Tls set value failed." << endl
                 << "Error code: " << GetLastError() << endl;

            cout << "Press any key to exit ";
            cin.get();
            return 0;
        }
        // продвигаем индексы
        dwPrevTlsIndex = dwNextTlsIndex;
    }

    // ждем команду на освобождение индексов
    cout << "Input any char to free tls indexes: ";
    cin >> c;

    // устанавливаем предыдущий TLS-индекс в первый
    dwPrevTlsIndex = dwFirstTlsIndex;
    // двигаемся дальше по tls индексам
    // приве

    for (;;)
    {
        // выводим на экран предыдущий TLS-индекс
        cout << "Free tls index = " << dwPrevTlsIndex << endl;
        Sleep(50);
        // получаем следующий TLS-индекс
        dwNextTlsIndex = (DWORD)TlsGetValue(dwPrevTlsIndex);
        if (!dwNextTlsIndex)
            break;
        // освобождаем предыдущий TLS-индекс
        if (!TlsFree(dwPrevTlsIndex))

        {
            cerr << "Tls free failed." << endl
                 << "Error code: " << GetLastError() << endl;

            cout << "Press any key to exit ";
            cin.get();
            return 0;
        }
        // продвигаем индексы
        dwPrevTlsIndex = dwNextTlsIndex;
    }

    // ждем команду на выход из потока
    cout << "Input any char to exit: ";
    cin >> c;

    return 0;
}

int main()
{
    HANDLE hThread;
    DWORD IDThread;

    // запускаем поток
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread,
                           NULL, 0, &IDThread);
    if (hThread == NULL)
    {
        cerr << "Create thread failed." << endl
             << "Error code: " << GetLastError() << endl;

        cout << "Press any key to exit ";
        cin.get();
        return 0;
    }
    // ждем, пока поток Add закончит работу
    WaitForSingleObject(hThread, INFINITE);
    // закрываем дескриптор потока
    CloseHandle(hThread);

    return 0;
}