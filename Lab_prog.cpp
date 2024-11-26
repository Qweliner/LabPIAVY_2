#include <iostream>
#include <string>
#include <windows.h>
#include "Lab_prog_2.h"

//TO DO
//
// надо будет сделать юзерфрендли, проверку дат и фамилий из предыдущей программы
//

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=120 lines=30");
    setlocale(LC_ALL, "Russian");

    std::string folderPath, correspondenceFilename, addressesFilename, outputFilename;
    bool folderPathSet = false; // Флаг, указывающий, установлен ли путь к папке


    while (true) {
        system("cls");
        std::cout << "Главное меню:\n";
        std::cout << "1. Начать работу (S) ";
        if (!folderPathSet) {
            std::cout << "(сначала укажите путь к папке)\n"; // Подсказка
        }
        else {
            std::cout << '\n';
        }

        std::cout << "2. Путь к папке (F)\n";
        std::cout << "3. Инструкция (I)\n";
        std::cout << "Esc - Выход\n";
        std::cout << "Ваш выбор: ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': case 's': case 'S':
            if (!folderPathSet) {
                std::cerr << "Ошибка: сначала укажите путь к папке.\n";
                std::cout << "Нажмите любую клавишу для продолжения...";
                _getch();

            }
            else {
                runProgram(folderPath, correspondenceFilename, addressesFilename, outputFilename);
            }

            break;
        case '2': case 'f': case 'F':
            std::cout << "Введите путь к папке: ";
            std::cin >> folderPath;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера

            // Проверка и добавление обратной косой черты
            if (folderPath.back() != '\\' && folderPath.back() != '/') {
                folderPath += '\\';
            }

            folderPathSet = true; // Путь установлен
            std::cout << "\nТекущий путь к папке: " << folderPath << std::endl; // Вывод текущего пути
            std::cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '3': case 'i': case 'I':
            printInstructions();
            break;
        case 27: // Esc
            return 0;
        default:
            std::cerr << "Неверный выбор.\n";
        }
    }
    return 0;
}