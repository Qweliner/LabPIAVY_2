#include <iostream>
#include <string>
#include <windows.h>
#include "Lab_prog_2.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=120 lines=30");
    setlocale(LC_ALL, "Russian");

    string folderPath, correspondenceFilename, addressesFilename, outputFilename;
    bool folderPathSet = false;

    while (true) {
        system("cls");
        cout << "Главное меню:\n";
        cout << "1. Начать работу (S) ";
        if (!folderPathSet) {
            cout << "(сначала укажите путь к папке)\n";
        }
        else {
            cout << '\n';
        }

        cout << "2. Путь к папке (F)\n";
        cout << "3. Инструкция (I)\n";
        cout << "Esc - Выход\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': case 's': case 'S':
            if (!folderPathSet) {
                cerr << "Ошибка: сначала укажите путь к папке.\n";
                cout << "Нажмите любую клавишу для продолжения...";
                _getch();
            }
            else {
                runProgram(folderPath, correspondenceFilename, addressesFilename, outputFilename);
            }
            break;
        case '2': case 'f': case 'F':
            cout << "Введите путь к папке: ";
            cin >> folderPath;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (folderPath.back() != '\\' && folderPath.back() != '/') {
                folderPath += '\\';
            }

            folderPathSet = true;
            cout << "\nТекущий путь к папке: " << folderPath << endl;
            cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        case '3': case 'i': case 'I':
            printInstructions();
            break;
        case 27:
            return 0;
        default:
            cerr << "Неверный выбор.\n";
        }
    }
    return 0;
}