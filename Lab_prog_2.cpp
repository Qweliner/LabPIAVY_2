//Lab_prog_2.cpp
#include "Lab_prog_2.h"
#define _CRT_SECURE_NO_WARNINGS // Отключение предупреждений безопасности CRT
#pragma warning(disable:4996) // Отключение конкретного предупреждения C4996

using namespace std;

// --- Глобальные переменные ---
char folder_way[256] = { 0 }; // Путь к рабочей папке
const char* file_extension = ".txt"; // Расширение файлов данных
const char* ocfe = "IC_"; // Префикс файлов корреспонденции
const char* oa = "AO_";   // Префикс файлов адресов

// --- Функции валидации символов ---
bool isValidFileNameChar(char c) { // Проверка символа для имени файла
    unsigned char uc = (unsigned char)c;
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

bool isValidPathChar(char c) { // Проверка символа для пути
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

bool isValidOrgNameChar(char c) { // Проверка символа для названия организации
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // Лат.+Кир.
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // Доп. символы: - _ . , " ( ) №(211) + ! & : «(171) »(187) #
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#');
}

bool isValidAddressChar(char c) { // Проверка символа для адреса
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // Доп. символы: - , . / №(211)
    return (is_letter || is_digit || is_space || uc == '-' || uc == ',' || uc == '.' || uc == '/' || uc == 211);
}

bool isValidContactPersonChar(char c) { // Проверка символа для ФИО руководителя
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_space = (uc == ' ');
    // Доп. символы: -
    return (is_letter || is_space || uc == '-');
}

bool isValidCorrTypeChar(char c) { // Проверка символа для вида корреспонденции
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // Доп. символы: - _ . , ( )
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '(' || uc == ')');
}

bool isValidCorrDateChar(char c) { // Проверка символа для даты корреспонденции (разрешаем буквы для "нет данных")
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // Доп. символы: .
    return (is_letter || is_digit || is_space || uc == '.');
}

// --- Функции ввода/вывода и работы с путями ---

// Получение строки с консоли с фильтрацией символов и поддержкой буфера обмена (Ctrl+V)
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction); // Вывод приглашения
    int i = 0;
    buffer[0] = '\0';
    while (true) {
        int key = _getch();
        if (key == 22) { // CTRL V
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        // Используем переданную функцию isValidCharFunc для проверки вставляемых символов
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            if (isValidCharFunc(pszText[j])) { //   Проверка здесь
                                buffer[i++] = pszText[j];
                                printf("%c", pszText[j]);
                            }
                        }
                        GlobalUnlock(hData);
                    }
                }
                CloseClipboard();
            }
        }
        else if (key == 0 || key == 224) { // Проверка на спецсимволы
            if (_kbhit()) {
                _getch();
                continue;
            }
        }
        if (key == 27) { // Esc
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        else if (key == '\r') { // Enter
            buffer[i] = '\0';
            printf("\n");
            return;
        }
        else if (key == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                buffer[i] = '\0';
            }
        }
        else {
            char c = (char)key;
            // Используем переданную функцию isValidCharFunc для проверки вводимых символов
            if (isValidCharFunc(c)) { //   Проверка здесь
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
        }
    }
}


// Нормализация пути: абсолютный, без двойных слешей, с '\' на конце
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path == nullptr || path[0] == '\0') { // Если путь пуст, берем текущую директорию
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "Не удалось определить текущую папку.\n");
            normalized_path[0] = '\0';
        }
        size_t len = strlen(normalized_path); // Добавляем '\' в конец, если нет
        if (len > 0 && normalized_path[len - 1] != '\\' && len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        return;
    }

    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) { // Получаем полный путь
        SAFE_STRCPY(normalized_path, path, normalized_path_size); // Если не вышло, копируем как есть
    }

    int j = 0; // Удаление двойных слешей
    bool last_was_slash = false;
    for (int i = 0; normalized_path[i] != '\0'; ++i) {
        if (normalized_path[i] == '\\') {
            if (!last_was_slash) normalized_path[j++] = normalized_path[i];
            last_was_slash = true;
        }
        else {
            normalized_path[j++] = normalized_path[i];
            last_was_slash = false;
        }
    }
    normalized_path[j] = '\0';

    size_t len = strlen(normalized_path); // Добавляем '\' в конец, если нет
    if (len > 0 && normalized_path[len - 1] != '\\') {
        if (len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        else {
            fprintf(stderr, "Предупреждение: Путь слишком длинный для добавления '\\'.\n");
        }
    }
}

// Выбор и проверка пути к папке
void selectFolderPath() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251); // Установка кодировок
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256];

    // Запрос пути с валидацией
    getLineWithRestrictedChars("Введите путь к папке (или оставьте пустым для текущей, Esc для отмены): ",
        folder_way_new, sizeof(folder_way_new), isValidPathChar);

    if (folder_way_new[0] == '\0' && strlen(folder_way_new) == 0) { // Проверка на отмену (Esc или Enter на пустом поле)
        printf("Выбор пути отменен.\n");
        if (folder_way[0] == '\0') normalizePath("", folder_way, sizeof(folder_way)); // Установить текущий, если ничего не было выбрано
        else printf("Используется последний корректный путь: %s\n", folder_way); // Иначе оставляем старый
        cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
        return;
    }

    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp)); // Нормализация введенного пути

    struct stat path_stat; // Структура для информации о файле/папке
    if (stat(normalized_path_temp, &path_stat) == 0) { // Проверка существования пути
        if (path_stat.st_mode & S_IFDIR) { // Проверка, является ли путь директорией
            char test_file_path[512]; // Попытка создать/удалить файл для проверки прав на запись
            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", normalized_path_temp);
            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) { // Права есть
                fclose(test_file);
                remove(test_file_path);
                printf("Путь к папке выбран: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way)); // Сохраняем путь
            }
            else { // Нет прав на запись
                printf("Ошибка: Нет прав для записи в указанную папку.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
        else { // Путь существует, но это не папка
            printf("Ошибка: Указанный путь не является папкой.\n");
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }
    else { // Путь не существует или недоступен
        printf("Ошибка: Указанный путь не существует или недоступен.\n");
        // Доп. проверка на случай, если stat не сработал из-за '\' на конце
        size_t len = strlen(normalized_path_temp);
        if (len > 1 && normalized_path_temp[len - 1] == '\\') {
            normalized_path_temp[len - 1] = '\0'; // Убираем слеш
            if (stat(normalized_path_temp, &path_stat) == 0 && (path_stat.st_mode & S_IFDIR)) {
                normalized_path_temp[len - 1] = '\\'; // Возвращаем слеш
                printf("Путь к папке выбран (после доп. проверки): %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
            else { // Путь все равно неверный
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
        else { // Путь точно неверный
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }
    cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
}

// --- Функции основной логики ---

// Чтение и вывод инструкций из файла
void readInstructionsFromFile(const string& filename) {
    system("cls"); // Очистка экрана
    FILE* file = fopen(filename.c_str(), "r"); // Открытие файла инструкций
    if (file) { // Если файл открыт
        char line[256];
        while (fgets(line, sizeof(line), file)) { // Чтение и вывод строк
            printf("%s", line);
        }
        fclose(file);
    }
    else { // Ошибка открытия файла
        printf("Не удалось открыть файл с инструкцией: %s\n", filename.c_str());
        char current_path[256]; // Попытка показать текущую папку программы
        if (_getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("Текущая папка программы: %s\n", current_path);
        }
        else {
            printf("Не удалось определить текущую папку программы.\n");
        }
    }
    printf("\nНажмите любую клавишу для продолжения...\n");
    _getch();
    system("cls"); // Очистка экрана
}

// Удаление пробелов в начале и конце строки
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// Обработка данных для одной организации: поиск в файлах и формирование вывода
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile, addrFile;
    vector<Address> addresses;
    vector<Correspondence> correspondences;
    bool addressFound = false, correspondenceFound = false;
    string addrFileError = "", corrFileError = "";

    // Лямбда для вывода в буфер или файл
    auto writeToTarget = [&](const string& str) {
        if (selectiveOutput) outputBuffer.push_back(str);
        else if (outfile) *outfile << str << endl;
        };

    // --- Поиск в файле адресов ---
    addrFile.open(addrFilename);
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            if (!(getline(ss, currentOrg, ';') && getline(ss, address, ';') && getline(ss, contactPerson, ';'))) continue; // Пропуск некорректных строк
            currentOrg = trim(currentOrg); // Убираем пробелы у имени
            if (!currentOrg.empty() && currentOrg == orgName) { // Сравниваем с искомым именем
                address = trim(address);
                contactPerson = trim(contactPerson);
                // Добавляем, заменяя пустые поля на "нет данных"
                addresses.push_back({ currentOrg, address.empty() ? "нет данных" : address, contactPerson.empty() ? "нет данных" : contactPerson });
                addressFound = true;
            }
        }
        addrFile.close();
    }
    else {
        addrFileError = "Ошибка: Не удалось открыть файл адресов: " + addrFilename; // Сохраняем ошибку
    }

    // --- Поиск в файле корреспонденции ---
    corrFile.open(corrFilename);
    if (corrFile.is_open()) {
        string line;
        while (getline(corrFile, line)) {
            stringstream ss(line);
            string type, date, currentOrg;
            if (!(getline(ss, type, ';') && getline(ss, date, ';') && getline(ss, currentOrg, ';'))) continue;
            currentOrg = trim(currentOrg);
            if (!currentOrg.empty() && currentOrg == orgName) {
                type = trim(type);
                date = trim(date);
                correspondences.push_back({ type.empty() ? "нет данных" : type, date.empty() ? "нет данных" : date, currentOrg });
                correspondenceFound = true;
            }
        }
        corrFile.close();
    }
    else {
        corrFileError = "Ошибка: Не удалось открыть файл корреспонденции: " + corrFilename;
    }

    // --- Вывод результата ---
    if (selectiveOutput) { // Режим поиска (в буфер/консоль)
        if (!addrFileError.empty()) writeToTarget(addrFileError); // Вывод ошибок файлов
        if (!corrFileError.empty()) writeToTarget(corrFileError);
        if (addressFound || correspondenceFound || !addrFileError.empty() || !corrFileError.empty()) { // Если есть что выводить
            writeToTarget("Название организации: " + orgName);
            if (addressFound) { // Вывод адресов
                for (const auto& addr : addresses) {
                    writeToTarget("  Адрес: " + addr.address);
                    writeToTarget("  Фамилия руководителя: " + addr.contactPerson);
                }
            }
            else if (addrFileError.empty()) { // Адрес не найден, но ошибки не было
                writeToTarget("  Адрес: нет данных");
                writeToTarget("  Фамилия руководителя: нет данных");
            }
            writeToTarget("  Корреспонденция:"); // Вывод корреспонденции
            if (correspondenceFound) {
                for (const auto& corr : correspondences) {
                    writeToTarget("\t- Вид: " + corr.type + ", Дата: " + corr.date);
                }
            }
            else if (corrFileError.empty()) { // Корреспонденция не найдена, ошибки не было
                writeToTarget("\t- Вид: нет данных, Дата: нет данных");
            }
            writeToTarget("--------------------"); // Разделитель
        }
    }
    else { // Режим отчета (в файл)
        if (printedOrganizations.count(orgName)) return; // Пропуск дубликатов
        printedOrganizations.insert(orgName); // Отмечаем организацию как выведенную

        writeToTarget("Название организации: " + orgName);
        if (addressFound) { // Вывод адресов
            for (const auto& addr : addresses) {
                writeToTarget("  Адрес: " + addr.address);
                writeToTarget("  Фамилия руководителя: " + addr.contactPerson);
            }
        }
        else if (addrFileError.empty()) { // Адрес не найден, ошибки не было
            writeToTarget("  Адрес: нет данных");
            writeToTarget("  Фамилия руководителя: нет данных");
        }
        else {
            writeToTarget("  " + addrFileError); // Вывод ошибки адреса
        }

        if (!corrFileError.empty()) writeToTarget("  " + corrFileError); // Вывод ошибки корреспонденции

        writeToTarget("  Корреспонденция:"); // Вывод корреспонденции
        if (correspondenceFound) {
            for (const auto& corr : correspondences) {
                writeToTarget("\t- Вид: " + corr.type + ", Дата: " + corr.date);
            }
        }
        else if (corrFileError.empty()) { // Корреспонденция не найдена, ошибки не было
            writeToTarget("\t- Вид: нет данных, Дата: нет данных");
        }
        writeToTarget("--------------------"); // Разделитель
    }
}

// Запрос имен файлов у пользователя
pair<string, string> getFilenamesFromUser(const char* folderPath) {
    char corrBase[256], addrBase[256];
    char fullCorr[512], fullAddr[512];
    cout << "\nДля отмены ввода файла оставьте поле пустым и нажмите Enter, или нажмите Esc.\n\n";

    while (true) { // Запрос файла корреспонденции
        getLineWithRestrictedChars("Введите имя файла исходящей корреспонденции (без IC_ и .txt): ", corrBase, sizeof(corrBase), isValidFileNameChar);
        if (strlen(corrBase) == 0) return { "", "" }; // Отмена
        snprintf(fullCorr, sizeof(fullCorr), "%s%s%s%s", folderPath, ocfe, corrBase, file_extension); // Формирование полного пути
        if (!filesystem::exists(fullCorr)) { // Проверка существования
            cerr << "Ошибка: Файл " << fullCorr << " не существует.\nПопробуйте еще раз.\n";
            continue;
        }
        break; // Файл найден
    }

    while (true) { // Запрос файла адресов
        getLineWithRestrictedChars("Введите имя файла адресов организаций (без AO_ и .txt): ", addrBase, sizeof(addrBase), isValidFileNameChar);
        if (strlen(addrBase) == 0) return { "", "" }; // Отмена
        snprintf(fullAddr, sizeof(fullAddr), "%s%s%s%s", folderPath, oa, addrBase, file_extension); // Формирование полного пути
        if (!filesystem::exists(fullAddr)) { // Проверка существования
            cerr << "Ошибка: Файл " << fullAddr << " не существует.\nПопробуйте еще раз.\n";
            continue;
        }
        break; // Файл найден
    }

    return { string(fullCorr), string(fullAddr) }; // Возврат полных путей
}

// Основная функция программы: выбор режима, поиск/обработка данных, вывод
void runProgram(const char* folderPath, const string& correspondenceFilename, const string& addressesFilename, string& outputFilename) {
    while (true) { // Цикл подменю
        cout << "\nВыберите режим работы:\n"
            << "1. Поиск по данным и вывод в консоль\n"
            << "2. Полный вывод в файл\n"
            << "Esc - вернуться в главное меню\n";
        char choice = _getch();
        cout << endl;

        switch (choice) {
        case '1': { // Режим 1: Избирательный поиск
            cout << "\nВыберите поле для поиска:\n"
                << "1. Название организации\n"
                << "2. Адрес организации\n"
                << "3. Фамилия руководителя\n"
                << "4. Вид корреспонденции\n"
                << "5. Дата подготовки\n"
                << "Esc - отмена\n";

            char searchFieldChoice;
            int searchFieldIndex;
            bool (*validatorFunc)(char) = nullptr;
            string searchFieldName = "", fileToScan = "";
            cout << endl;
            while (true) {

                searchFieldChoice = _getch();
                searchFieldIndex = searchFieldChoice - '0';

                if (searchFieldChoice == 27) break; // Отмена

                switch (searchFieldChoice) {
                case '1': validatorFunc = isValidOrgNameChar; searchFieldName = "Название организации"; break;
                case '2': validatorFunc = isValidAddressChar; searchFieldName = "Адрес организации"; fileToScan = addressesFilename; break;
                case '3': validatorFunc = isValidContactPersonChar; searchFieldName = "Фамилия руководителя"; fileToScan = addressesFilename; break;
                case '4': validatorFunc = isValidCorrTypeChar; searchFieldName = "Вид корреспонденции"; fileToScan = correspondenceFilename; break;
                case '5': validatorFunc = isValidCorrDateChar; searchFieldName = "Дата подготовки"; fileToScan = correspondenceFilename; break;
                default: continue;
                }
                break;
            }
            
            char searchQueryBuffer[256];
            getLineWithRestrictedChars(("Введите " + searchFieldName + " для поиска (Esc - отмена): ").c_str(),
                searchQueryBuffer, sizeof(searchQueryBuffer), validatorFunc);
            if (searchQueryBuffer[0] == '\0') break;

            string searchQuery = trim(string(searchQueryBuffer));
            if (searchQuery.empty()) {
                printf("Поисковый запрос не может быть пустым.\n");
                break;
            }

            vector<string> matchingRecords;
            set<string> foundOrganizations;

            // --- Сканирование файла ---
            if (searchFieldIndex == 1) {
                foundOrganizations.insert(searchQuery);
                bool recordFound = false; ifstream tempFile;
                tempFile.open(addressesFilename);
                if (tempFile.is_open()) { string line; while (getline(tempFile, line)) { stringstream ss(line); string org, f1, f2; if (getline(ss, org, ';') && getline(ss, f1, ';') && getline(ss, f2, ';') && trim(org) == searchQuery) { matchingRecords.push_back("Название организации: " + trim(org)); recordFound = true; break; } } tempFile.close(); }
                if (!recordFound) {
                    tempFile.open(correspondenceFilename);
                    if (tempFile.is_open()) { string line; while (getline(tempFile, line)) { stringstream ss(line); string f1, f2, org; if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, org, ';') && trim(org) == searchQuery) { matchingRecords.push_back("Название организации: " + trim(org)); recordFound = true; break; } } tempFile.close(); }
                }
            }
            else {
                ifstream inputFile(fileToScan);
                if (!inputFile.is_open()) {
                    cerr << "Ошибка: Не удалось открыть файл: " << fileToScan << endl;
                }
                else {
                    string line;
                    while (getline(inputFile, line)) {
                        stringstream ss(line); string orgName, f1, f2, fieldToCompare, matchInfo = "";
                        if (fileToScan == addressesFilename) {
                            if (getline(ss, orgName, ';') && getline(ss, f1, ';') && getline(ss, f2, ';')) {
                                orgName = trim(orgName);
                                if (searchFieldIndex == 2) { fieldToCompare = trim(f1); if (!fieldToCompare.empty() && fieldToCompare.find(searchQuery) != string::npos) { foundOrganizations.insert(orgName); matchInfo = "Адрес: " + f1 + ", Название организации: " + orgName; } }
                                else if (searchFieldIndex == 3) { fieldToCompare = trim(f2); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "Фамилия руководителя: " + f2 + ", Название организации: " + orgName; } }
                            }
                        }
                        else {
                            if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, orgName, ';')) {
                                orgName = trim(orgName);
                                if (searchFieldIndex == 4) { fieldToCompare = trim(f1); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "Вид: " + f1 + ", Дата: " + trim(f2) + ", Название организации: " + orgName; } }
                                else if (searchFieldIndex == 5) { fieldToCompare = trim(f2); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "Дата: " + f2 + ", Вид: " + trim(f1) + ", Название организации: " + orgName; } }
                            }
                        }
                        if (!matchInfo.empty()) matchingRecords.push_back(matchInfo);
                    }
                    inputFile.close();
                }
            }

            // --- Вывод результатов поиска (Изменено!) ---
            vector<string> outputBuffer; // Буфер для вывода полной информации
            set<string> dummyPrintedSet; // Пустышка для processOrganization

            // Собираем полную информацию обо всех найденных организациях
            for (const string& orgNameToProcess : foundOrganizations) {
                processOrganization(orgNameToProcess, correspondenceFilename, addressesFilename, true, nullptr, dummyPrintedSet, outputBuffer);
            }

            // Проверяем, найдены ли какие-либо данные
            bool dataFound = !outputBuffer.empty() || !matchingRecords.empty();

            if (!dataFound) {
                cout << "\nПо введенному вами запросу \"" << searchQuery << "\" ничего не найдено." << endl;
            }
            else {
                // Логика вывода в зависимости от поля поиска
                if (searchFieldIndex == 4 || searchFieldIndex == 5) { // Вид или Дата
                    cout << "\nПо введенному вами запросу " << searchFieldName << " \"" << searchQuery << "\" найдены следующие данные:\n" << endl;
                    for (const string& record : matchingRecords) cout << record << endl;
                    cout << "\nСвязанные компании и их данные:\n" << endl;
                    for (const string& line : outputBuffer) cout << line << endl;
                }
                else if (searchFieldIndex == 2 || searchFieldIndex == 3) { // Адрес или Руководитель
                    cout << "\nСвязанные компании и их данные:\n" << endl;
                    for (const string& line : outputBuffer) cout << line << endl;
                }
                else if (searchFieldIndex == 1) { // Название организации
                    // Просто выводим buffer, заголовок "Связанные компании..." не нужен
                    cout << endl; // Отступ перед выводом
                    for (const string& line : outputBuffer) cout << line << endl;
                }
            }
            // Убрана пауза
            break; // Выход из case '1' и возврат к началу цикла while(true)
        }
        case '2': { // Режим 2: Полный вывод в файл
            // Формирование имени файла отчета
            string corrName = filesystem::path(correspondenceFilename).filename().string();
            string addrName = filesystem::path(addressesFilename).filename().string();
            size_t pos = corrName.rfind(".txt"); if (pos != string::npos) corrName = corrName.substr(0, pos);
            pos = addrName.rfind(".txt"); if (pos != string::npos) addrName = addrName.substr(0, pos);
            string baseFilename = string(folderPath) + "Отчет_" + corrName + "_" + addrName;
            string filename = baseFilename + ".txt"; int i = 1;
            while (filesystem::exists(filename)) filename = baseFilename + "(" + to_string(i++) + ").txt";

            try {
                ofstream outfile(filename);
                if (!outfile.is_open()) {
                    cerr << "Ошибка: не удалось создать/открыть файл для записи: " << filename << "\n";
                    break;
                }

                set<string> allUniqueOrgNames;
                set<string> printedOrganizations;
                ifstream fileReader; string line;

                // Чтение имен из файла адресов
                fileReader.open(addressesFilename);
                if (fileReader.is_open()) { while (getline(fileReader, line)) { stringstream ss(line); string org; if (getline(ss, org, ';')) { org = trim(org); if (!org.empty()) allUniqueOrgNames.insert(org); } } fileReader.close(); }
                else { cerr << "Предупреждение: не удалось открыть файл адресов: " << addressesFilename << endl; outfile << "!!! Ошибка чтения файла адресов: " << addressesFilename << " !!!" << endl << endl; }

                // Чтение имен из файла корреспонденции
                fileReader.clear();
                fileReader.open(correspondenceFilename);
                if (fileReader.is_open()) { while (getline(fileReader, line)) { stringstream ss(line); string f1, f2, org; if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, org, ';')) { org = trim(org); if (!org.empty()) allUniqueOrgNames.insert(org); } } fileReader.close(); }
                else { cerr << "Предупреждение: не удалось открыть файл корреспонденции: " << correspondenceFilename << endl; outfile << "!!! Ошибка чтения файла корреспонденции: " << correspondenceFilename << " !!!" << endl << endl; }

                // Обработка и запись в файл
                vector<string> dummyOutputBuffer;
                for (const string& orgNameToProcess : allUniqueOrgNames) {
                    processOrganization(orgNameToProcess, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, dummyOutputBuffer);
                }
                outfile.close();
                cout << "Отчет успешно сформирован и записан в файл: " << filename << ".\n";
            }
            catch (const exception& e) {
                cerr << "Ошибка файловой операции: " << e.what() << endl;
            }
            // Убрана пауза
            break; // Выход из case '2' и возврат к началу цикла while(true)
        }
        case 27: return; // Esc - выход из подменю (возврат в главное меню)
        default:
            cout << "Неверный выбор. Пожалуйста, используйте 1, 2 или Esc." << endl;
            break;
        }
    }
}

// --- Главное меню ---
void menu() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251); // Установка кодировок
    if (folder_way[0] == '\0') normalizePath("", folder_way, sizeof(folder_way)); // Установка текущего пути, если не выбран

    while (true) { // Главный цикл меню
        system("cls"); // Очистка экрана
        // Отображение заголовка и меню
        printf("*************************************************************\n"
            "*    Программа обработки данных корреспонденции и адресов   *\n"
            "*************************************************************\n"
            "* Анализирует файлы:\n"
            "* - Исходящая корреспонденция (Формат: %s*%s)\n"
            "* - Адреса организаций        (Формат: %s*%s)\n"
            "*************************************************************\n\n"
            "Главное меню:\n"
            "1. Начать работу\n"
            "2. Изменить путь к папке с файлами\n"
            "3. Показать инструкцию\n\n"
            "Текущий путь к папке: %s\n\n"
            "Esc - выход из программы\n\n",
            ocfe, file_extension, oa, file_extension, (folder_way[0] == '\0' ? "НЕ ВЫБРАН" : folder_way));

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': { // Начать работу
            printf("\n"); 
            if (folder_way[0] == '\0') { printf("Ошибка: Сначала необходимо выбрать путь к папке (пункт 2).\n"); printf("Нажмите любую клавишу..."); _getch(); break; }
            pair<string, string> filenames = getFilenamesFromUser(folder_way); // Запрос имен файлов
            if (filenames.first == "" || filenames.second == "")
                break;
            string dummyOutput;
            runProgram(folder_way, filenames.first, filenames.second, dummyOutput); // Запуск основной логики
            break; // Возврат в меню после runProgram
        }
        case '2': { // Изменить путь
            printf("\n"); 
            selectFolderPath();
            break;
        }
        case '3': { // Инструкция
            printf("\n"); 
            readInstructionsFromFile(Constants::INSTRUCTIONS_FILE);
            break;
        }
        case 27: { // Esc - выход
            printf("\nЗавершение работы программы.\n");
            return; // Выход из menu() и программы
        }
        default: { // Неверный выбор
            break;
        }
        }
    }
}