#include "Prog_1_header.h"
#include <windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=120 lines=30");

    menu();

    return 0;
}