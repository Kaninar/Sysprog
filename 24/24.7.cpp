#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    // перемещаем файл
    if (!ReplaceFile(
            "C:\\new_file.dat",        // имя замещаемого файла
            "C:\\demo_file.dat",       // имя файла-заместителя
            "C:\\back_file.dat",       // имя резервного файла
            REPLACEFILE_WRITE_THROUGH, // освободить буферы
            NULL, NULL                 // не используются
            ))
    {
        cerr << "Replace file failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();
        return 0;
    }

    cout
        << "The file is replaced." << endl;

    return 0;
}