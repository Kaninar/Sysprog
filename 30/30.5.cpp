#include <windows.h>
#include <fstream>
#include <iostream>
using namespace std;

int main()

{
    char file_name[] = "Demo.bin";
    int a[10];

    // открываем файл для ввода
    ifstream in(file_name, ios::in | ios::binary);
    if (!in)
    {
        cerr << "File constructor failed." << endl;
        return 0;
    }
    // вводим финальный массив из файла и выводим на консоль
    cout << "Final array: ";
    for (int i = 0; i < 10; ++i)
    {
        in.read((char *)&a[i], sizeof(int));
        cout << a[i] << ' ';
    }
    cout << endl;
    // закрываем входной файл
    in.close();

    // ждем команду на завершение процесса
    cout << "Input any char to exit: ";
    cin.get();

    return 0;
}