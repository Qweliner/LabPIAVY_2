#include "Lab_prog_2.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <conio.h>

using namespace std;

// ОПРЕДЕЛЕНИЕ глобальной переменной
string currentFolderPath = "";

/** @brief Выполняет чтение строки из консоли с поддержкой Esc и Backspace и фильтрацией недопустимых символов.
 *  @param instruction Сообщение, отображаемое пользователю перед вводом.
 *  @return Введенная строка, или пустая строка, если был нажат Esc.
 */
string getLineWithEsc(const string& instruction) {
    string input;
    cout << instruction;
    while (true) {
        int key = _getch();
        char ch = static_cast<char>(key);
        if (key == 27) { // ESC
            cin.clear();
            input = ""; // Очистка строки ввода
            cout << endl;
            return input;
        }
        else if (key == '\r') { // Enter
            cout << endl;
            return input;
        }
        else if (key == 8) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        // Разрешены: латиница, кириллица, цифры, пробел, точка, тире, подчеркивание
         else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                   (ch >= 'а' && ch <= 'я') || (ch >= 'А' && ch <= 'Я') ||
                   (ch >= '0' && ch <= '9') || ch == ' ' || ch == '.' || ch == '-' || ch == '_')
        {
            input += ch;
            cout << ch;
        }
    }
}

/** @brief Читает инструкции из файла. */
void readInstructionsFromFile(const string& filename) {
    system("mode con cols=150 lines=36");
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            cout << line << endl;
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл инструкции: " << filename << endl;
        cerr << "Пожалуйста, поместите файл instructions.txt в ту же папку, где находится исполняемый файл программы.\n";
    }
    cout << "\nНажмите любую клавишу для продолжения...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

/** @brief Обрабатывает информацию об организации.
 *  Находит и выводит данные организации из файлов корреспонденции и адресов.
 */
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename,
    bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer, const string& folderPath) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

    auto writeToBuffer = [&](const string& str) {
        outputBuffer.push_back(str);
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
                }
            }
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        writeToBuffer("Не удалось открыть файл адресов: " + folderPath+ addrFilename);
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
                            *outfile << "\nДругие найденные данные компании:\n" << endl;
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
            ifstream corrFile(folderPath + corrFilename);

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
                writeToBuffer("Не удалось открыть файл корреспонденции: " + folderPath + corrFilename);
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

    correspondenceFilename = getLineWithEsc("Введите имя файла с исходящей корреспонденцией (без расширения) или нажмите Esc для отмены: ");
    if (correspondenceFilename.empty()) {
        return { "", "" };
    }

    addressesFilename = getLineWithEsc("Введите имя файла с адресами организаций (без расширения) или нажмите Esc для отмены: ");
    if (addressesFilename.empty()) {
        return { "", "" };
    }

    return { correspondenceFilename, addressesFilename };
}

/** @brief Запускает основной процесс программы. */
void runProgram(const string& folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) { // Добавлен бесконечный цикл для выбора режима вывода
        bool escPressed = false; // Локальная переменная для runProgram
        cout << "\nВыберите режим вывода:\n";
        cout << "1. Избирательный вывод на экран\n";
        cout << "2. Полный вывод в файл\n";
        cout << "Esc - вернуться в главное меню\n";
        cout << "\nВаш выбор: ";

        char choice = _getch();
        char choiceChar = static_cast<char>(choice);
        if (choiceChar != '1' && choiceChar != '2' && choice != 27) {
            cerr << "Неверный выбор.\n";
            cout << "Нажмите любую клавишу для продолжения...\n";
            _getch();
             continue;
        }
        cout << endl;

        switch (choice) {
            case '1': {
                string orgName = getLineWithEsc("Введите название организации для поиска или нажмите Esc для отмены: ");
                if (orgName.empty()) {
                    break; // Выход из case '1', возврат в цикл выбора режима
                }

                ofstream* outfile = nullptr;  // Указатель на ofstream, инициализируем nullptr
                set<string> printedOrganizations;
                vector<string> outputBuffer;
                processOrganization(orgName, correspondenceFilename, addressesFilename,
                                    true, outfile, printedOrganizations, outputBuffer, folderPath);

                // Вывод в консоль
                for (const string& line : outputBuffer) {
                    cout << line << endl;
                }

                cout << "Нажмите любую клавишу для возврата в главное меню...\n"; // Добавлено
                _getch();                                                        // Добавлено
                system("cls");                                                    // Добавлено
                break; // Выход из case '1', возврат в цикл выбора режима
            }
            case '2': {
                // Формируем базовое имя файла
                string baseFilename = folderPath + "Отчет_IC_" + correspondenceFilename + "_AO_" + addressesFilename;
                string filename = baseFilename + ".txt";
                int i = 1;
                while (filesystem::exists(filename)) {
                    filename = baseFilename + "(" + to_string(i++) + ")" + ".txt";
                }
                try {
                    ofstream outfile(filename);

                    if (!outfile.is_open()) {
                        cerr << "Ошибка: не удалось открыть файл для записи.\n";
                        break; // Выход из case '2', возврат в цикл выбора режима
                    }
                    set<string> printedOrganizations;
                    vector<string> outputBuffer; // Этот буфер больше не нужен.
                    ifstream addrFile(folderPath + addressesFilename);

                    if (addrFile.is_open()) {
                        string line;
                        getline(addrFile, line); // Пропустить заголовок
                        while (getline(addrFile, line)) {
                            size_t pos = line.find('\t');
                            if (pos != string::npos) {
                                string orgName = line.substr(0, pos);
                                if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
                                    processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer, folderPath);  // Передаем &outfile
                                }
                            }
                        }
                        addrFile.close();
                    }
                    else {
                        cerr << "Ошибка: не удалось открыть файл адресов: " + addressesFilename + ".\n";
                            break;
                    }
                    outfile.close(); // Закрываем файл *перед* ожиданием
                    cout << "Информация успешно записана в файл " << filename << ".\n";

                    cout << "Нажмите любую клавишу для возврата в главное меню...\n";
                    _getch();
                    break; // Выход из case '2', возврат в цикл выбора режима
                }
                catch (const exception& e)
                {
                    cerr << "Exception occurred: " << e.what() << endl;
                    break;
                }
            }
            case 27:
                return;  // Сразу выходим, если ESC

            default:
                cerr << "Неверный выбор.\n";
                cout << "Нажмите любую клавишу для продолжения...\n";
                _getch();
        }
    }
}

/** @brief Отображает главное меню. */
void menu() {
    string correspondenceFilename, addressesFilename, outputFilename;
    string escPressed; //Для отслеживания esc в getFileName
    bool folderPathSet = true;
    currentFolderPath = normalizePath(filesystem::current_path().string()) + "\\";
    const string instructionsFile = currentFolderPath + "instructions.txt";

    cout << "\nПрограмма для обработки данных о корреспонденции и адресах организаций.\n";
    cout << "Она позволяет искать информацию об организациях и их корреспонденции, а также формировать отчеты.\n";

    while (true) {
        system("cls");

        // Вывод заголовка программы
        cout << "*************************************************************\n";
        cout << "* Программа для обработки данных корреспонденции и адресов  *\n";
        cout << "* Программа позволяет искать и обрабатывать информацию об   *\n";
        cout << "* организациях на основе двух файлов -                      *\n";
        cout << "* Исходящей корреспонденции (IC_ ***.txt)                   *\n";
        cout << "* Адресов организаций (AO_***.txt)                          *\n";
        cout << "*************************************************************\n\n";

        cout << "Главное меню:\n";
        cout << "1. Начать работу\n";
        cout << "2. Изменить путь к папке\n";
        cout << "3. Инструкция\n\n";
        cout << "Текущий путь: " << currentFolderPath << "\n\n";
        cout << "Esc - выход\n\n";

        char mainChoice = _getch();
        char mainChoiceChar = static_cast<char>(mainChoice);

        if (mainChoiceChar != '1' && mainChoiceChar != '2' && mainChoice != 27 && mainChoiceChar != '3') {
            cerr << "Неверный выбор.\n";
            cout << "Нажмите любую клавишу для продолжения...\n";
            _getch();
             continue;
        }

        switch (mainChoice) {
            case '1': {
                pair<string, string> filenames = getFilenamesFromUser(currentFolderPath);
                if (filenames.first.empty() && filenames.second.empty()) {
                    break;  // Прерываем "Начать работу", если ввод файлов отменен
                }
                correspondenceFilename = filenames.first;
                addressesFilename = filenames.second;
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename); // Передаем filenames
                break; // Выход из case '1'
            }
            case '2': {
                string newPath = getLineWithEsc("Введите новый путь к папке: ");
                if(newPath.empty()) break; //Если нажат Esc, то выходим из case '2'
                newPath = normalizePath(newPath); // Нормализация введенного пути

                // Проверка существования директории
                struct stat buffer;
                if (stat(newPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR)) {
                    currentFolderPath = newPath + "\\";
                    cout << "Путь успешно изменен.\n";
                } else {
                    cerr << "Ошибка: указанный путь не существует или не является директорией.\n";
                }
                cout << "Нажмите любую клавишу для продолжения...\n";
                _getch();
                break;
            }
            case 3: {
                readInstructionsFromFile(instructionsFile);
                break;
            }
            case 27:
                return;
            default:
                cerr << "Неверный выбор.\n";
        }
    }
}

/** @brief Нормализует путь к файлу, делая его абсолютным.
 */
string normalizePath(const string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);
    return normalizedPath.string();
}