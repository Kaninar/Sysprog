#include <windows.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
    HANDLE hFindFile;
    WIN32_FIND_DATA fd;
    char szFullFileName[MAX_PATH];

    // находим первый файл
    hFindFile = FindFirstFile("dir\\*", &fd);
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
    // удаляем из каталога файлы
    while (FindNextFile(hFindFile, &fd))
    {
        // если это не подкаталог, то удаляем его
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            // формируем имя файла
            sprintf(szFullFileName, "dir/%s", fd.cFileName);
            // удаляем файл
            if (!DeleteFile(szFullFileName))
            {
                cerr << "Delete file failed." << endl
                     << "The last error code: " << GetLastError() << endl;
                cout << "Press any key to finish.";
                cin.get();

                return 0;
            }
            else
                cout << "The next file: " << fd.cFileName << " is deleted." << endl;
        }
        else
            cout << "The next directory: " << fd.cFileName << " is not deleted."
                 << endl;
    }
    // закрываем дескриптор поиска
    if (!FindClose(hFindFile))
    {
        cout << "Find close failed." << endl;
        return 0;
    }
    // удалаем каталог
    if (!RemoveDirectory("dir"))
    {
        cerr << "Remove directory failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";

        cin.get();

        return 0;
    }

    cout << "The directory is removed." << endl;

    cin.get();
    return 0;
}