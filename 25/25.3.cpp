#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    HANDLE hFindFile;
    WIN32_FIND_DATA fd;

    // находим первый файл
    hFindFile = FindFirstFile("dir/*", &fd);
    if (hFindFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Find first file failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }
    // выводим на консоль имя первого файла
    cout << "The first file name: " << fd.cFileName << endl;
    // находим следующий файл и выводим на консоль его имя
    while (FindNextFile(hFindFile, &fd))
        cout << "The next file name: " << fd.cFileName << endl;
    // закрываем дескриптор поиска
    FindClose(hFindFile);
    cin.get();

    return 0;
}