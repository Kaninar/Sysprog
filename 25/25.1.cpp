#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    // создаем каталог
    if (!CreateDirectory("dir", NULL))
    {
        cerr << "Create directory failed." << endl
             << "The last error code: " << GetLastError() << endl;
        cout << "Press any key to finish.";
        cin.get();

        return 0;
    }

    cout << "The directory is created." << endl;
    cin.get return 0;
}