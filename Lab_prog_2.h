//Lab_prog_2.h
#pragma once // ��������� ������������� ��� �������������� ���������� ��������� ����� �����

// ����������� ����������� ����������� ��������� C++
#include <iostream>  // ��� ������� �����/������ (cin, cout)
#include <fstream>   // ��� ������ � ��������� �������� (ifstream, ofstream)
#include <string>    // ��� ������ �� �������� (std::string)
#include <vector>    // ��� ������������� ������������ �������� (std::vector)
#include <set>       // ��� ������������� �������� (�������� ���������� ���������)
#include <sstream>   // ��� ������ �� ���������� �������� (������ �����)
#include <algorithm> // ��� ����������� ���������� (��������, std::find)
#include <filesystem>// ��� ������ � �������� �������� (�������� ������������� ������, �����)
#include <cstdio>    // ��� ������������ �����/������ � ����� C (printf, fopen, ...)
#include <cstring>   // ��� ������� ������ �� �������� � ����� C (strcpy, strlen, ...)
#include <conio.h>   // ��� ����������� �����/������ ��� ����������� (_getch, _kbhit)
#include <windows.h> // ��� ������� Windows API (������ � ��������, ������� ������)
#include <direct.h>  // ��� ������� ������ � ������������ (_getcwd)
#include <sys/stat.h>// ��� ��������� ���������� � ������ (stat)
#include <cstdlib>   // ��� ����� ������ C (��������, _fullpath)
#include <cctype>    // ��� ������� �������� �������� (isdigit, tolower)

// ���� ������ max ���������, �������� ��� �����������, ����� �������� ���������� � std::max
#ifdef max
#undef max
#endif

// --- ���������� ���������� � ��������� ---

// ���������� ���������� ��� �������� ���� � ������� ����� (C-style ������)
extern char folder_way[256];
// ����������� ���������� ��� ������������ ������ ������
extern const char* file_extension;
// ������� ��� ������ � ��������� ����������������
extern const char* ocfe; // Incoming Correspondence File Extension (��� Outgoing?) - ����������� �����������, �������� �� �����, �� �������� ����
// ������� ��� ������ � �������� �����������
extern const char* oa;   // Organization Addresses

// ������ ��� ����������� ����������� ����� � ����� C (���������� strcpy_s)
#define SAFE_STRCPY(dest, src, size) strcpy_s(dest, size, src)

// ����������� ������������ ����� ����, ���� �� ���������� ����� (����������� ��� Windows)
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

// ������������ ���� ��� ��������
namespace Constants {
    // ��� ����� � ������������
    const std::string INSTRUCTIONS_FILE = "instructions.txt";
}

// --- ��������� ������ ---

// ��������� ��� �������� ���������� �� ����� ������ ���������������
struct Correspondence {
    std::string type;         // ��� ���������������
    std::string date;         // ����
    std::string organization; // �������� �����������
};

// ��������� ��� �������� ���������� �� ����� ������ ������
struct Address {
    std::string organization;  // �������� �����������
    std::string address;       // �����
    std::string contactPerson; // ���������� ���� (������������)
};


// --- ��������� ������� ---

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ����� ����� */
bool isValidFileNameChar(char c);

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ���� � �����/����� */
bool isValidPathChar(char c);

/** @brief ������� �������� ������� �� ������������ ��� ������������� � �������� ����������� */
bool isValidOrgNameChar(char c);

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ������ ����������� */
bool isValidAddressChar(char c); // ������ 2

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ��� ������������ */
bool isValidContactPersonChar(char c); // ������ 2

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ���� ��������������� */
bool isValidCorrTypeChar(char c); // ������ 2

/** @brief ������� �������� ������� �� ������������ ��� ������������� � ���� ��������������� */
bool isValidCorrDateChar(char c); // ������ 2

/**
 * @brief ������� ��� ��������� ������ � ������� � ����������� �������� ��������.
 * @param instruction ����� ����������� ��� �����.
 * @param buffer ����� ��� ���������� ��������� ������.
 * @param buffer_size ������ ������.
 * @param isValidCharFunc ��������� �� �������, ����������� ������������ �������.
 */
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char));

/**
 * @brief ������� ������������ ���� (�������������� � ���������� ����, �������� ������ ������, ���������� ����� � ����� ��� �����).
 * @param path �������� ����.
 * @param normalized_path ����� ��� ���������� ���������������� ����.
 * @param normalized_path_size ������ ������.
 */
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size);

/** @brief ������� ��� ������ � �������� ���� � ������� ����� */
void selectFolderPath();

// --- ������� �������� ������ ---

/** @brief ������ � ������� ���������� �� ����� Constants::INSTRUCTIONS_FILE */
void readInstructionsFromFile(const std::string& filename);

/**
 * @brief ������������ ���������� ��� ����� �����������: ���� ������ � ������ ������� � ��������������� � �������/��������� ���������.
 * @param orgName �������� ����������� ��� ������.
 * @param corrFilename ���� � ����� ���������������.
 * @param addrFilename ���� � ����� �������.
 * @param selectiveOutput ����: true - ����� � ����� (��� ������), false - ����� � ���� (��� ������).
 * @param outfile ��������� �� �������� ����� ��� ������ (������������, ���� selectiveOutput = false).
 * @param printedOrganizations ��������� ��� ������������ ��� ���������� ����������� (� ������ ������).
 * @param outputBuffer ������ ��� ����� ����� ������ (������������, ���� selectiveOutput = true).
 */
void processOrganization(const std::string& orgName, const std::string& corrFilename,
    const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile,
    std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);

/**
 * @brief ����������� � ������������ ������� ����� ������ ��������������� � �������, ��������� �� ������������� � ��������� ������ ����.
 * @param folderPath ���� � �����, ��� ������ �����.
 * @return ���� �����: ������ ���� � ����� ��������������� � ������ ���� � ����� �������. ���������� ������ ������ ��� ������.
 */
std::pair<std::string, std::string> getFilenamesFromUser(const char* folderPath);

/**
 * @brief ��������� �������� ������� ���������: ����� ������ (�����/�����), ��������� ������ � ����� ����������.
 * @param folderPath ���� � ������� �����.
 * @param correspondenceFilename ���� � ����� ���������������.
 * @param addressesFilename ���� � ����� �������.
 * @param outputFilename (�� ������������ � ������� ����������, �� �������� � ���������) ������ ��� ����� ��������� �����.
 */
void runProgram(const char* folderPath, const std::string& correspondenceFilename,
    const std::string& addressesFilename, std::string& outputFilename);

/** @brief ���������� ������� ���� ��������� � ��������� �������� ������ �������������� � �������������. */
void menu();