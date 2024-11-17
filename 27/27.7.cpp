#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    HANDLE hFile;     // дескриптор файла
    HANDLE hEndWrite; // дескриптор события

    OVERLAPPED ovl; // структура управления асинхронным доступом к файлу

    // создаем события с автоматическим сбросом
    hEndWrite = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEndWrite == NULL)
        return GetLastError();

    // инициализируем структуру OVERLAPPED
    ovl.Offset = 0;         // младшая часть смещения равна 0
    ovl.OffsetHigh = 0;     // старшая часть смещения равна 0
    ovl.hEvent = hEndWrite; // событие для оповещения завершения записи

    // создаем файл для записи данных
    hFile = CreateFile(
        "demo_file.dat",      // имя файла
        GENERIC_WRITE,        // запись в файл
        FILE_SHARE_WRITE,     // совместный доступ к файлу
        NULL,                 // защиты нет
        OPEN_ALWAYS,          // открываем или создаем новый файл
        FILE_FLAG_OVERLAPPED, // асинхронный доступ к файлу
        NULL                  // шаблона нет
    );
    // проверяем на успешное создание
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Create file failed." << endl
             << "The last error code: " << GetLastError() << endl;

        CloseHandle(hEndWrite);

        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }

    // пишем данные в файл
    for (int i = 0; i < 10; ++i)
    {

        DWORD dwBytesWrite;
        DWORD dwNumberOfBytesTransferred;
        DWORD dwRet;

        if (!WriteFile(
                hFile,         // дескриптор файла
                &i,            // адрес буфера, откуда идет запись
                sizeof(i),     // количество записываемых байтов
                &dwBytesWrite, // количество записанных байтов
                &ovl           // запись асинхронная
                ))
        {
            dwRet = GetLastError();
            if (dwRet == ERROR_IO_PENDING)
                cout << "Write file pending." << endl;
            else
            {
                cout << "Write file failed." << endl
                     << "The last error code: " << dwRet << endl;
                cin.get();
                return 0;
            }
        }

        // проверяем состояние асинхронной операции записи
        if (!GetOverlappedResult(
                hFile,                       // дескриптор файла
                &ovl,                        // адрес структуры для асинхронной работы
                &dwNumberOfBytesTransferred, // количество переданных байтов
                FALSE                        // не ждать завершения операции записи
                ))
        {
            cout << "Get overlapped result failed." << endl
                 << "The last error code: " << GetLastError() << endl;
            cin.get();
            return 0;
        }
        else
            cout << "Number of bytes transferred: "

                 << dwNumberOfBytesTransferred << endl;

        // ждем завершения асинхронной операции записи
        WaitForSingleObject(hEndWrite, INFINITE);
        // увеличиваем смещение в файле
        ovl.Offset += sizeof(i);
    }
    // закрываем дескрипторы
    CloseHandle(hFile);
    CloseHandle(hEndWrite);

    cout << "The file is written." << endl;
    cin.get();
    return 0;
}