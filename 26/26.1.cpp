#define _WIN32_WINNT 0x0400 // версия не ниже, чем Windows NT 4.0

#include <windows.h>
#include <iostream>
using namespace std;

HANDLE hThread; // дескриптор потока

DWORD IDThread; // идентификатор потока
DWORD dwRet;    // возвращаемое значение при установке асинхронной
                // процедуры

// процедура, которая вызывается асинхронно
void CALLBACK a_proc(DWORD p)
{
    int n;
    DWORD *ptr = (DWORD *)p;

    cout << "The asynchronous procedure is called." << endl;
    // ждем ввода целого числа
    cout << "Input integer: ";
    cin >> n;
    // увеличиваем счетчик
    *ptr += n;

    cout << "The asynchronous procedure is returned." << endl;
}

// поток, в очередь которого включается асинхронная процедура
DWORD WINAPI add(LPVOID ptr)
{
    // распечатываем начальное значение счетчика
    cout << "Initial count = " << *(DWORD *)ptr << endl;
    // ждем, пока выполнится асинхронная процедура
    SleepEx(INFINITE, TRUE);
    // распечатываем конечное значение счетчика
    cout << "Final count = " << *(DWORD *)ptr << endl;
    cin.get();
    return 0;
}

// главный поток
int main()
{
    DWORD count = 10;

    // запускаем поток
    hThread = CreateThread(NULL, 0, add, &count, 0, &IDThread);
    if (hThread == NULL)
    {
        cout << GetLastError();
        cout << "Failed to create thread";
    }

    Sleep(1000); // чтобы успел стартовать поток

    // устанавливаем асинхронную процедуру для потока
    dwRet = QueueUserAPC((PAPCFUNC)&a_proc, hThread, (ULONG_PTR)&count);
    if (!dwRet)
    {
        cout << "Queue user APC failed:" << dwRet << endl;

        return 0;
    }
    // ждем, пока поток add закончит работу
    WaitForSingleObject(hThread, INFINITE);
    // закрываем дескриптор потока
    CloseHandle(hThread);

    cout << endl
         << "Press any key to exit...";
    cin.get();
    return 0;
}