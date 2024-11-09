#include <windows.h>
#include <iostream>
#include <excpt.h>
#include <stdio.h>
#include "sehmap.h"
using namespace std;

EXCEPTION_RECORD er; // информация об исключении

DWORD filter_function(EXCEPTION_POINTERS *p)
{
    // сохраняем содержимое структуры EXCEPTION_RECORD
    er = *(p->ExceptionRecord);

    return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
    int *p = NULL; // пустой указатель на целое число

    try
    {
        *p = 10; // ошибка, так как пустой указатель
    }
    catch (EXCEPTION_EXECUTE_HANDLER)
    {
        // распечатываем информацию об исключении
        cout << "ExceptionCode = " << er.ExceptionCode << endl;
        cout << "ExceptionFlags = " << er.ExceptionFlags << endl;
        cout << "ExceptionRecord = " << er.ExceptionRecord << endl;
        cout << "ExceptionAddress = " << er.ExceptionAddress << endl;
        cout << "NumberParameters = " << er.NumberParameters << endl;
        // распечатываем параметры
        if (er.ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
        {
            cout << "Type of access = " << er.ExceptionInformation[0]
                 << endl;
            cout << "Address of access = " << er.ExceptionInformation[1]
                 << endl;
        }
        cout << endl;
    }

    return 0;
}