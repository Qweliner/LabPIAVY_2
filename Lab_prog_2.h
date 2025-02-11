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

#ifdef max
#undef max
#endif

//using namespace std; //  Удалено из заголовочного файла

// Глобальная переменная для хранения текущего пути к папке с данными
extern std::string currentFolderPath;

// Объявление констант
namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt"; // Имя файла с инструкциями
    const int MAX_MENU_CHOICE = 3;       // Максимальный выбор в меню
} // namespace Constants

// Структуры данных
struct Correspondence {
    std::string type;       // Тип корреспонденции
    std::string date;       // Дата корреспонденции
    std::string organization; // Название организации
};

struct Address {
    std::string organization; // Название организации
    std::string address;      // Адрес организации
    std::string contactPerson;  // Контактное лицо
};

/** @brief Выводит табуляцию. */
void tabul(int x);

/** @brief Определяет, является ли год високосным. */
bool is_leap(int year);

/** @brief Проверяет допустимость ФИО. */
bool isValidFio(const std::string& fio);

/** @brief Проверяет допустимость формата даты. */
bool isValidDate(const std::string& dateStr);

/** @brief Проверяет допустимость имени файла. */
bool isValidFileName(const std::string& fileName);

/** @brief Читает инструкции из файла. */
void readInstructionsFromFile(const std::string& filename);

/** @brief Обрабатывает информацию об организации. */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput,
    std::ofstream* outfile,
    std::set<std::string>& printedOrganizations);

/** @brief Запрашивает имена файлов у пользователя. */
std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath);

/** @brief Запускает основной процесс программы. */
void runProgram(std::string& folderPath, std::string& correspondenceFilename,
    std::string& addressesFilename, std::string& outputFilename);

/** @brief Отображает главное меню. */
void menu();