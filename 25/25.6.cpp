#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    // перемещаем каталог

    if (!MoveFile("dir", "dir"))
    {
        cerr << "Move file failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }

    cout << "The directory is moved." << endl;
    cin.get();
    return 0;
}