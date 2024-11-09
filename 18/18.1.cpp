#include <windows.h>
#include <iostream>
using namespace std;

int filterException(int code, PEXCEPTION_POINTERS ex)
{
    std::cout << "Filtering " << std::hex << code << std::endl;
    return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
    int a = 10;
    int *p = NULL; // пустой указатель на целое число

    __try
    {
        cout << "a = " << *p << endl; // ошибка, так как p = NULL
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        cout << "There was some exception." << endl;
        p = &a;
    }

    cout << "a = " << *p << endl; // нормально

    return 0;
}