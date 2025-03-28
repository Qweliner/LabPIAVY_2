//Lab_prog_2.h
#pragma once // Директива препроцессора для предотвращения повторного включения этого файла

// Подключение необходимых стандартных библиотек C++
#include <iostream>  // Для потоков ввода/вывода (cin, cout)
#include <fstream>   // Для работы с файловыми потоками (ifstream, ofstream)
#include <string>    // Для работы со строками (std::string)
#include <vector>    // Для использования динамических массивов (std::vector)
#include <set>       // Для использования множеств (хранение уникальных элементов)
#include <sstream>   // Для работы со строковыми потоками (разбор строк)
#include <algorithm> // Для стандартных алгоритмов (например, std::find)
#include <filesystem>// Для работы с файловой системой (проверка существования файлов, путей)
#include <cstdio>    // Для стандартного ввода/вывода в стиле C (printf, fopen, ...)
#include <cstring>   // Для функций работы со строками в стиле C (strcpy, strlen, ...)
#include <conio.h>   // Для консольного ввода/вывода без буферизации (_getch, _kbhit)
#include <windows.h> // Для функций Windows API (работа с консолью, буфером обмена)
#include <direct.h>  // Для функций работы с директориями (_getcwd)
#include <sys/stat.h>// Для получения информации о файлах (stat)
#include <cstdlib>   // Для общих утилит C (например, _fullpath)
#include <cctype>    // Для функций проверки символов (isdigit, tolower)

// Если макрос max определен, отменяем его определение, чтобы избежать конфликтов с std::max
#ifdef max
#undef max
#endif

// --- Глобальные переменные и константы ---

// Глобальная переменная для хранения пути к рабочей папке (C-style строка)
extern char folder_way[256];
// Стандартное расширение для используемых файлов данных
extern const char* file_extension;
// Префикс для файлов с исходящей корреспонденцией
extern const char* ocfe; // Incoming Correspondence File Extension (или Outgoing?) - комментарий изначальный, возможно не точен, но отражает суть
// Префикс для файлов с адресами организаций
extern const char* oa;   // Organization Addresses

// Макрос для безопасного копирования строк в стиле C (использует strcpy_s)
#define SAFE_STRCPY(dest, src, size) strcpy_s(dest, size, src)

// Определение максимальной длины пути, если не определено ранее (стандартное для Windows)
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

// Пространство имен для констант
namespace Constants {
    // Имя файла с инструкциями
    const std::string INSTRUCTIONS_FILE = "instructions.txt";
}

// --- Структуры данных ---

// Структура для хранения информации об одной записи корреспонденции
struct Correspondence {
    std::string type;         // Вид корреспонденции
    std::string date;         // Дата
    std::string organization; // Название организации
};

// Структура для хранения информации об одной записи адреса
struct Address {
    std::string organization;  // Название организации
    std::string address;       // Адрес
    std::string contactPerson; // Контактное лицо (руководитель)
};


// --- Прототипы функций ---

/** @brief Функция проверки символа на допустимость для использования в имени файла */
bool isValidFileNameChar(char c);

/** @brief Функция проверки символа на допустимость для использования в пути к файлу/папке */
bool isValidPathChar(char c);

/** @brief Функция проверки символа на допустимость для использования в названии организации */
bool isValidOrgNameChar(char c);

/** @brief Функция проверки символа на допустимость для использования в адресе организации */
bool isValidAddressChar(char c); // Правка 2

/** @brief Функция проверки символа на допустимость для использования в ФИО руководителя */
bool isValidContactPersonChar(char c); // Правка 2

/** @brief Функция проверки символа на допустимость для использования в виде корреспонденции */
bool isValidCorrTypeChar(char c); // Правка 2

/** @brief Функция проверки символа на допустимость для использования в дате корреспонденции */
bool isValidCorrDateChar(char c); // Правка 2

/**
 * @brief Функция для получения строки с консоли с фильтрацией вводимых символов.
 * @param instruction Текст приглашения для ввода.
 * @param buffer Буфер для сохранения введенной строки.
 * @param buffer_size Размер буфера.
 * @param isValidCharFunc Указатель на функцию, проверяющую допустимость символа.
 */
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char));

/**
 * @brief Функция нормализации пути (преобразование в абсолютный путь, удаление лишних слешей, добавление слеша в конце для папки).
 * @param path Исходный путь.
 * @param normalized_path Буфер для сохранения нормализованного пути.
 * @param normalized_path_size Размер буфера.
 */
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);

/** @brief Функция для выбора и проверки пути к рабочей папке */
void selectFolderPath();

// --- Функции основной логики ---

/** @brief Читает и выводит инструкции из файла Constants::INSTRUCTIONS_FILE */
void readInstructionsFromFile(const std::string& filename);

/**
 * @brief Обрабатывает информацию для одной организации: ищет данные в файлах адресов и корреспонденции и выводит/сохраняет результат.
 * @param orgName Название организации для поиска.
 * @param corrFilename Путь к файлу корреспонденции.
 * @param addrFilename Путь к файлу адресов.
 * @param selectiveOutput Флаг: true - вывод в буфер (для поиска), false - вывод в файл (для отчета).
 * @param outfile Указатель на файловый поток для записи (используется, если selectiveOutput = false).
 * @param printedOrganizations Множество для отслеживания уже выведенных организаций (в режиме отчета).
 * @param outputBuffer Вектор для сбора строк вывода (используется, если selectiveOutput = true).
 */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile,
    std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);

/**
 * @brief Запрашивает у пользователя базовые имена файлов корреспонденции и адресов, проверяет их существование и формирует полные пути.
 * @param folderPath Путь к папке, где искать файлы.
 * @return Пара строк: полный путь к файлу корреспонденции и полный путь к файлу адресов. Возвращает пустые строки при отмене.
 */
std::pair<std::string, std::string> getFilenamesFromUser(const char* folderPath);

/**
 * @brief Запускает основной процесс программы: выбор режима (поиск/отчет), обработка данных и вывод результата.
 * @param folderPath Путь к рабочей папке.
 * @param correspondenceFilename Путь к файлу корреспонденции.
 * @param addressesFilename Путь к файлу адресов.
 * @param outputFilename (Не используется в текущей реализации, но оставлен в сигнатуре) Строка для имени выходного файла.
 */
void runProgram(const char* folderPath, const std::string& correspondenceFilename,
    const std::string& addressesFilename, std::string& outputFilename);

/** @brief Отображает главное меню программы и управляет основным циклом взаимодействия с пользователем. */
void menu();