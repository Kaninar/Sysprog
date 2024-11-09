#include <windows.h>
#include <iostream>
#include <sstream>
#include <timezoneapi.h>
#include <string>
using namespace std;

std::string SystemTimeToString(FILETIME ft)
{
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    ostringstream os;
    os << st.wDay << ":" << st.wMonth << ":" << st.wYear << "  "
       << st.wHour + 5 << ":" << st.wMinute << ":" << st.wSecond;
    return os.str();
}

int main()
{

    HANDLE hFile;
    BY_HANDLE_FILE_INFORMATION bhfi; // информация о файле
    // открываем файл для чтения
    hFile = CreateFile(
        "file.dat",            // имя файла
        0,                     // получение информации о файле
        0,                     // монопольный доступ к файлу
        NULL,                  // защиты нет
        OPEN_EXISTING,         // открываем существующий файл
        FILE_ATTRIBUTE_NORMAL, // обычный файл
        NULL                   // шаблона нет
    );
    // проверяем на успешное открытие
    if (hFile == INVALID_HANDLE_VALUE)
    {
        cerr << "Create file failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }
    // получаем информацию о файле
    if (!GetFileInformationByHandle(hFile, &bhfi))
    {
        cerr << "Get file information by handle failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }
    // распечатываем информацию о файле
    cout
        << "File attributes: " << bhfi.dwFileAttributes << endl
        << "Creation date: " << SystemTimeToString(bhfi.ftCreationTime) << endl
        << "Last access time: " << SystemTimeToString(bhfi.ftLastAccessTime) << endl
        << "Last write time: " << SystemTimeToString(bhfi.ftLastWriteTime) << endl
        << "Volume serial number: " << bhfi.dwVolumeSerialNumber << endl
        << "File size high: " << bhfi.nFileSizeHigh << endl
        << "File size low: " << bhfi.nFileSizeLow << endl
        << "Number of links: " << bhfi.nNumberOfLinks << endl
        << "File index high: " << bhfi.nFileIndexHigh << endl
        << "File index low: " << bhfi.nFileIndexLow << endl;

    // закрываем дескриптор файла
    CloseHandle(hFile);

    cin.get();

    return 0;
}