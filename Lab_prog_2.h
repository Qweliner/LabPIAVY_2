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
#include <sys/stat.h>
#include <set>

#ifdef max
#undef max
#endif

string currentFolderPath = "";

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

bool isValidFio(const string& fio) {
    // Проверка на наличие цифр в ФИО
    bool valid_char = true;
    for (char c : fio) {
        if (not((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            c == ' ' || c == '-' || c == '\x2D')) {
            valid_char = false;
        }
    }
    return valid_char;
}

bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year);

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

void readInstructionsFromFile(const std::string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл инструкции: " << filename << endl;
    }
    cout << "\nНажмите любую клавишу для продолжения...";
    _getch();
}

void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile = nullptr, set<string>& printedOrganizations = *(new set<string>())) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

    if (addrFile.is_open()) {
        string line;
        getline(addrFile, line); // Пропустить заголовок
        lineNumber++;

        while (getline(addrFile, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                string currentOrg = line.substr(0, pos1);
                if (currentOrg == orgName) {

                    addresses.push_back({ line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1) });
                    orgFoundInAddress = true;
                    string fio = line.substr(pos2 + 1);
                    if (!isValidFio(fio)) {
                        cout << "Ошибка в файле: " << addrFilename << ", строка: " << lineNumber << ", некорректное значение поля ФИО: " << fio << endl;
                    }
                }
            }
            if (!selectiveOutput && outfile && printedOrganizations.find(orgName) != printedOrganizations.end()) {
                break;
            }
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        cerr << "Не удалось открыть файл адресов: " << addrFilename << endl;
        return;
    }

    if (!orgFoundInAddress) {
        if (selectiveOutput)
            cerr << "Организация " << orgName << " не найдена в файле адресов." << endl;
        return;
    }

    if (selectiveOutput) {
        cout << "Организация: " << orgName << endl;
        if (addresses.size() > 0) {
            for (size_t i = 0; i < addresses.size(); ++i) {
                if (i == 0) {
                    cout << "Адрес: " << addresses[i].first << endl;
                    cout << "Контактное лицо: " << addresses[i].second << endl;
                }
                else {
                    if (i == 1) cout << "\nДругие найденные данные компании:\n";
                    cout << "\nАдрес: " << addresses[i].first << endl;
                    cout << "Контактное лицо: " << addresses[i].second << endl;
                }
            }
        }
    }
    else if (outfile) {
        if (printedOrganizations.find(orgName) == printedOrganizations.end())
        {
            printedOrganizations.insert(orgName);
            *outfile << "Организация: " << orgName << endl;
            if (addresses.size() > 0) {
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i == 0) {
                        *outfile << "Адрес: " << addresses[i].first << endl;
                        *outfile << "Контактное лицо: " << addresses[i].second << endl;
                    }
                    else {
                        if (i == 1) *outfile << "\nДругие найденные данные компании:\n";
                        *outfile << "\nАдрес: " << addresses[i].first << endl;
                        *outfile << "Контактное лицо: " << addresses[i].second << endl;
                    }
                }
            }
        }
    }

    if (orgFoundInAddress) {
        if (selectiveOutput || (outfile && printedOrganizations.find(orgName) != printedOrganizations.end())) {
            if (corrFile.is_open()) {
                string line;
                getline(corrFile, line); // Пропустить заголовок
                lineNumber = 1;
                lineNumber++;
                bool foundCorrespondence = false;
                while (getline(corrFile, line)) {
                    size_t pos1 = line.find('\t');
                    size_t pos2 = line.find('\t', pos1 + 1);

                    if (pos1 != string::npos && pos2 != string::npos) {
                        string currentOrg = line.substr(pos2 + 1);
                        if (currentOrg == orgName) {
                            foundCorrespondence = true;
                            string type = line.substr(0, pos1);
                            string date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                            bool dateError = !isValidDate(date);

                            if (dateError) {
                                cout << "Ошибка в файле: " << corrFilename << ", строка: " << lineNumber << ", некорректное значение поля Дата: " << date << endl;
                            }

                            if (selectiveOutput) {
                                cout << "\tТип корреспонденции: " << type << endl;
                                cout << "\tДата: " << date << endl;
                            }
                            else if (outfile) {
                                *outfile << "\tТип корреспонденции: " << type << endl;
                                *outfile << "\tДата: " << date << endl;
                            }
                        }
                    }
                    lineNumber++;
                }
                if (!foundCorrespondence && selectiveOutput) {
                    cerr << "Корреспонденция для организации " << orgName << " не найдена." << endl;
                }
                corrFile.close();
            }
            else {
                cerr << "Не удалось открыть файл корреспонденции: " << corrFilename << endl;
            }
        }
    }
    if (selectiveOutput) {
        cout << "--------------------" << endl;
    }
    else if (outfile) {
        *outfile << "--------------------" << endl;
    }
}

pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    cout << "Введите имя файла с исходящей корреспонденцией (без расширения): ";
    cin >> correspondenceFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(correspondenceFilename)) {
        tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");

    }

    cout << "Введите имя файла с адресами организаций (без расширения): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11); printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER."); system("PAUSE>nul");

    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt", folderPath + "AO_" + addressesFilename + ".txt" };

}

void runProgram(string& folderPath, string& correspondenceFilename, string& addressesFilename, string& outputFilename) {
    pair<string, string> filenames = getFilenamesFromUser(folderPath);
    correspondenceFilename = filenames.first;
    addressesFilename = filenames.second;

    if (folderPath.back() != '\\' && folderPath.back() != '/') {
        folderPath += '\\';
    }

    outputFilename = folderPath + "output.txt";

    char choice;
    cout << "\nВыберите режим вывода:\n";
    cout << "1. Избирательный вывод на экран (I)\n";
    cout << "2. Полный вывод в файл (O)\n";
    cout << "Esc - Вернуться в главное меню\n";
    cout << "Ваш выбор: ";

    choice = _getch();
    cout << endl;

    switch (choice) {
    case '1': case 'i': case 'I': {
        string orgName;
        cout << "Введите название организации для поиска: ";
        getline(cin, orgName);
        processOrganization(orgName, correspondenceFilename, addressesFilename, true);
        break;
    }
    case '2': case 'o': case 'O': {
        ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл для записи: " << outputFilename << endl;
            return;
        }
        set<string> printedOrganizations;
        ifstream addrFile(addressesFilename);
        if (addrFile.is_open()) {
            string line;
            getline(addrFile, line); // Пропустить заголовок
            while (getline(addrFile, line)) {
                size_t pos1 = line.find('\t');
                if (pos1 != string::npos) {
                    string orgName = line.substr(0, pos1);
                    if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
                        processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations);
                    }
                }
            }
            addrFile.close();
        }
        outfile.close();
        cout << "\nИнформация успешно записана в файл: " << outputFilename << endl;
        break;
    }
    case 27:
        return;
    default:
        cerr << "Неверный выбор.\n";
    }

    cout << "Нажмите любую клавишу для возврата в меню...";
    _getch();
}

void menu() {
    bool folderPathSet = false;
    string correspondenceFilename, addressesFilename, outputFilename;
    string currentFolderPath = "";
    const string instructionsFile = "instructions.txt";

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
        cout << "3. Инструкция (I)\n\n";
        cout << "Текущий путь: " << currentFolderPath << "\n\n";
        cout << "Esc - Выход\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': case 's': case 'S':
            if (!folderPathSet) {
                cerr << "Ошибка: сначала укажите путь к папке.\n";
                _getch();
            }
            else {
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename);
            }
            break;
        case '2': case 'f': case 'F': {
            cout << "Введите путь к папке: ";
            getline(cin, currentFolderPath);

            // Проверка пути с использованием stat
            struct stat buffer;
            if (stat(currentFolderPath.c_str(), &buffer) != 0) {
                if (errno == ENOENT) {
                    cerr << "Ошибка: указанный путь не существует.\n";
                }
                else {
                    cerr << "Ошибка: не удалось проверить путь к папке. Код ошибки: " << errno << "\n";
                }
                _getch();
                continue; // Возврат в начало цикла, если путь некорректен
            }

            // Добавляем '\' в конце пути, если его там нет
            if (currentFolderPath.back() != '\\') {
                currentFolderPath += '\\';
            }

            folderPathSet = true;
            cout << "\nТекущий путь к папке: " << currentFolderPath << endl;
            cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        }
        case '3': case 'i': case 'I':
            readInstructionsFromFile(instructionsFile);
            break;
        case 27:
            return;
        default:
            cerr << "Неверный выбор.\n";
        }
    }
}