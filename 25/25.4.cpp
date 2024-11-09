#include <windows.h>
#include <iostream>
using namespace std;

int main()

{
    // удалаем каталог
    if (!RemoveDirectory("subdir"))
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