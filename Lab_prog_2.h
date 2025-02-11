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

//using namespace std; //  ������� �� ������������� �����

// ���������� ���������� ��� �������� �������� ���� � ����� � �������
extern std::string currentFolderPath;

// ���������� ��������
namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt"; // ��� ����� � ������������
    const int MAX_MENU_CHOICE = 3;       // ������������ ����� � ����
} // namespace Constants

// ��������� ������
struct Correspondence {
    std::string type;       // ��� ���������������
    std::string date;       // ���� ���������������
    std::string organization; // �������� �����������
};

struct Address {
    std::string organization; // �������� �����������
    std::string address;      // ����� �����������
    std::string contactPerson;  // ���������� ����
};

/** @brief ������� ���������. */
void tabul(int x);

/** @brief ����������, �������� �� ��� ����������. */
bool is_leap(int year);

/** @brief ��������� ������������ ���. */
bool isValidFio(const std::string& fio);

/** @brief ��������� ������������ ������� ����. */
bool isValidDate(const std::string& dateStr);

/** @brief ��������� ������������ ����� �����. */
bool isValidFileName(const std::string& fileName);

/** @brief ������ ���������� �� �����. */
void readInstructionsFromFile(const std::string& filename);

/** @brief ������������ ���������� �� �����������. */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput,
    std::ofstream* outfile,
    std::set<std::string>& printedOrganizations);

/** @brief ����������� ����� ������ � ������������. */
std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath);

/** @brief ��������� �������� ������� ���������. */
void runProgram(std::string& folderPath, std::string& correspondenceFilename,
    std::string& addressesFilename, std::string& outputFilename);

/** @brief ���������� ������� ����. */
void menu();