//Lab_prog_2.h
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <filesystem> // Для filesystem::exists и filesystem::path
#include <cstdio>
#include <cstring>
#include <conio.h>   // Для _getch, _kbhit
#include <windows.h> // Для SetConsoleCP, SetConsoleOutputCP, OpenClipboard, GetClipboardData, GlobalLock, GlobalUnlock, CloseClipboard
#include <direct.h>  // Для _getcwd
#include <sys/stat.h> // Для stat
#include <cstdlib>   // Для _fullpath
#include <cctype>    // Для isdigit, tolower

#ifdef max
#undef max
#endif

extern char folder_way[256]; // Путь к папке (как из первой программы)
extern const char* file_extension;
extern const char* ocfe; // Префикс для файлов корреспонденции
extern const char* oa;   // Префикс для файлов адресов

// Макрос для безопасного копирования строк
#define SAFE_STRCPY(dest, src, size) strcpy_s(dest, size, src)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt"; // Имя файла с инструкциями.
}

struct Correspondence {
    std::string type;
    std::string date;
    std::string organization;
};

struct Address {
    std::string organization;
    std::string address;
    std::string contactPerson;
};


/** @brief Функция проверки символа для имени файла */
bool isValidFileNameChar(char c);

/** @brief Функция проверки символа для пути */
bool isValidPathChar(char c);

/** @brief Функция для получения строки с консоли с фильтрацией символов */
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char));

/** @brief Функция нормализации пути (упрощенная для Windows) */
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);

/** @brief Функция выбора пути к папке (аналог program_way) */
void selectFolderPath();

// --- Функции из второй программы (основная логика) ---

/** @brief Читает инструкции из файла */
void readInstructionsFromFile(const std::string& filename);

/** @brief Обрабатывает информацию об организации */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile,
    std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);

/** @brief Запрашивает имена файлов у пользователя (адаптировано под C-стиль пути) */
std::pair<std::string, std::string> getFilenamesFromUser(const char* folderPath);

/** @brief Запускает основной процесс программы (адаптировано под C-стиль пути) */
void runProgram(const char* folderPath, const std::string& correspondenceFilename,
    const std::string& addressesFilename, std::string& outputFilename);

/** @brief Отображает главное меню */
void menu();
