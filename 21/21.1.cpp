#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main()

{
    // int a[1000];
    int *a;              // указатель на массив целых чисел
    const int size = 10; // размерность массива
    try
    {

        // распределяем виртуальную память
        a = (int *)VirtualAlloc(
            NULL,
            size * sizeof(int),
            MEM_COMMIT,
            PAGE_READWRITE);
        FillMemory(a, size * sizeof(int), 0);

        if (!a)
        {
            cout << "Virtual allocation failed." << endl;
            throw GetLastError();
        }

        for (u_char i = 0; i < size; i++)
        {
            cout << a[i] << endl;
            a[i] = i + 2;
        }
    }
    catch (DWORD e)
    {
        cerr << e << endl;
    }

    if (!VirtualFree(a, 0, MEM_RELEASE))
    {
        cout << "Memory release failed." << endl;
        return GetLastError();
    }
    _getch();

    return 0;
}