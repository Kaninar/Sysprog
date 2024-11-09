#include <windows.h>
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
    char machineName[80];
    char pipeName[80];
    HANDLE hNamedPipe;

    DWORD dwFlags = PIPE_CLIENT_END |
                    PIPE_TYPE_MESSAGE; // клиент канала и передача данных сообщениями
    DWORD dwOutBufferSize;             // состояние канала
    DWORD dwInBufferSize;              // количество экземпляров канала
    DWORD dwMaxInstances;              // размер буфера клиента канала

    // вводим имя машины в сети, на которой работает сервер
    cout << "Enter a name of the server machine: ";
    cin >> machineName;
    cin.get();
    // подставляем имя машины в имя канала
    wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe", machineName);

    // связываемся с именованным каналом
    hNamedPipe = CreateFile(
        pipeName,                           // имя канала
        GENERIC_READ | GENERIC_WRITE,       // читаем и записываем в канал
        FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись
        NULL,                               // безопасность по умолчанию
        OPEN_EXISTING,                      // открываем существующий канал
        FILE_ATTRIBUTE_NORMAL,              // атрибуты по умолчанию
        NULL);                              // дополнительных атрибутов нет

    // проверяем связь с каналом
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {

        cerr
            << "Connection with the named pipe failed." << endl
            << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }

    // получаем информацию о канале
    if (!GetNamedPipeInfo(
            hNamedPipe,       // дескриптор именованного канала
            &dwFlags,         // тип канала
            &dwOutBufferSize, // размер выходного буфера
            &dwInBufferSize,  // размер входного буфера
            &dwMaxInstances)) // максимальное количество экземпляров канала
    {
        cerr << "Get named pipe info failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }

    // выводим информацию на консоль
    cout << "Out buffer size: " << dwOutBufferSize << endl
         << "In buffer size: " << dwInBufferSize << endl
         << "Max instances: " << dwMaxInstances << endl;

    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    // завершаем процесс
    cout << "Press any key to exit.";
    cin.get();

    return 0;
}