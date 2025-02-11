#include "Lab_prog_2.h"
#include <algorithm>
#include <fstream> //  Добавлен для работы с файлами
#include <iostream>
#include <limits>
#include <sstream>
#include <vector> // Добавляем vector для хранения вывода

using namespace std;

// Инициализация глобальной переменной
string currentFolderPath = "";

/** @brief Выводит табуляцию. */
void tabul(int x) {
    for (int i = x; i != 0; i--)
        printf("   "); // Вывод трех пробелов
}

/** @brief Определяет, является ли год високосным. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief Проверяет допустимость ФИО. */
bool isValidFio(const string& fioStr) {
    bool valid_char = true;
    for (char c : fioStr) {
        if (not((c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я') ||
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' ||
            c == '-' || c == '\x2D'))
            valid_char = false;
    }
    return valid_char;
}

/** @brief Проверяет допустимость формата даты. */
bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10)
        return false;
    int day = 0, month = 0, year = 0;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year);
    if (count != 3)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > daysInFeb)
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    return true;
}

/** @brief Проверяет допустимость имени файла. */
bool isValidFileName(const string& fileName) {
    const string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == string::npos;
}

/** @brief Читает инструкции из файла. */
void readInstructionsFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            cout << line << endl;
        file.close();
    }
    else
        cerr << "Не удалось открыть файл инструкции: " << filename << endl;
    cout << "\nНажмите любую клавишу для продолжения...";
    _getch();
}

/** @brief Обрабатывает информацию об организации.
 *  Находит и выводит данные организации из файлов корреспонденции и адресов.
 */
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile,set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

    auto writeToBuffer = [&](const string& str) {
        outputBuffer.push_back(str); // Записываем строку в буфер
        };

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
                    addresses.push_back({ line.substr(pos1 + 1, pos2 - pos1 - 1),
                                         line.substr(pos2 + 1) });
                    orgFoundInAddress = true;
                    string contactFio = line.substr(pos2 + 1);
                    if (!isValidFio(contactFio)) {
                        writeToBuffer("Ошибка в файле: " + addrFilename + ", строка: " + to_string(lineNumber) +
                            ", некорректное значение поля ФИО: " + contactFio);
                    }
                }
            }
            if (!selectiveOutput && outfile &&
                printedOrganizations.find(orgName) != printedOrganizations.end())
                break; // Прерываем, если не селективный и уже напечатан
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        writeToBuffer("Не удалось открыть файл адресов: " + addrFilename);
        return;
    }

    if (!orgFoundInAddress) {
        if (selectiveOutput)
            writeToBuffer("Организация " + orgName + " не найдена в файле адресов.");
        return;
    }

    if (selectiveOutput) {
        writeToBuffer("Организация: " + orgName);
        if (addresses.size() > 0) {
            for (size_t i = 0; i < addresses.size(); ++i) {
                if (i == 0) {
                    writeToBuffer("Адрес: " + addresses[i].first);
                    writeToBuffer("Контактное лицо: " + addresses[i].second);
                }
                else {
                    if (i == 1)
                        writeToBuffer("\nДругие найденные данные компании:\n");
                    writeToBuffer("\nАдрес: " + addresses[i].first);
                    writeToBuffer("Контактное лицо: " + addresses[i].second);
                }
            }
        }
    }
    else if (outfile) {
        if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
            printedOrganizations.insert(orgName);
            *outfile << "Организация: " << orgName << endl;
            if (addresses.size() > 0) {
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i == 0) {
                        *outfile << "Адрес: " << addresses[i].first << endl;
                        *outfile << "Контактное лицо: " << addresses[i].second << endl;
                    }
                    else {
                        if (i == 1)
                            *outfile << "\nДругие найденные данные компании:\n";
                        *outfile << "\nАдрес: " << addresses[i].first << endl;
                        *outfile << "Контактное лицо: " << addresses[i].second << endl;
                    }
                }
            }
        }
    }

    if (orgFoundInAddress) {
        if (selectiveOutput ||
            (outfile && printedOrganizations.find(orgName) !=
                printedOrganizations.end())) {
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

                            if (dateError)
                                writeToBuffer("Ошибка в файле: " + corrFilename + ", строка: " + to_string(lineNumber)
                                    + ", некорректное значение поля Дата: " + date);

                            if (selectiveOutput) {
                                writeToBuffer("\tТип корреспонденции: " + type);
                                writeToBuffer("\tДата: " + date);
                            }
                            else if (outfile) {
                                *outfile << "\tТип корреспонденции: " << type << endl;
                                *outfile << "\tДата: " << date << endl;
                            }
                        }
                    }
                    lineNumber++;
                }
                if (!foundCorrespondence && selectiveOutput)
                    writeToBuffer("Корреспонденция для организации " + orgName + " не найдена.");
                corrFile.close();
            }
            else
                writeToBuffer("Не удалось открыть файл корреспонденции: " + corrFilename);
        }
    }
    if (selectiveOutput)
        writeToBuffer("--------------------");
    else if (outfile)
        *outfile << "--------------------" << endl;
}

/** @brief Запрашивает имена файлов у пользователя. */
pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    cout << "Введите имя файла с исходящей корреспонденцией (без расширения): ";
    cin >> correspondenceFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(correspondenceFilename)) {
        tabul(11);
        printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое "
            "имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER.");
        system("PAUSE>nul");
    }

    cout << "Введите имя файла с адресами организаций (без расширения): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11);
        printf("Недопустимые символы в имени файла. Пожалуйста, используйте другое "
            "имя.\n");
        printf("ДЛЯ ПРОДОЛЖЕНИЯ НАЖМИТЕ ENTER.");
        system("PAUSE>nul");
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt",
            folderPath + "AO_" + addressesFilename + ".txt" };
}

/** @brief Запускает основной процесс программы. */
void runProgram(string& folderPath, string& correspondenceFilename,
    string& addressesFilename, string& outputFilename) {
    pair<string, string> filenames = getFilenamesFromUser(folderPath);
    correspondenceFilename = filenames.first;
    addressesFilename = filenames.second;

    if (folderPath.back() != '\\' && folderPath.back() != '/')
        folderPath += '\\';

    outputFilename = folderPath + "output.txt";

    char choice;
    cout << "\nВыберите режим вывода:\n";
    cout << "1. Избирательный вывод на экран\n";
    cout << "2. Полный вывод в файл\n";
    cout << "Esc - Вернуться в главное меню\n";
    cout << "\nВаш выбор: ";

    choice = _getch();
    cout << endl;

    switch (choice) {
    case '1': {
        string orgName;
        cout << "Введите название организации для поиска: ";
        getline(cin, orgName);
        ofstream* outfile = nullptr;
        set<string> printedOrganizations;
        vector<string> outputBuffer; // Создаем буфер для хранения данных
        processOrganization(orgName, correspondenceFilename, addressesFilename,
            true, outfile, printedOrganizations, outputBuffer);

        if (outputBuffer.size() > 28) { // Если строк больше, чем 28
            string currentFolderPathselectiveoutput = folderPath + "selective_output.txt";
                ofstream selectiveOutfile(currentFolderPathselectiveoutput);
            if (selectiveOutfile.is_open()) { // Проверяем, что файл открыт
                for (const string& line : outputBuffer)
                    selectiveOutfile << line << endl;
                selectiveOutfile.close();
                cout << "\nРезультат сохранен в файле selective_output.txt, так как превысил 28 строк.\n";
                cout << "Путь к результату: " << currentFolderPathselectiveoutput << "\n";
            }
            else
                cerr << "Ошибка: не удалось открыть файл для записи избирательного вывода.\n";
        }
        else { // Иначе, если строк не больше 28, то выводим в консоль
            for (const string& line : outputBuffer)
                cout << line << endl;
        }
        break;
    }
    case '2': {
        ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            cerr << "Ошибка: Не удалось открыть файл для записи: " << outputFilename
                << endl;
            return;
        }
        set<string> printedOrganizations;
        vector<string> outputBuffer; // Этот буфер больше не нужен
        ifstream addrFile(addressesFilename);
        if (addrFile.is_open()) {
            string line;
            getline(addrFile, line); // Пропустить заголовок
            while (getline(addrFile, line)) {
                size_t pos1 = line.find('\t');
                if (pos1 != string::npos) {
                    string orgName = line.substr(0, pos1);
                    if (printedOrganizations.find(orgName) == printedOrganizations.end())
                        processOrganization(orgName, correspondenceFilename,
                            addressesFilename, false, &outfile,
                            printedOrganizations, outputBuffer); // Прокидываем буфер для совместимости
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

/** @brief Отображает главное меню. */
void menu() {
    bool folderPathSet = false;
    string correspondenceFilename, addressesFilename, outputFilename;
    string currentFolderPath = "";
    const string instructionsFile = "instructions.txt";

    while (true) {
        system("cls");
        cout << "Главное меню:\n";
        cout << "1. Начать работу ";
        if (!folderPathSet)
            cout << "(сначала укажите путь к папке)\n";
        else
            cout << '\n';
        cout << "2. Путь к папке\n";
        cout << "3. Инструкция\n\n";
        cout << "Текущий путь: " << currentFolderPath << "\n\n";
        cout << "Esc - Выход\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1':
            if (!folderPathSet) {
                cerr << "Ошибка: сначала укажите путь к папке.\n";
                _getch();
            }
            else
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename,
                    outputFilename);
            break;
        case '2': {
            cout << "Введите путь к папке: ";
            getline(cin, currentFolderPath);

            struct stat buffer;
            if (stat(currentFolderPath.c_str(), &buffer) != 0) {
                if (errno == ENOENT)
                    cerr << "Ошибка: указанный путь не существует.\n";
                else
                    cerr << "Ошибка: не удалось проверить путь к папке. Код ошибки: "
                    << errno << "\n";
                _getch();
                continue; // Возврат в начало цикла, если путь некорректен
            }

            if (currentFolderPath.back() != '\\')
                currentFolderPath += '\\';

            folderPathSet = true;
            cout << "\nТекущий путь к папке: " << currentFolderPath << endl;
            cout << "Нажмите любую клавишу для продолжения...";
            _getch();
            break;
        }
        case '3':
            readInstructionsFromFile(instructionsFile);
            break;
        case 27:
            return;
        default:
            cerr << "Неверный выбор.\n";
        }
    }
}