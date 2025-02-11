// lab_prog.cpp

#include "Lab_prog_2.h"
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=120 lines=30");

	menu(); // Запуск главного меню программы
	return 0;
}