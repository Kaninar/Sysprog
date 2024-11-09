#include <windows.h>
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
    char machineName[80];
    char pipeName[80];
    HANDLE hNamedPipe;
    DWORD dwBytesRead;     // для количества прочитанных байтов
    char pchInBuffer[80];  // для записи сообщения
    char pchOutBuffer[80]; // для чтения сообщения
    int nMessageLength;    // длина сообщения

    // вводим имя машины в сети, на которой работает сервер
    cout << "Enter a name of the server machine: ";
    cin >> machineName;
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
        cerr << "Connection with the named pipe failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }

    // вводим строку
    cin.get();

    cout << "Input a string: ";
    cin.getline(pchInBuffer, 80);
    // определяем длину строки
    nMessageLength = strlen(pchInBuffer) + 1;

    // пишем и читаем из именованного канала одной транзакцией
    if (!TransactNamedPipe(
            hNamedPipe,           // дескриптор канала
            &pchInBuffer,         // адрес входного буфера канала
            nMessageLength,       // длина входного сообщения
            &pchOutBuffer,        // адрес выходного буфера канала
            sizeof(pchOutBuffer), // длина выходного буфера канала
            &dwBytesRead,         // количество прочитанных байтов
            NULL))                // передача транзакции синхронная
    {
        // ошибка транзакции
        cerr << "Transact named pipe failed: " << endl
             << "The last error code: " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "Press any key to exit.";
        cin.get();

        return 0;
    }
    // выводим посланное сообщение на консоль
    cout << "The sent message: "
         << endl
         << '\t' << pchInBuffer << endl;
    // выводим полученное сообщение на консоль
    cout << "The received message: "
         << endl
         << '\t' << pchOutBuffer << endl;
    // закрываем дескриптор канала
    CloseHandle(hNamedPipe);
    // завершаем процесс
    cout << "Press any key to exit.";
    cin.get();

    return 0;
}