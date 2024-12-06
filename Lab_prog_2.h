#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <locale.h>
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <limits>

#ifdef max
#undef max
#endif

using namespace std;

// Структуры
struct Correspondence {
    string type;
    string date;
    string organization;
};

struct Address {
    string organization;
    string address;
    string contactPerson;
};

void tabul(int x) {
    for (int i = x; i != 0; i--) printf("   ");
}

bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool isValidDate(const char* dateStr) {
    if (strlen(dateStr) != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr, "%2d.%2d.%4d", &day, &month, &year);

    if (count != 3) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (day < 1 || day > 31) {
        return false;
    }
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > daysInFeb) {
            return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }

    return true;
}

bool isValidFileName(const std::string& fileName) {
    // Запрещенные символы в Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

void printInstructions() {
    system("cls");
    cout << "\nИнструкция по работе с программой:\n";
    cout << "1. Путь к папке: Укажите полный путь к папке, содержащей файлы с корреспонденцией и адресами.\n";
    cout << "   Пример: C:\\Users\\YourName\\Documents\\Correspondence\n";
    cout << "2. Название файлов: Введите названия файлов БЕЗ расширения .txt.\n";
    cout << "   Программа автоматически добавит префиксы (IC_ и AO_) и расширение.\n";
    cout << "3. Избирательный вывод: При выборе этого режима введите название организации для поиска.\n";
    cout << "4. Полный вывод: При выборе этого режима данные будут записаны в файл output.txt в указанной папке.\n";
    cout << "5. Ошибки:\n";
    cout << "   - Неверный путь к папке: Программа сообщит об ошибке.\n";
    cout << "   - Неверный формат файлов: Программа пропустит строки с некорректным форматом данных и выведет предупреждение.\n";
    cout << "   - Организация не найдена: Программа сообщит, если не найдет информацию о запрошенной организации.\n";
    cout << "6. Навигация по меню: Используйте клавиши 1, 2, 3, I, S, O или Esc для навигации.\n";
    cout << "   Esc - выход из программы или возврат в предыдущее меню.\n";
    cout << "\nНажмите любую клавишу для продолжения...";
    _getch();
}

vector<Correspondence> readCorrespondence(const string& filename) {
    vector<Correspondence> correspondence;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        getline(file, line); // Пропустить заголовок

        while (getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                Correspondence entry;
                entry.type = line.substr(0, pos1);
                entry.date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.organization = line.substr(pos2 + 1);
                correspondence.push_back(entry);
            }
            else {
                cerr << "Ошибка формата файла корреспонденции в строке: " << line << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл корреспонденции: " << filename << endl;
    }
    return correspondence;
}

vector<Address> readAddresses(const string& filename) {
    vector<Address> addresses;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        getline(file, line); // Пропустить заголовок

        while (getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                Address entry;
                entry.organization = line.substr(0, pos1);
                entry.address = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.contactPerson = line.substr(pos2 + 1);
                addresses.push_back(entry);
            }
            else {
                cerr << "Ошибка формата файла адресов в строке: " << line << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл адресов: " << filename << endl;
    }
    return addresses;
}

void printSelectiveInfo(const vector<Correspondence>& correspondence, const vector<Address>& addresses) {
    string orgName;
    cout << "Введите название организации для поиска: ";
    cin >> orgName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (const auto& corr : correspondence) {
        if (corr.organization == orgName) {
            for (const auto& addr : addresses) {
                if (addr.organization == orgName) {
                    cout << "Тип корреспонденции: " << corr.type << endl;
                    cout << "Дата: " << corr.date << endl;
                    cout << "Адрес: " << addr.address << endl;
                    cout << "Контактное лицо: " << addr.contactPerson << endl;
                    cout << "--------------------" << endl;
                    return;
                }
            }
            cerr << "Адрес для организации " << orgName << " не найден." << endl;
            return;
        }
    }
    cerr << "Корреспонденция для организации " << orgName << " не найдена." << endl;
}

void printAllInfoToFile(const vector<Correspondence>& correspondence, const vector<Address>& addresses, const string& filename) {
    ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "Не удалось открыть файл для записи: " << filename << endl;
        return;
    }

    map<string, vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        outfile << "Организация: " << addr.organization << endl;
        outfile << "Адрес: " << addr.address << endl;
        outfile << "Контактное лицо: " << addr.contactPerson << endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                outfile << "\tТип корреспонденции: " << corr.type << endl;
                outfile << "\tДата: " << corr.date << endl;
            }
        }
        else {
            outfile << "\tКорреспонденция не найдена." << endl;
        }
        outfile << "--------------------" << endl;
    }

    outfile.close();
    cout << "Информация успешно записана в файл: " << filename << endl;
}


pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    cout << "Введите имя файла с исходящей корреспонденцией (без расширения): ";
    cin >> correspondenceFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(correspondenceFilename)) {
        tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
        //break;
    }

    cout << "Введите имя файла с адресами организаций (без расширения): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");
        //break;
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt", folderPath + "AO_" + addressesFilename + ".txt" };

}



void runProgram(string& folderPath, string& correspondenceFilename, string& addressesFilename, string& outputFilename) {


    tie(correspondenceFilename, addressesFilename) = getFilenamesFromUser(folderPath);
    if (folderPath.back() != '\\' && folderPath.back() != '/') {
        folderPath += '\\';
    }

    outputFilename = folderPath + "output.txt";
    vector<Correspondence> correspondence = readCorrespondence(correspondenceFilename);
    vector<Address> addresses = readAddresses(addressesFilename);

    if (correspondence.empty() || addresses.empty()) {
        cerr << "Ошибка чтения данных. Проверьте файлы и повторите попытку.\n";
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();

        return;
    }


    char choice;
    cout << "\nВыберите режим вывода:\n";
    cout << "1. Избирательный вывод на экран (I)\n";
    cout << "2. Полный вывод в файл (O)\n";
    cout << "Esc - Вернуться в главное меню\n";
    cout << "Ваш выбор: ";

    choice = _getch();

    switch (choice) {
    case '1': case 'i': case 'I':
        printSelectiveInfo(correspondence, addresses);
        break;
    case '2': case 'o': case 'O':
        printAllInfoToFile(correspondence, addresses, outputFilename);
        break;
    case 27: // Esc
        return;
    default:
        cerr << "Неверный выбор.\n";
    }
    cout << "Нажмите любую клавишу для возврата в меню...";
    _getch();


}