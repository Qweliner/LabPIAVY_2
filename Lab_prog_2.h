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

#ifdef max
#undef max
#endif

// √лобальна€ переменна€ дл€ хранени€ текущего пути к папке с данными
extern std::string currentFolderPath;

// ќбъ€вление констант
namespace Constants {
    const std::string INSTRUCTIONS_FILE = "instructions.txt";
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

bool is_leap(int year);
bool isValidFio(const std::string& fio);
bool isValidDate(const std::string& dateStr);
bool isValidFileName(const std::string& fileName);
void readInstructionsFromFile(const std::string& filename);
void processOrganization(const std::string& orgName, const std::string& corrFilename, const std::string& addrFilename, bool selectiveOutput, std::ofstream* outfile, std::set<std::string>& printedOrganizations, std::vector<std::string>& outputBuffer);
std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath);
void runProgram(const std::string& folderPath, const std::string& correspondenceFilename, const std::string& addressesFilename, std::string& outputFilename);
void menu();
std::string normalizePath(const std::string& path);

std::string getLineWithEsc(const std::string& instruction);