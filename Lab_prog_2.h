//Lab_prog_2.h
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <filesystem> // ��� filesystem::exists � filesystem::path
#include <cstdio>
#include <cstring>
#include <conio.h>   // ��� _getch, _kbhit
#include <windows.h> // ��� SetConsoleCP, SetConsoleOutputCP, OpenClipboard, GetClipboardData, GlobalLock, GlobalUnlock, CloseClipboard
#include <direct.h>  // ��� _getcwd
#include <sys/stat.h> // ��� stat
#include <cstdlib>   // ��� _fullpath
#include <cctype>    // ��� isdigit, tolower

#ifdef max
#undef max
#endif

extern char folder_way[256]; // ���� � ����� (��� �� ������ ���������)
extern const char* file_extension;
extern const char* ocfe; // ������� ��� ������ ���������������
extern const char* oa;   // ������� ��� ������ �������

// ������ ��� ����������� ����������� �����
#define SAFE_STRCPY(dest, src, size) strcpy_s(dest, size, src)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt"; // ��� ����� � ������������.
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


/** @brief ������� �������� ������� ��� ����� ����� */
bool isValidFileNameChar(char c);

/** @brief ������� �������� ������� ��� ���� */
bool isValidPathChar(char c);

/** @brief ������� ��� ��������� ������ � ������� � ����������� �������� */
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char));

/** @brief ������� ������������ ���� (���������� ��� Windows) */
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);

/** @brief ������� ������ ���� � ����� (������ program_way) */
void selectFolderPath();

// --- ������� �� ������ ��������� (�������� ������) ---

/** @brief ������ ���������� �� ����� */
void readInstructionsFromFile(const std::string& filename);

/** @brief ������������ ���������� �� ����������� */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile,
    std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);

/** @brief ����������� ����� ������ � ������������ (������������ ��� C-����� ����) */
std::pair<std::string, std::string> getFilenamesFromUser(const char* folderPath);

/** @brief ��������� �������� ������� ��������� (������������ ��� C-����� ����) */
void runProgram(const char* folderPath, const std::string& correspondenceFilename,
    const std::string& addressesFilename, std::string& outputFilename);

/** @brief ���������� ������� ���� */
void menu();
