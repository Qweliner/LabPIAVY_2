//Lab_prog_2.cpp
#include "Lab_prog_2.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std; // Оставляем для удобства работы с C++ частями

// --- Глобальные переменные (определения) ---
char folder_way[256] = { 0 };       // Путь к папке
const char* file_extension = ".txt";
const char* ocfe = "IC_"; // Префикс для файлов корреспонденции (был outgoing correspondence file extension?)
const char* oa = "AO_";   // Префикс для файлов адресов (был organization addresses?)

// --- Реализации функций из первой программы (ввод и работа с путем) ---

// Функция проверки символа для имени файла
bool isValidFileNameChar(char c) {
    unsigned char uc = (unsigned char)c; // Для корректной работы с кириллицей
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция проверки символа для пути
bool isValidPathChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// Функция для получения строки с консоли с фильтрацией символов
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction);
    int i = 0;
    buffer[0] = '\0'; // Инициализация буфера

    while (true) {
        int key = _getch();
        if (key == 0 || key == 224) { // Расширенные клавиши
            if (_kbhit()) { // Проверяем, есть ли второй байт
                _getch();   // Игнорируем второй байт
                continue;
            }
        }
        if (key == 27) { // ESC
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
        else if (key == 22) { // Ctrl+V
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            // Проверяем вставляемый символ с помощью переданной функции
                            if (isValidCharFunc(pszText[j])) {
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
        else { // Обычный символ
            char c = (char)key;
            // Проверяем символ с помощью переданной функции
            if (isValidCharFunc(c)) {
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
            // Если символ невалидный, ничего не делаем (не печатаем, не добавляем)
        }
    }
}

// Функция нормализации пути (упрощенная для Windows)
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path[0] == '\0') {
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "Не удалось определить текущую папку.\n");
            normalized_path[0] = '\0';
        }
        // Добавляем слеш в конце, если его нет
        size_t len = strlen(normalized_path);
        if (len > 0 && normalized_path[len - 1] != '\\' && len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        return;
    }

    // Используем _fullpath для нормализации под Windows
    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) {
        // Ошибка нормализации (например, путь слишком длинный или недопустимый)
        SAFE_STRCPY(normalized_path, path, normalized_path_size); // Копируем как есть для дальнейшей проверки
    }

    // Дополнительно убираем повторяющиеся слеши, т.к. _fullpath их может оставить
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

    // Добавляем завершающий слеш, если его нет
    size_t len = strlen(normalized_path);
    if (len > 0 && normalized_path[len - 1] != '\\') {
        // Проверяем, есть ли место для слеша и нуль-терминатора
        if (len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        else {
            // Недостаточно места для добавления слеша, оставляем как есть
            fprintf(stderr, "Предупреждение: Путь слишком длинный для добавления завершающего '\\'.\n");
        }
    }
    // Убирать завершающий слеш не нужно по логике первой программы
}

// Функция выбора пути к папке (адаптированный аналог program_way)
void selectFolderPath() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256]; // Временный буфер для нормализации

    // Ввод пути с проверкой символов
    getLineWithRestrictedChars("Введите путь к папке: ", folder_way_new, sizeof(folder_way_new), isValidPathChar);

    if (folder_way_new[0] == '\0') { // Пользователь нажал Esc
        return;
    }

    // Нормализация пути (уже добавляет слеш в конце)
    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp));

    // Проверка существования пути и прав доступа
    struct stat path_stat;
    if (stat(normalized_path_temp, &path_stat) == 0) { // Путь существует
        if (path_stat.st_mode & S_IFDIR) { // Это директория
            // Проверка прав на запись (в стиле C)
            char test_file_path[512];
            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", normalized_path_temp); // Используем путь со слешем

            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) { // Права на запись есть
                fclose(test_file);
                remove(test_file_path); // Удаляем тестовый файл
                printf("Путь к папке выбран: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way)); // Сохраняем путь со слешем
            }
            else { // Нет прав на запись
                printf("Нет прав для доступа к указанной папке. Выберите другую папку или обратитесь к администратору.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
        }
        else { // Путь существует, но это не директория
            printf("Указанный путь не является папкой.\n");
            if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
        }
    }
    else { // Путь не существует
        // Проверяем, возможно, ошибка из-за отсутствия слеша, хотя normalizePath должен его добавить
        size_t len = strlen(normalized_path_temp);
        if (len > 0 && normalized_path_temp[len - 1] == '\\') {
            // Уберем слеш и попробуем еще раз stat, если normalizePath его добавил, а папки нет
            normalized_path_temp[len - 1] = '\0';
            if (stat(normalized_path_temp, &path_stat) != 0 || !(path_stat.st_mode & S_IFDIR)) {
                // Если и без слеша не папка или не существует
                printf("Указанный путь не существует или не является папкой. Проверьте правильность написания.\n");
                if (folder_way[0] != '\0') printf("Используется последний корректный путь: %s\n", folder_way);
            }
            else {
                // Папка существует, но normalizePath зачем-то добавил лишний слеш? Вернем его.
                normalized_path_temp[len - 1] = '\\';
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
    while (getchar() != '\n'); // Очистка буфера ввода после _getch, если используется system("PAUSE>nul") не подходит
}


// --- Реализации функций из второй программы (адаптированные) ---

void readInstructionsFromFile(const string& filename) {
    system("mode con cols=150 lines=36");
    // Используем ifstream для совместимости с остальным кодом C++
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line))
            cout << line << endl;
        file.close();
    }
    else {
        // Используем cerr для вывода ошибок
        cerr << "Не удалось открыть файл инструкции: " << filename << endl;
        cerr << "Пожалуйста, поместите файл " << Constants::INSTRUCTIONS_FILE << " в папку: " << folder_way << "\n";
    }
    cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    // Эта функция остается без изменений, так как она работает с std::string
    // и ее внутренняя логика не затрагивалась запросом.
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

    // Сначала ищем организацию в файле адресов.
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            getline(ss, currentOrg, ';');
            getline(ss, address, ';');
            getline(ss, contactPerson, ';');
            // Убираем пробелы в начале и конце
            size_t first = currentOrg.find_first_not_of(" \t");
            if (string::npos != first)
            {
                size_t last = currentOrg.find_last_not_of(" \t");
                currentOrg = currentOrg.substr(first, (last - first + 1));
            }
            else {
                currentOrg = ""; // Строка состоит только из пробелов
            }
            if (currentOrg == orgName) {
                addresses.push_back({ currentOrg, address, contactPerson });
                orgFoundInAddress = true;
            }
        }
        addrFile.close();
    }
    else {
        writeToBuffer("Не удалось открыть файл адресов: " + addrFilename);
        return;
    }

    if (!orgFoundInAddress && selectiveOutput) {
        writeToBuffer("Название организации \"" + orgName + "\" не найдена в файле адресов.");
        return;
    }

    // Полный вывод
    if (!selectiveOutput)
    {
        if (printedOrganizations.count(orgName)) {
            return;
        }
        printedOrganizations.insert(orgName);
        writeToBuffer("Название организации: " + orgName);

        for (const auto& addr : addresses) {
            writeToBuffer("Адрес: " + addr.address);
            writeToBuffer("Фамилия руководителя: " + addr.contactPerson);
        }

        // Поиск и вывод корреспонденции
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                size_t first = currentOrg.find_first_not_of(" \t");
                if (string::npos != first)
                {
                    size_t last = currentOrg.find_last_not_of(" \t");
                    currentOrg = currentOrg.substr(first, (last - first + 1));
                }
                else {
                    currentOrg = "";
                }
                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\tВид корреспонденции: " + type);
                    writeToBuffer("\tДата подготовки: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\tКорреспонденция не найдена.");
            }
            corrFile.close();
        }
        else {
            writeToBuffer("Не удалось открыть файл корреспонденции: " + corrFilename);
        }
        writeToBuffer("--------------------");
    }

    // Выборочный вывод (поиск)
    if (selectiveOutput && orgFoundInAddress)
    {
        writeToBuffer("Название организации: " + orgName);
        if (!addresses.empty()) {
            writeToBuffer("Адрес: " + addresses[0].address); // Предполагаем, что основной адрес первый
            writeToBuffer("Фамилия руководителя: " + addresses[0].contactPerson);

            if (addresses.size() > 1) {
                writeToBuffer("\nДругие найденные данные компании:\n");
                for (size_t i = 1; i < addresses.size(); ++i) {
                    writeToBuffer("Адрес: " + addresses[i].address);
                    writeToBuffer("Фамилия руководителя: " + addresses[i].contactPerson);
                }
            }
        }
        // Поиск и вывод корреспонденции
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                size_t first = currentOrg.find_first_not_of(" \t");
                if (string::npos != first)
                {
                    size_t last = currentOrg.find_last_not_of(" \t");
                    currentOrg = currentOrg.substr(first, (last - first + 1));
                }
                else {
                    currentOrg = "";
                }

                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\tВид корреспонденции: " + type);
                    writeToBuffer("\tДата подготовки: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\tКорреспонденция не найдена.");
            }
            corrFile.close();
        }
        else {
            writeToBuffer("Не удалось открыть файл корреспонденции: " + corrFilename);
        }
        writeToBuffer("--------------------");
    }
}

pair<string, string> getFilenamesFromUser(const char* folderPath) {
    char correspondenceFilenameBase[256]; // Буфер для базового имени файла
    char addressesFilenameBase[256];
    char fullCorrFilename[512]; // Буфер для полного пути
    char fullAddressesFilename[512];

    while (true) {
        // Используем getLineWithRestrictedChars для ввода имени файла
        getLineWithRestrictedChars("Введите имя файла с исходящей корреспонденцией (без префикса IC_ и расширения .txt) или нажмите Esc для отмены: ",
            correspondenceFilenameBase, sizeof(correspondenceFilenameBase), isValidFileNameChar);
        if (correspondenceFilenameBase[0] == '\0') { // Нажат Esc
            return { "", "" };
        }
        if (strlen(correspondenceFilenameBase) == 0) {
            printf("Имя файла не может быть пустым.\n");
            continue; // Повторить ввод
        }

        // Формируем полный путь в стиле C
        snprintf(fullCorrFilename, sizeof(fullCorrFilename), "%s%s%s%s", folderPath, ocfe, correspondenceFilenameBase, file_extension);

        // Проверяем существование файла, используя C++ filesystem для простоты
        if (!filesystem::exists(fullCorrFilename)) {
            cerr << "Ошибка: Файл " << fullCorrFilename << " не существует.\n";
            cout << "Пожалуйста, попробуйте еще раз.\n";
            continue;
        }
        break; // Имя корректно и файл существует
    }

    while (true) {
        getLineWithRestrictedChars("Введите имя файла с адресами организаций (без префикса AO_ и расширения .txt) или нажмите Esc для отмены: ",
            addressesFilenameBase, sizeof(addressesFilenameBase), isValidFileNameChar);
        if (addressesFilenameBase[0] == '\0') { // Нажат Esc
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
        break; // Имя корректно и файл существует
    }

    // Возвращаем пару std::string, конвертируя C-строки
    return { string(fullCorrFilename), string(fullAddressesFilename) };
}

void runProgram(const char* folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) { // outputFilename не используется, но оставим

    while (true) {
        cout << "\nВыберите режим вывода:\n";
        cout << "1. Поиск по название организации и вывод в консоль\n";
        cout << "2. Полный вывод в файл\n";
        cout << "Esc - вернуться в главное меню\n";
        cout << "\nВаш выбор: ";

        char choice = _getch();
        // Не выводим выбор пользователя сразу, чтобы не мешать вводу
        // cout << choice << endl; // Убрано

        // Простая проверка без цикла while
        if (choice != '1' && choice != '2' && choice != 27) {
            cout << "\nНеверный выбор. Попробуйте снова." << endl;
            continue;
        }
        cout << endl; // Перевод строки после корректного выбора

        switch (choice) {
        case '1': {
            char orgNameBuffer[256]; // Буфер для имени организации
            // Используем getLineWithRestrictedChars для ввода названия организации
            // Используем isValidPathChar, так как название организации может содержать больше символов, чем имя файла
            getLineWithRestrictedChars("Введите название организации для поиска или нажмите Esc для отмены: ",
                orgNameBuffer, sizeof(orgNameBuffer), isValidPathChar);

            if (orgNameBuffer[0] == '\0') { // Нажат Esc
                break; // Вернуться к выбору режима
            }
            if (strlen(orgNameBuffer) == 0) {
                printf("Название организации не может быть пустым.\n");
                break;
            }

            string orgName(orgNameBuffer); // Конвертируем в std::string для processOrganization

            ofstream* outfile = nullptr;
            set<string> printedOrganizations;
            vector<string> outputBuffer;
            processOrganization(orgName, correspondenceFilename, addressesFilename,
                true, outfile, printedOrganizations, outputBuffer);

            // Вывод буфера на консоль
            for (const string& line : outputBuffer) {
                cout << line << endl;
            }
            cout << "\nНажмите любую клавишу для продолжения..." << endl;
            _getch(); // Пауза после вывода
            break; // Вернуться к выбору режима
        }
        case '2': {
            // Логика формирования имени выходного файла и записи остается прежней,
            // так как она использует std::filesystem и std::string,
            // но базовый путь берем из C-строки folderPath.

            string corrFileNameOnly = filesystem::path(correspondenceFilename).filename().string();
            string addrFileNameOnly = filesystem::path(addressesFilename).filename().string();

            // Убираем расширение .txt
            size_t pos = corrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                corrFileNameOnly = corrFileNameOnly.substr(0, pos);
            }
            pos = addrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                addrFileNameOnly = addrFileNameOnly.substr(0, pos);
            }

            // Формируем имя выходного файла (с путём из C-строки)
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
                    break; // Вернуться к выбору режима
                }

                set<string> printedOrganizations;
                vector<string> outputBuffer; // Не используется для файлового вывода напрямую

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
                            orgName = "";
                        }

                        // Вызываем processOrganization для каждой уникальной организации
                        if (!orgName.empty() && printedOrganizations.find(orgName) == printedOrganizations.end()) {
                            processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);
                        }
                    }
                    addrFile.close();
                }
                else {
                    cerr << "Ошибка: не удалось открыть файл адресов: " << addressesFilename << ".\n";
                    outfile.close(); // Закрыть выходной файл, если он был открыт
                    cout << "Нажмите любую клавишу для продолжения..." << endl; _getch();
                    break; // Вернуться к выбору режима
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
            break; // Вернуться к выбору режима
        }
        case 27: // Esc
            return;  // Выход из runProgram в главное меню
        }
    }
}

void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Автоматическое определение пути при запуске (стиль C)
    if (folder_way[0] == '\0') {
        normalizePath("", folder_way, sizeof(folder_way)); // Получаем текущий путь и нормализуем
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
        case '1': { // "Начать работу"
            if (folder_way[0] == '\0') {
                printf("Сначала выберите путь к папке (пункт 2).\n");
                printf("Нажмите любую клавишу для продолжения..."); _getch();
                break;
            }
            // Получаем имена файлов от пользователя (функция адаптирована под C-путь)
            pair<string, string> filenames = getFilenamesFromUser(folder_way);
            if (filenames.first.empty() && filenames.second.empty()) {
                break; // Если пользователь нажал Esc в getFilenamesFromUser
            }
            string outputFilenameDummy; // Переменная больше не нужна, но оставим для сигнатуры
            // Вызываем runProgram (функция адаптирована под C-путь)
            runProgram(folder_way, filenames.first, filenames.second, outputFilenameDummy);
            break;
        }
        case '2': { // "Изменить путь к папке"
            selectFolderPath(); // Используем адаптированную функцию из первой программы
            break;
        }
        case '3': { // Инструкция
            // Формируем путь к файлу инструкции, используя C-путь
            string instructionsFilePath = string(folder_way) + Constants::INSTRUCTIONS_FILE;
            readInstructionsFromFile(instructionsFilePath);
            break;
        }
        case 27: // Esc - выход
            return;

        default:
            // Игнорируем неверный ввод
            break;
        }
    }
}