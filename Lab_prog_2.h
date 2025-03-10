#pragma once

#include <algorithm>
#include <cctype>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <Windows.h>
#include <filesystem>

#ifdef max  // Нужно для Visual Studio, чтобы не было конфликтов с <limits>
#undef max
#endif

// Глобальная переменная для хранения текущего пути к папке с данными.
extern std::string currentFolderPath;

// Объявление констант.
namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt"; // Имя файла с инструкциями.
}

// Структура для хранения данных о корреспонденции.
struct Correspondence {
    std::string type;       // Тип корреспонденции.
    std::string date;       // Дата корреспонденции.
    std::string organization; // Название организации.
};

// Структура для хранения данных об адресе организации.
struct Address {
    std::string organization; // Название организации.
    std::string address;      // Адрес организации.
    std::string contactPerson;  // Контактное лицо.
};

/** @brief Проверяет, является ли год високосным. */
bool is_leap(int year);

/** @brief Проверяет допустимость ФИО. */
bool isValidFio(const std::string& fio);

/** @brief Проверяет допустимость формата даты. */
bool isValidDate(const std::string& dateStr);

/** @brief Проверяет допустимость имени файла. */
bool isValidFileName(const std::string& fileName);

/** @brief Читает инструкции из файла. */
void readInstructionsFromFile(const std::string& filename);

/** @brief Обрабатывает информацию об организации.
 *  Находит и выводит данные организации из файлов корреспонденции и адресов.
 */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile,
    std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);

/** @brief Запрашивает имена файлов у пользователя. */
std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath);

/** @brief Запускает основной процесс программы. */
void runProgram(const std::string& folderPath, const std::string& correspondenceFilename,
    const std::string& addressesFilename, std::string& outputFilename);

/** @brief Отображает главное меню. */
void menu();

/** @brief Нормализует путь к файлу, делая его абсолютным. */
std::string normalizePath(const std::string& path);

/** @brief Выполняет чтение строки из консоли с поддержкой Esc и Backspace, а также фильтрацией.
 *  @param instruction Сообщение, отображаемое пользователю перед вводом.
 *  @return Введенная строка, или пустая строка, если был нажат Esc.
 */
std::string getLineWithEsc(const std::string& instruction);