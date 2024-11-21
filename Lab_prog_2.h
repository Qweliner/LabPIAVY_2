#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <limits>


using namespace std;

// Структура для организации
struct Organization {
    string name;
    string address;
    string contactPerson;
};

// Структура для корреспонденции
struct Correspondence {
    string type;
    string date;
    string organizationName;
};

// Структура для объединенных данных
struct MergedData {
    vector<Organization> organizations;
    vector<Correspondence> correspondences;
};

void instruction() {
    system("cls");
    cout << "ИНСТРУКЦИЯ К ПРОГРАММЕ\n\n";
    cout << "Данная программа предназначена для работы с данными из двух файлов: \"Исходящая корреспонденция\" (IC_data.txt) и \"Адреса организаций\" (AO_data.txt).\n\n";

    cout << "Функциональные возможности:\n";
    cout << "1. Избирательный вывод на экран: выводит информацию об организации и ее корреспонденции.\n";
    cout << "2. Полный вывод в файл: создает отчет со всей информацией.\n\n";

    cout << "Использование:\n";
    cout << "1. Запустите программу.\n";
    cout << "2. Укажите путь к папке с файлами (путь должен заканчиваться на '\\').\n";
    cout << "3. Выберите режим работы (1 или 2).\n";
    cout << "4. Следуйте инструкциям на экране.\n\n";

    cout << "Требования к данным:\n";
    cout << "* Файлы должны быть в формате TSV.\n";
    cout << "* Дата в формате ДД.ММ.ГГГГ.\n";
    cout << "* В ФИО организатора недопустимы цифры.\n\n";

    cout << "Возможные ошибки:\n";
    cout << "* Неверный путь к папке.\n";
    cout << "* Файлы не найдены или повреждены.\n";
    cout << "* Некорректный формат данных.\n\n";

    cout << "Нажмите любую клавишу для продолжения...\n";
    system("pause");
}

// Функция запроса пути к папке
string getFolderPath() {
    string folderPath;
    while (true) {
        system("cls");
        cout << "Введите путь к папке (заканчивающийся на \\): ";
        getline(cin, folderPath);

        if (folderPath.empty()) {
            cout << "Путь не может быть пустым!\n";
            system("pause");
            continue;
        }

        if (folderPath.back() != '\\') {
            folderPath += '\\';
        }

        struct stat buffer;
        if (stat(folderPath.c_str(), &buffer) != 0) {
            cout << "Ошибка: Папка не найдена!\n";
            system("pause");
            continue;
        }
        return folderPath;
    }
}

// Функция загрузки данных из файла
bool loadFile(const string& filePath, MergedData& data) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла: " << filePath << endl;
        return false;
    }

    string line;
    getline(file, line); // Пропускаем заголовок

    if (filePath.find("IC_") != string::npos) { // Исходящая корреспонденция
        while (getline(file, line)) {
            stringstream ss(line);
            string type, date, orgName;
            getline(ss, type, '\t');
            getline(ss, date, '\t');
            getline(ss, orgName, '\t');
            data.correspondences.push_back({ type, date, orgName });
        }
    }
    else if (filePath.find("AO_") != string::npos) { // Адреса организаций
        while (getline(file, line)) {
            stringstream ss(line);
            string name, address, contact;
            getline(ss, name, '\t');
            getline(ss, address, '\t');
            getline(ss, contact, '\t');
            data.organizations.push_back({ name, address, contact });
        }
    }
    else {
        cerr << "Неизвестный тип файла: " << filePath << endl;
        return false;
    }
    file.close();
    return true;
}

// Функция объединения данных
void mergeData(const MergedData& icData, const MergedData& aoData, MergedData& mergedData) {
    mergedData.organizations = aoData.organizations;
    mergedData.correspondences = icData.correspondences;
}

// Функция избирательного вывода на экран
void selectiveOutput(const MergedData& mergedData) {
    string orgName;
    cout << "Название организации: ";
    getline(cin, orgName);

    auto itOrg = find_if(mergedData.organizations.begin(), mergedData.organizations.end(),
        [&](const Organization& org) { return org.name == orgName; });

    if (itOrg != mergedData.organizations.end()) {
        cout << "Организация: " << itOrg->name << endl;
        cout << "Адрес: " << itOrg->address << endl;
        cout << "ФИО: " << itOrg->contactPerson << endl;
        cout << "Корреспонденция:\n";
        for (const auto& corr : mergedData.correspondences) {
            if (corr.organizationName == orgName) {
                cout << "  Тип: " << corr.type << endl;
                cout << "  Дата: " << corr.date << endl;
            }
        }
    }
    else {
        cout << "Организация не найдена!\n";
    }
    system("pause");
}

// Функция полного вывода в файл
void fullOutputToFile(const MergedData& mergedData, const string& outputFilePath) {
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Ошибка открытия файла: " << outputFilePath << endl;
        return;
    }

    for (const auto& org : mergedData.organizations) {
        outputFile << "Организация: " << org.name << endl;
        outputFile << "Адрес: " << org.address << endl;
        outputFile << "ФИО: " << org.contactPerson << endl;
        outputFile << "Корреспонденция:\n";
        for (const auto& corr : mergedData.correspondences) {
            if (corr.organizationName == org.name) {
                outputFile << "  Тип: " << corr.type << endl;
                outputFile << "  Дата: " << corr.date << endl;
            }
        }
        outputFile << endl;
    }
    outputFile.close();
    cout << "Данные успешно записаны в " << outputFilePath << endl;
    system("pause");
}

// Функция отображения меню
void displayMenu() {
    char choice;
    do {
        system("cls");
        cout << "\nМЕНЮ\n";
        cout << "1) Избирательный вывод\n";
        cout << "2) Полный вывод в файл\n";
        cout << "3) Инструкция\n";
        cout << "Esc) Выход\n";
        cout << "> ";

        choice = cin.get(); 

        if (choice == 27) break; 

        MergedData mergedData;
        string folderPath = getFolderPath();
        if (folderPath.empty()) return;

        MergedData icData, aoData;
        string icFilePath = folderPath + "IC_data.txt";
        string aoFilePath = folderPath + "AO_data.txt";

        if (!loadFile(icFilePath, icData) || !loadFile(aoFilePath, aoData)) {
            cerr << "Ошибка загрузки данных!\n";
            system("pause");
            return;
        }
        mergeData(icData, aoData, mergedData);

        try {
            switch (choice) {
            case '1':
                selectiveOutput(mergedData);
                break;
            case '2': {
                string outputFileName;
                cout << "Имя выходного файла (без расширения): ";
                cin >> outputFileName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //https://stackoverflow.com/questions/25020129/cin-ignorenumeric-limitsstreamsizemax-n
                if (outputFileName.empty()) {
                    cout << "Имя файла не может быть пустым!\n";
                    system("pause");
                    break;
                }
                string outputFilePath = folderPath + outputFileName + ".txt";
                fullOutputToFile(mergedData, outputFilePath);
                break;
            }
            case '3':
                instruction();
                break;
            default:
                cout << "Неверный выбор!\n";
                system("pause");
            }
        }
        catch (const exception& e) {
            cerr << "Произошла ошибка: " << e.what() << endl;
            system("pause");
        }
    } while (true);
}

// Функция запуска программы
void runProgram() {
    displayMenu();
}

// Функция проверки корректности даты
bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '.' || dateStr[5] != '.') return false;
    for (char c : dateStr) {
        if (!isdigit(c) && c != '.') return false;
    }
    try {
        int d = stoi(dateStr.substr(0, 2));
        int m = stoi(dateStr.substr(3, 2));
        int y = stoi(dateStr.substr(6, 4));
        if (m < 1 || m > 12 || d < 1 || d > 31) return false;
        if (m == 2) {
            bool leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
            if (d > (leap ? 29 : 28)) return false;
        }
        if ((m == 4 || m == 6 || m == 9 || m == 11) && d == 31) return false;
        return true;
    }
    catch (const invalid_argument& e) {
        return false;
    }
}