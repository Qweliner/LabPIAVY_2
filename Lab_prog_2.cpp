//Lab_prog_2.cpp
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

string getLineWithEsc(const string& instruction) {
    string input;
    cout << instruction;
    while (true) {
        int key = _getch();
        char ch = static_cast<char>(key);
        if (key == 27) { // ESC
            cin.clear();
            input = "";
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
        // Разрешаем печатные символы (включая кавычки)
        else if (ch >= 32 && ch <= 126) {
            input += ch;
            cout << ch;
        }
        // Кириллица
        else if ((ch >= 'а' && ch <= 'я') || (ch >= 'А' && ch <= 'Я'))
        {
            input += ch;
            cout << ch;
        }
    }
}

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
    cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<Address> addresses;
    bool orgFoundInAddress = false;

    auto writeToBuffer = [&](const string& str) {
        if (selectiveOutput) {
            outputBuffer.push_back(str); // Для консольного вывода
        }
        else if (outfile) {
            *outfile << str << endl;    // Для файлового вывода
        }
        };


    // Сначала ищем организацию в файле адресов.  Это нужно для обоих режимов вывода.
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            getline(ss, currentOrg, ';');
            getline(ss, address, ';');
            getline(ss, contactPerson, ';');
            currentOrg.erase(0, currentOrg.find_first_not_of(" \t"));  //Удаляем пробелы
            currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1);
            if (currentOrg == orgName) {
                addresses.push_back({ currentOrg, address, contactPerson });
                orgFoundInAddress = true;
            }
        }
        addrFile.close();
    }
    else {
        writeToBuffer("Не удалось открыть файл адресов: " + addrFilename);
        return; // Если не удалось открыть файл адресов, дальше не идём
    }

    if (!orgFoundInAddress && selectiveOutput) {
        writeToBuffer("Название организации \"" + orgName + "\" не найдена в файле адресов.");
        return;
    }

    //Если это полный вывод, выводим данные для каждой записи
    if (!selectiveOutput)
    {
        if (printedOrganizations.count(orgName)) {
            return; // Название организации уже была обработана
        }
        printedOrganizations.insert(orgName); // Помечаем организацию как обработанную
        // Вывод информации об организации (из файла адресов)
        writeToBuffer("Название организации: " + orgName);

        for (const auto& addr : addresses) {
            writeToBuffer("Адрес: " + addr.address);
            writeToBuffer("Фамилия руководителя: " + addr.contactPerson);
        }

        // Поиск и вывод корреспонденции для организации
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                currentOrg.erase(0, currentOrg.find_first_not_of(" \t")); //Удаляем пробелы
                currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1);
                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\tВид корреспонденции: " + type);
                    writeToBuffer("\tДата подготовки: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\tКорреспонденция не найдена."); // Для полного вывода
            }
            corrFile.close();
        }
        else {
            writeToBuffer("Не удалось открыть файл корреспонденции: " + corrFilename);
        }
        writeToBuffer("--------------------");
    }

    //Если это поиск
    if (selectiveOutput && orgFoundInAddress)
    {
        writeToBuffer("Название организации: " + orgName);
        if (!addresses.empty()) {
            writeToBuffer("Адрес: " + addresses[0].address);
            writeToBuffer("Фамилия руководителя: " + addresses[0].contactPerson);

            if (addresses.size() > 1) {
                writeToBuffer("\nДругие найденные данные компании:\n");
                for (size_t i = 1; i < addresses.size(); ++i) {
                    writeToBuffer("Адрес: " + addresses[i].address);
                    writeToBuffer("Фамилия руководителя: " + addresses[i].contactPerson);
                }
            }
        }
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                currentOrg.erase(0, currentOrg.find_first_not_of(" \t"));  //Удаляем пробелы
                currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1); //

                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\tВид корреспонденции: " + type);
                    writeToBuffer("\tДата подготовки: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\tКорреспонденция не найдена."); // Сообщение, если не найдено
            }
            corrFile.close();
        }
        else {
            writeToBuffer("Не удалось открыть файл корреспонденции: " + corrFilename);
        }
        writeToBuffer("--------------------"); // Добавляем разделитель
    }
}

pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    while (true) {
        correspondenceFilename = getLineWithEsc("Введите имя файла с исходящей корреспонденцией (префикса IC_ и без расширения .txt) или нажмите Esc для отмены: ");
        if (correspondenceFilename.empty()) {
            return { "", "" };
        }

        string fullCorrFilename = folderPath + "IC_" + correspondenceFilename + ".txt";
        if (!filesystem::exists(fullCorrFilename)) {
            cerr << "Ошибка: Файл " << fullCorrFilename << " не существует.\n";
            cout << "Пожалуйста, попробуйте еще раз.\n";
            continue;
        }
        break;
    }

    while (true) {
        addressesFilename = getLineWithEsc("Введите имя файла с адресами организаций (префикса AO_ и без расширения .txt) или нажмите Esc для отмены: ");
        if (addressesFilename.empty()) {
            return { "", "" };
        }

        string fullAddressesFilename = folderPath + "AO_" + addressesFilename + ".txt";
        if (!filesystem::exists(fullAddressesFilename)) {
            cerr << "Ошибка: Файл " << fullAddressesFilename << " не существует.\n";
            cout << "Пожалуйста, попробуйте еще раз.\n";
            continue;
        }
        break;
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt",
             folderPath + "AO_" + addressesFilename + ".txt" };
}

void runProgram(const string& folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) {
        cout << "\nВыберите режим вывода:\n";
        cout << "1. Поиск по компании и вывод в консоль\n";
        cout << "2. Полный вывод в файл\n";
        cout << "Esc - вернуться в главное меню\n";
        cout << "\nВаш выбор: ";

        char choice = _getch();
        char choiceChar = static_cast<char>(choice);

        while (choiceChar != '1' && choiceChar != '2' && choice != 27) {
            choice = _getch();
            choiceChar = static_cast<char>(choice);
        }

        cout << endl;

        switch (choice) {
        case '1': {
            string orgName = getLineWithEsc("Введите название организации для поиска или нажмите Esc для отмены: ");
            if (orgName.empty()) {
                break;
            }

            ofstream* outfile = nullptr; // Указатель на ofstream (не создаём объект!)
            set<string> printedOrganizations;
            vector<string> outputBuffer; // Буфер для вывода
            processOrganization(orgName, correspondenceFilename, addressesFilename,
                true, outfile, printedOrganizations, outputBuffer);

            // Вывод буфера на консоль
            for (const string& line : outputBuffer) {
                cout << line << endl;
            }
            break;
        }
        case '2': {
            // Получаем имена файлов без путей и расширений для формирования имени выходного файла
            string corrFileNameOnly = filesystem::path(correspondenceFilename).filename().string();
            string addrFileNameOnly = filesystem::path(addressesFilename).filename().string();

            size_t pos = corrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                corrFileNameOnly = corrFileNameOnly.substr(0, pos);
            }

            pos = addrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                addrFileNameOnly = addrFileNameOnly.substr(0, pos);
            }

            // Формируем имя выходного файла (с путём)
            string baseFilename = folderPath + "Отчет_" + corrFileNameOnly + "_" + addrFileNameOnly;
            string filename = baseFilename + ".txt";
            int i = 1;
            while (filesystem::exists(filename)) { // Проверяем, существует ли файл
                filename = baseFilename + "(" + to_string(i++) + ")" + ".txt"; // Добавляем (1), (2) и т.д.
            }

            try { //Добавил обработку исключений

                ofstream outfile(filename); // Создаём ofstream *после* формирования имени файла

                if (!outfile.is_open()) {
                    cerr << "Ошибка: не удалось открыть файл для записи.\n";
                    break;
                }

                set<string> printedOrganizations; // Множество для отслеживания уже выведенных организаций
                vector<string> outputBuffer;       //Вектор для буферизации вывода

                ifstream addrFile(addressesFilename); // Открываем файл адресов
                if (addrFile.is_open()) {
                    string line;
                    while (getline(addrFile, line)) {
                        stringstream ss(line);
                        string orgName, address, contactPerson;
                        getline(ss, orgName, ';');
                        getline(ss, address, ';');
                        getline(ss, contactPerson, ';');

                        orgName.erase(0, orgName.find_first_not_of(" \t"));  //Удаляем пробелы
                        orgName.erase(orgName.find_last_not_of(" \t") + 1);

                        //Если орагнизация не встречалась, то выводим о ней информацию
                        if (printedOrganizations.find(orgName) == printedOrganizations.end()) {

                            processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);

                        }
                    }
                    addrFile.close();
                }
                else {
                    cerr << "Ошибка: не удалось открыть файл адресов: " << addressesFilename << ".\n";
                    break;
                }

                outfile.close();
                cout << "Информация успешно записана в файл " << filename << ".\n";
                break;
            }
            catch (const exception& e)
            {
                cerr << "Ошибка: не удалось создать или записать файл.\n";
                cerr << e.what() << endl;
                break;
            }
        }
        case 27:
            return;  // Выход из runProgram
        }
    }
}

void menu() {
    string correspondenceFilename, addressesFilename, outputFilename;
    string escPressed;
    bool folderPathSet = true; // Флаг, что путь к папке установлен
    currentFolderPath = normalizePath(filesystem::current_path().string()) + "\\"; // Сразу нормализуем
    const string instructionsFile = currentFolderPath + "instructions.txt";

    cout << "\nПрограмма для обработки данных о корреспонденции и адресах организаций.\n";
    cout << "Она позволяет искать информацию об организациях и их корреспонденции, а также формировать отчеты.\n";

    while (true) {
        system("cls"); // Очищаем экран

        cout << "*************************************************************\n";
        cout << "* Программа для обработки данных корреспонденции и адресов  *\n";
        cout << "* Программа позволяет искать и обрабатывать информацию об   *\n";
        cout << "* организациях на основе двух файлов -                      *\n";
        cout << "* Исходящей корреспонденции (IC_***.txt)                    *\n";
        cout << "* Адресов организаций (AO_***.txt)                          *\n";
        cout << "*************************************************************\n\n";

        cout << "Главное меню:\n";
        cout << "1. Начать работу\n";
        cout << "2. Изменить путь к папке\n";
        cout << "3. Инструкция\n\n";
        cout << "Текущий путь: " << currentFolderPath << "\n\n";
        cout << "Esc - выход\n\n";

        int mainChoice = _getch(); // Считываем символ
        char mainChoiceChar = static_cast<char>(mainChoice); // char

        switch (mainChoice) {
        case '1': { // "Начать работу"
            // Получаем имена файлов от пользователя
            pair<string, string> filenames = getFilenamesFromUser(currentFolderPath);
            if (filenames.first.empty() && filenames.second.empty()) {
                break; // Если пользователь нажал Esc, выходим из case '1'
            }
            correspondenceFilename = filenames.first;
            addressesFilename = filenames.second;
            runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename);
            break;
        }
        case '2': { // "Изменить путь к папке"
            string newPath = getLineWithEsc("Введите новый путь к папке: ");
            if (newPath.empty()) break;
            newPath = normalizePath(newPath); // Нормализуем путь

            struct stat buffer;

            // Проверка на то, существует ли папка и является ли она директорией
            if (stat(newPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR)) {
                if (newPath.back() != '\\') // Проверка есть ли \ в конце пути
                    currentFolderPath = newPath + "\\"; // Добавляем разделитель
                cout << "Путь успешно изменен.\n";
            }
            else {
                cerr << "Ошибка: указанный путь не существует или не доступен.\n";
            }
            cout << "Нажмите любую клавишу для продолжения...\n"; // Добавлено сообщение
            _getch(); // Ждём нажатия клавиши
            break;
        }

        case '3': // Инструкция
            readInstructionsFromFile(instructionsFile);
            break;

        case 27: // Esc - выход
            return;

        default:
            continue; // Некорректный ввод - продолжаем цикл
        }
    }
}

string normalizePath(const string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);  // Получаем абсолютный путь
    return normalizedPath.string(); // Возвращаем как строку
}