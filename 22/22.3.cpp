#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
    HANDLE hHeap;
    int *a = NULL; // указатель на массив

    int size = 4096; // размер массива, а также кучи

    // создаем кучу динамически
    hHeap = HeapCreate(HEAP_NO_SERIALIZE, size, size);
    if (!hHeap)
    {
        cout << "Heap create failed." << endl;
        return GetLastError();
    }
    // пытаемся распределить память под массив
    a = (int *)HeapAlloc(hHeap, NULL, size * sizeof(int));
    // обрабатываем ошибку в случае неудачи
    if (!a)
    {
        cout << "Heap allocation failed." << endl;
        return GetLastError();
    }
    // разрушаем кучу
    if (!HeapDestroy(hHeap))
    {
        cout << "Heap destroy failed." << endl;
        return GetLastError();
    }

    cout << endl
         << "Press any button to exit" << endl;

    _getch();
    /*
        Предпологаемая ошибка:

        8 (0x8)

        Not enough memory resources are available to process this command.

        ERROR_INVALID_BLOCK

    */

    return 0;
}
