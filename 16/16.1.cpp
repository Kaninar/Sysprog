#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    HANDLE hNamedPipe;

    // создаем именованный канал для чтения
    hNamedPipe = CreateNamedPipe(
        "\\\\.\\pipe\\demo_pipe",      // имя канала
        PIPE_ACCESS_INBOUND,           // читаем из канала
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений
        1,                             // максимальное количество экземпляров канала
        0,                             // размер выходного буфера по умолчанию
        0,                             // размер входного буфера по умолчанию
        INFINITE,                      // клиент ждет связь бесконечно долго
        NULL                           // защита по умолчанию
    );
    // проверяем на успешное создание
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        cerr << "Create named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }

    // ждем, пока клиент свяжется с каналом
    cout << "The server is waiting for connection with a client." << endl;
    if (!ConnectNamedPipe(
            hNamedPipe, // дескриптор канала
            NULL        // связь синхронная
            ))
    {
        cerr << "The connection failed." << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }

    // читаем данные из канала
    for (int i = 0; i < 10; i++)
    {
        int nData;
        DWORD dwBytesRead;
        if (!ReadFile(
                hNamedPipe,    // дескриптор канала
                &nData,        // адрес буфера для ввода данных
                sizeof(nData), // число читаемых байтов
                &dwBytesRead,  // число прочитанных байтов
                NULL           // передача данных синхронная
                ))
        {
            cerr << "Read file failed." << endl
                 << "The last error code: " << GetLastError() << endl;
            CloseHandle(hNamedPipe);
            cout << "Press any key to exit.";
            cin.get();

            return 0;
        }
        // выводим прочитанные данные на консоль
        cout << "The number " << nData << " was read by the server" << endl;
    }
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    // завершаем процесс
    cout << "The data are read by the server." << endl;
    cout << "Press any key to exit.";
    cin.get();

    return 0;
}