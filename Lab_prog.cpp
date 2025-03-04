// Lab_prog.cpp
#include "Lab_prog_2.h"
#include <iostream>
#include <windows.h>
#include <filesystem>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=120 lines=30");

    // Вывод заголовка программы
    cout << "*********************************************************\n";
    cout << "* Программа для обработки данных корреспонденции и адресов *\n";
    cout << "*  Описание: Программа позволяет искать и обрабатывать   *\n";
    cout << "*            информацию об организациях на основе данных *\n";
    cout << "*********************************************************\n\n";

    // Запуск главного меню программы
    menu();
    return 0;
}

//
//
// #ifdef max
// #undef max
// #endif
// 
//