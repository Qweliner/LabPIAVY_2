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

bool isValidDate(const string& dateStr) { // Изменено: теперь принимает std::string
    if (dateStr.length() != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year); // Используем c_str()

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

void correctData(string& data, const string& fieldName) {
    char choice;
    bool new_is_correct = false;
    string new_data;

    cout << "Некорректное значение поля " << fieldName << ": " << data << endl;
    cout << "Желаете изменить? (y/n): ";
    choice = _getch();
    if (not(choice == 'y' || choice == 'Y' || choice == 'у' || choice == 'У' || choice == 'н' || choice == 'Н')) {
        return; //Выходим из функции, если обновление не нужно
    }
    while (!new_is_correct) {
        cout << "Введите исправленное значение: ";
        getline(cin, new_data);
        if (fieldName == "Дата") {
            new_is_correct = isValidDate(new_data);
        }
        else if (fieldName == "ФИО") {
            new_is_correct = isValidFio(new_data);
        }
        else {
            new_is_correct = true;
        }
    }
    data = new_data;
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

void printSelectiveInfo(vector<Correspondence> correspondence, vector<Address> addresses, const string& corrFile, const string& addrFile) { // Убрали & из векторов
    string orgName;
    cout << "Введите название организации для поиска: ";
    getline(cin, orgName);

    bool found = false;
    for (size_t i = 0; i < correspondence.size(); ++i) {
        if (correspondence[i].organization == orgName) {
            found = true;
            for (size_t j = 0; j < addresses.size(); ++j) {
                if (addresses[j].organization == orgName) {
                    bool dateError = !isValidDate(correspondence[i].date);
                    bool fioError = !isValidFio(addresses[j].contactPerson);

                    if (dateError) {
                        correctData(correspondence[i].date, "Дата");
                    }
                    if (fioError) {
                        correctData(addresses[j].contactPerson, "ФИО");
                    }
                    cout << "\n--------------------" << endl;
                    cout << "Тип корреспонденции: " << correspondence[i].type << endl;
                    cout << "Дата: " << correspondence[i].date << endl;
                    cout << "Адрес: " << addresses[j].address << endl;
                    cout << "Контактное лицо: " << addresses[j].contactPerson << endl;
                    cout << "--------------------" << endl;
                }
            }
        }
    }
    if (!found) {
        cerr << "Корреспонденция для организации " << orgName << " не найдена." << endl;
    }
}

void printAllInfoToFile(const vector<Correspondence>& correspondence, const vector<Address>& addresses, const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        return;
    }

    map<string, vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        string fioCopy = addr.contactPerson;
        if (!isValidFio(fioCopy)) {
            correctData(fioCopy, "ФИО");
        }
        outfile << "Организация: " << addr.organization << endl;
        outfile << "Адрес: " << addr.address << endl;
        outfile << "Контактное лицо: " << fioCopy << endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                string dateCopy = corr.date;
                if (!isValidDate(dateCopy)) {
                    correctData(dateCopy, "Дата");
                }
                outfile << "\tТип корреспонденции: " << corr.type << endl;
                outfile << "\tДата: " << dateCopy << endl;
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

    vector<Correspondence> correspondence;
    vector<Address> addresses;

    try {
        correspondence = readCorrespondence(correspondenceFilename);
        addresses = readAddresses(addressesFilename);
    }
    catch (const std::runtime_error& error) {
        cerr << "Ошибка при чтении файлов: " << error.what() << endl;
        cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();
        return;
    }

    if (correspondence.empty() || addresses.empty()) {
        cerr << "Ошибка: Файлы корреспонденции или адресов пусты." << endl;
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
        printSelectiveInfo(correspondence, addresses, correspondenceFilename, addressesFilename);
        break;
    case '2': case 'o': case 'O':
        printAllInfoToFile(correspondence, addresses, outputFilename);
        break;
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
            printInstructions();
            break;
        case 27:
            return;
        default:
            cerr << "Неверный выбор.\n";
        }
    }
}