#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    // удаляем файл
    if (!DeleteFile("C:\\demo_file.dat"))
    {
        cerr << "Delete file failed." << endl
             << "The last error code: " << GetLastError() << endl;
    }

    cout << "The file is deleted." << endl;
    cout << "Press any key to finish.";

    cin.get();

    return 0;
}