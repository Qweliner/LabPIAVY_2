﻿// Lab_prog.cpp
#include "Lab_prog_2.h"
#include <iostream>
#include <windows.h>
#include <filesystem>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=120 lines=30");

    // Запуск главного меню программы
    menu();
    return 0;
}
