//Lab_prog_2.cpp
#include "Lab_prog_2.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

// --- Глобальные переменные (определения) ---
char folder_way[256] = { 0 };
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// --- Реализации функций проверки символов ---

// Функция проверки символа для имени файла
bool isValidFileNameChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция проверки символа для пути
bool isValidPathChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция проверки символа для названия организации (из первой программы)
bool isValidOrgNameChar(char c) { // <-- РЕАЛИЗАЦИЯ НОВОЙ ФУНКЦИИ
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // Латиница + Кириллица (грубо)
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // Точная логика из openFileForAppend для поля "Название организации"
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'); // №(211), «(171), »(187)
}

// --- Реализации функций ввода и работы с путем ---

// Функция для получения строки с консоли с фильтрацией символов
// Принимает указатель на функцию проверки isValidCharFunc
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction);
    int i = 0;
    buffer[0] = '\0';

    while (true) {
        int key = _getch();
        if (key == 0 || key == 224) {
            if (_kbhit()) {
                _getch();
                continue;
            }
        }
        if (key == 27) {
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        else if (key == '\r') {
            buffer[i] = '\0';
            printf("\n");
            return;
        }
        else if (key == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
                buffer[i] = '\0';
            }
        }
        else if (key == 22) {
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        // Используем переданную функцию isValidCharFunc для проверки вставляемых символов
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            if (isValidCharFunc(pszText[j])) { // <--- Проверка здесь
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
        else {
            char c = (char)key;
            // Используем переданную функцию isValidCharFunc для проверки вводимых символов
            if (isValidCharFunc(c)) { // <--- Проверка здесь
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
        }
    }
}

// Функция нормализации пути (без изменений)
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path[0] == '\0') {
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "Не удалось определить текущую папку.\n");
            normalized_path[0] = '\0';
        }
        size_t len = strlen(normalized_path);
        if (len > 0 && normalized_path[len - 1] != '\\' && len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        return;
    }

    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) {
        SAFE_STRCPY(normalized_path, path, normalized_path_size);
    }

    int j = 0;
    bool last_was_slash = false;
    for (int i = 0; normalized_path[i] != '\0'; ++i) {
        if (normalized_path[i] == '\\') {
            if (!last_was_slash) {
                normalized_path[j++] = normalized_path[i];
            }
            last_was_slash = true;
        }
        else {
            normalized_path[j++] = normalized_path[i];
            last_was_slash = false;
        }
    }
    normalized_path[j] = '\0';

    size_t len = strlen(normalized_path);
    if (len > 0 && normalized_path[len - 1] != '\\') {
        if (len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        else {
            fprintf(stderr, "Предупреждение: Путь слишком длинный для добавления завершающего '\\'.\n");
        }
    }
}

// Функция выбора пути к папке
void selectFolderPath() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256];

    // Используем isValidPathChar для проверки пути
    getLineWithRestrictedChars("Введите путь к папке: ", folder_way_new, sizeof(folder_way_new), isValidPathChar); // <--- Передача функции

    if (folder_way_new[0] == '\0') {
        return;
    }

    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp));

    struct stat path_stat;
    if (stat(normalized_path_temp, &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) {
            char test_file_path[512];
            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", normalized_path_temp);

            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) {
                fclose(test_file);
                remove(test_file_path);
                printf("Путь к папке выбран: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
            else {
                printf("Нет прав для доступа к указанной папке. Выберите другую папку или обратитесь к администратору.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
        else {
            printf("Указанный путь не является папкой.\n");
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }
    else {
        size_t len = strlen(normalized_path_temp);
        if (len > 0 && normalized_path_temp[len - 1] == '\\') {
            normalized_path_temp[len - 1] = '\0';
            if (stat(normalized_path_temp, &path_stat) != 0 || !(path_stat.st_mode & S_IFDIR)) {
                printf("Указанный путь не существует или не является папкой. Проверьте правильность написания.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
            else {
                normalized_path_temp[len - 1] = '\\'; // Возвращаем слеш обратно
                printf("Путь к папке выбран: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
        }
        else {
            printf("Указанный путь не существует. Проверьте правильность написания.\n");
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }

    printf("Для продолжения нажмите Enter.");
    // Очистка буфера ввода после _getch
    int c; while ((c = getchar()) != '\n' && c != EOF);
}

// --- Реализации функций из второй программы (адаптированные) ---

// readInstructionsFromFile (без изменений)
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
        cerr << "Пожалуйста, поместите файл " << Constants::INSTRUCTIONS_FILE << " в папку: " << folder_way << "\n";
    }
    cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    _getch();
    system("mode con cols=120 lines=30");
}


// processOrganization (без изменений)
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile;
    ifstream addrFile;

    vector<Address> addresses;
    vector<Correspondence> correspondences;
    bool addressFound = false;
    bool correspondenceFound = false;
    string addrFileError = "";
    string corrFileError = "";

    auto writeToTarget = [&](const string& str) {
        if (selectiveOutput) {
            outputBuffer.push_back(str);
        }
        else if (outfile) {
            *outfile << str << endl;
        }
        };

    // --- Этап 1: Поиск в файле адресов ---
    addrFile.open(addrFilename);
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            if (!(getline(ss, currentOrg, ';') && getline(ss, address, ';') && getline(ss, contactPerson, ';'))) {
                continue;
            }

            size_t first = currentOrg.find_first_not_of(" \t");
            if (string::npos != first) {
                size_t last = currentOrg.find_last_not_of(" \t");
                currentOrg = currentOrg.substr(first, (last - first + 1));
            }
            else {
                currentOrg.clear();
            }

            if (!currentOrg.empty() && currentOrg == orgName) {
                first = address.find_first_not_of(" \t"); address = (first == string::npos) ? "" : address.substr(first);
                first = contactPerson.find_first_not_of(" \t"); contactPerson = (first == string::npos) ? "" : contactPerson.substr(first);
                if (!address.empty() && address.back() == ';') address.pop_back();
                if (!contactPerson.empty() && contactPerson.back() == ';') contactPerson.pop_back();

                addresses.push_back({ currentOrg, address.empty() ? "нет данных" : address, contactPerson.empty() ? "нет данных" : contactPerson }); // Сразу ставим заглушку при необходимости
                addressFound = true;
            }
        }
        addrFile.close();
    }
    else {
        addrFileError = "Не удалось открыть файл адресов: " + addrFilename;
    }

    // --- Этап 2: Поиск в файле корреспонденции ---
    corrFile.open(corrFilename);
    if (corrFile.is_open()) {
        string line;
        while (getline(corrFile, line)) {
            stringstream ss(line);
            string type, date, currentOrg;
            if (!(getline(ss, type, ';') && getline(ss, date, ';') && getline(ss, currentOrg, ';'))) {
                continue;
            }

            size_t first = currentOrg.find_first_not_of(" \t");
            if (string::npos != first) {
                size_t last = currentOrg.find_last_not_of(" \t");
                currentOrg = currentOrg.substr(first, (last - first + 1));
            }
            else {
                currentOrg.clear();
            }

            if (!currentOrg.empty() && currentOrg == orgName) {
                first = type.find_first_not_of(" \t"); type = (first == string::npos) ? "" : type.substr(first);
                first = date.find_first_not_of(" \t"); date = (first == string::npos) ? "" : date.substr(first);
                if (!type.empty() && type.back() == ';') type.pop_back();
                if (!date.empty() && date.back() == ';') date.pop_back();

                correspondences.push_back({ type.empty() ? "нет данных" : type, date.empty() ? "нет данных" : date, currentOrg }); // Сразу ставим заглушку при необходимости
                correspondenceFound = true;
            }
        }
        corrFile.close();
    }
    else {
        corrFileError = "Не удалось открыть файл корреспонденции: " + corrFilename;
    }

    // --- Этап 3: Вывод результата ---

    if (selectiveOutput) { // Режим поиска (в консоль)
        // Сначала выводим ошибки открытия файлов, если были
        if (!addrFileError.empty()) writeToTarget(addrFileError);
        if (!corrFileError.empty()) writeToTarget(corrFileError);

        // Проверяем, есть ли хоть какая-то информация или ошибки для вывода
        if (!addressFound && !correspondenceFound && addrFileError.empty() && corrFileError.empty()) {
            // Ничего не найдено и ошибок не было
            writeToTarget("Организация \"" + orgName + "\" не найдена ни в одном из файлов.");
        }
        else {
            // Выводим найденную информацию или заглушки
            writeToTarget("Название организации: " + orgName);

            // Вывод информации об адресе (или заглушек)
            if (addressFound) {
                for (const auto& addr : addresses) { // Могут быть дубликаты, выводим все найденные
                    writeToTarget("  Адрес: " + addr.address); // Заглушка уже внутри структуры
                    writeToTarget("  Фамилия руководителя: " + addr.contactPerson); // Заглушка уже внутри структуры
                }
            }
            else if (addrFileError.empty()) { // Файл читался, но совпадений нет
                writeToTarget("  Адрес: нет данных");
                writeToTarget("  Фамилия руководителя: нет данных");
            }
            // Если была ошибка addrFileError, она уже выведена

            // Вывод информации о корреспонденции (или заглушек)
            writeToTarget("  Корреспонденция:"); // Всегда выводим заголовок
            if (correspondenceFound) {
                for (const auto& corr : correspondences) { // Могут быть дубликаты, выводим все найденные
                    writeToTarget("\t- Вид: " + corr.type + ", Дата: " + corr.date); // Заглушки уже внутри структуры
                }
            }
            else if (corrFileError.empty()) { // Файл читался, но совпадений нет
                writeToTarget("\t- Вид: нет данных, Дата: нет данных");
            }
            // Если была ошибка corrFileError, она уже выведена

            writeToTarget("--------------------");
        }
    }
    else { // Режим полного вывода (в файл)
        // Логика полного вывода остается без изменений, она уже должна корректно
        // обрабатывать собранные данные (включая ошибки и найденную информацию).
        // Единственное - добавим проверку на пустые поля при выводе, чтобы соответствовать поиску.
        if (printedOrganizations.count(orgName)) {
            return;
        }

        printedOrganizations.insert(orgName);
        writeToTarget("Название организации: " + orgName);

        if (addressFound) {
            for (const auto& addr : addresses) {
                writeToTarget("  Адрес: " + addr.address); // Заглушка уже внутри
                writeToTarget("  Фамилия руководителя: " + addr.contactPerson); // Заглушка уже внутри
            }
        }
        else if (addrFileError.empty()) {
            writeToTarget("  Адрес: нет данных");
            writeToTarget("  Фамилия руководителя: нет данных");
        }
        else {
            // Выводим ошибку, если адрес не найден из-за нее
            writeToTarget("  " + addrFileError);
        }


        if (!corrFileError.empty()) {
            writeToTarget("  " + corrFileError);
        }
        writeToTarget("  Корреспонденция:"); // Выводим заголовок
        if (correspondenceFound) {
            for (const auto& corr : correspondences) {
                writeToTarget("\t- Вид: " + corr.type + ", Дата: " + corr.date); // Заглушки уже внутри
            }
        }
        else if (corrFileError.empty()) {
            writeToTarget("\t- Вид: нет данных, Дата: нет данных");
        }
        // Если была ошибка corrFileError, она уже выведена

        writeToTarget("--------------------");
    }
}

// getFilenamesFromUser (адаптирован для передачи нужной функции проверки)
pair<string, string> getFilenamesFromUser(const char* folderPath) {
    char correspondenceFilenameBase[256];
    char addressesFilenameBase[256];
    char fullCorrFilename[512];
    char fullAddressesFilename[512];

    while (true) {
        // Используем isValidFileNameChar для имени файла
        getLineWithRestrictedChars("Введите имя файла с исходящей корреспонденцией (без префикса IC_ и расширения .txt) или нажмите Esc для отмены: ",
            correspondenceFilenameBase, sizeof(correspondenceFilenameBase), isValidFileNameChar); // <--- Передача функции
        if (correspondenceFilenameBase[0] == '\0') {
            return { "", "" };
        }
        if (strlen(correspondenceFilenameBase) == 0) {
            printf("Имя файла не может быть пустым.\n");
            continue;
        }

        snprintf(fullCorrFilename, sizeof(fullCorrFilename), "%s%s%s%s", folderPath, ocfe, correspondenceFilenameBase, file_extension);

        if (!filesystem::exists(fullCorrFilename)) {
            cerr << "Ошибка: Файл " << fullCorrFilename << " не существует.\n";
            cout << "Пожалуйста, попробуйте еще раз.\n";
            continue;
        }
        break;
    }

    while (true) {
        // Используем isValidFileNameChar для имени файла
        getLineWithRestrictedChars("Введите имя файла с адресами организаций (без префикса AO_ и расширения .txt) или нажмите Esc для отмены: ",
            addressesFilenameBase, sizeof(addressesFilenameBase), isValidFileNameChar); // <--- Передача функции
        if (addressesFilenameBase[0] == '\0') {
            return { "", "" };
        }
        if (strlen(addressesFilenameBase) == 0) {
            printf("Имя файла не может быть пустым.\n");
            continue;
        }

        snprintf(fullAddressesFilename, sizeof(fullAddressesFilename), "%s%s%s%s", folderPath, oa, addressesFilenameBase, file_extension);

        if (!filesystem::exists(fullAddressesFilename)) {
            cerr << "Ошибка: Файл " << fullAddressesFilename << " не существует.\n";
            cout << "Пожалуйста, попробуйте еще раз.\n";
            continue;
        }
        break;
    }

    return { string(fullCorrFilename), string(fullAddressesFilename) };
}

// runProgram (адаптирован для передачи нужной функции проверки)
void runProgram(const char* folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) {
        cout << "\nВыберите режим вывода:\n";
        cout << "1. Поиск по название организации и вывод в консоль\n";
        cout << "2. Полный вывод в файл\n";
        cout << "Esc - вернуться в главное меню\n";
        cout << "\nВаш выбор: ";

        char choice = _getch();
        if (choice != '1' && choice != '2' && choice != 27) {
            cout << "\nНеверный выбор. Попробуйте снова." << endl;
            continue;
        }
        cout << endl;

        switch (choice) {
        case '1': {
            char orgNameBuffer[256];
            // Используем isValidOrgNameChar для названия организации
            getLineWithRestrictedChars("Введите название организации для поиска или нажмите Esc для отмены: ",
                orgNameBuffer, sizeof(orgNameBuffer), isValidOrgNameChar); // <--- Передача функции

            if (orgNameBuffer[0] == '\0') {
                break;
            }
            if (strlen(orgNameBuffer) == 0) {
                printf("Название организации не может быть пустым.\n");
                cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
                break;
            }

            string orgName(orgNameBuffer);

            ofstream* outfile = nullptr;
            set<string> printedOrganizations;
            vector<string> outputBuffer;
            processOrganization(orgName, correspondenceFilename, addressesFilename,
                true, outfile, printedOrganizations, outputBuffer);

            for (const string& line : outputBuffer) {
                cout << line << endl;
            }
            cout << "\nНажмите любую клавишу для продолжения..." << endl;
            _getch();
            break;
        }
        case '2': {
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

            string baseFilenameStr = string(folderPath) + "Отчет_" + corrFileNameOnly + "_" + addrFileNameOnly;
            string filename = baseFilenameStr + ".txt";
            int i = 1;
            while (filesystem::exists(filename)) {
                filename = baseFilenameStr + "(" + to_string(i++) + ")" + ".txt";
            }

            try {
                ofstream outfile(filename);
                if (!outfile.is_open()) {
                    cerr << "Ошибка: не удалось открыть файл для записи: " << filename << "\n";
                    cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
                    break;
                }

                set<string> printedOrganizations;
                vector<string> outputBuffer; // Не используется

                ifstream addrFile(addressesFilename);
                if (addrFile.is_open()) {
                    string line;
                    while (getline(addrFile, line)) {
                        stringstream ss(line);
                        string orgName, address, contactPerson;
                        getline(ss, orgName, ';');
                        getline(ss, address, ';');
                        getline(ss, contactPerson, ';');

                        size_t first = orgName.find_first_not_of(" \t");
                        if (string::npos != first)
                        {
                            size_t last = orgName.find_last_not_of(" \t");
                            orgName = orgName.substr(first, (last - first + 1));
                        }
                        else {
                            orgName.clear();
                        }

                        if (!orgName.empty() && printedOrganizations.find(orgName) == printedOrganizations.end()) {
                            processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);
                        }
                    }
                    addrFile.close();
                }
                else {
                    cerr << "Ошибка: не удалось открыть файл адресов: " << addressesFilename << ".\n";
                    outfile.close();
                    cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
                    break;
                }

                outfile.close();
                cout << "Информация успешно записана в файл " << filename << ".\n";
                cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();

            }
            catch (const exception& e)
            {
                cerr << "Ошибка: не удалось создать или записать файл.\n";
                cerr << e.what() << endl;
                cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
            }
            break;
        }
        case 27:
            return;
        }
    }
}

// menu (без изменений в логике вызовов)
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (folder_way[0] == '\0') {
        normalizePath("", folder_way, sizeof(folder_way));
    }

    printf("\nПрограмма для обработки данных о корреспонденции и адресах организаций.\n");
    printf("Она позволяет искать информацию об организациях и их корреспонденции, а также формировать отчеты.\n");

    while (true) {
        system("cls");

        printf("*************************************************************\n");
        printf("* Программа для обработки данных корреспонденции и адресов  *\n");
        printf("* Программа позволяет искать и обрабатывать информацию об   *\n");
        printf("* организациях на основе двух файлов -                      *\n");
        printf("* Исходящей корреспонденции (%s***%s)                    *\n", ocfe, file_extension);
        printf("* Адресов организаций (%s***%s)                          *\n", oa, file_extension);
        printf("*************************************************************\n\n");

        printf("Главное меню:\n");
        printf("1. Начать работу\n");
        printf("2. Изменить путь к папке\n");
        printf("3. Инструкция\n\n");
        printf("Текущий путь: %s\n\n", folder_way);
        printf("Esc - выход\n\n");

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': {
            if (folder_way[0] == '\0') {
                printf("Сначала выберите путь к папке (пункт 2).\n");
                printf("Нажмите любую клавишу для продолжения..."); _getch();
                break;
            }
            pair<string, string> filenames = getFilenamesFromUser(folder_way);
            if (filenames.first.empty() && filenames.second.empty()) {
                break;
            }
            string outputFilenameDummy;
            runProgram(folder_way, filenames.first, filenames.second, outputFilenameDummy);
            break;
        }
        case '2': {
            selectFolderPath();
            break;
        }
        case '3': {
            string instructionsFilePath = string(folder_way) + Constants::INSTRUCTIONS_FILE;
            readInstructionsFromFile(instructionsFilePath);
            break;
        }
        case 27:
            return;

        default:
            break;
        }
    }
}