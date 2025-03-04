#include "Lab_prog_2.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <conio.h>

using namespace std;

// ����������� ���������� ����������
string currentFolderPath = "";

/** @brief ��������� ������ ������ �� ������� � ���������� Esc � Backspace.
 *  @param instruction ���������, ������������ ������������ ����� ������.
 *  @return ��������� ������, ��� ������ ������, ���� ��� ����� Esc.
 */
string getLineWithEsc(const string& instruction) {
    string input;
    cout << instruction;
    while (true) {
        int key = _getch();
        if (key == 27) { // ESC
            cin.clear();
            input = ""; // ������� ������ �����
            cout << endl;
            return input;
        }
        else if (key == '\r') { // Enter
            cout << endl;
            return input;
        }
        else if (key == 8) { // Backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else if (key >= 32 && key <= 126) { // Printable characters
            input += (char)key;
            cout << (char)key;
        }
    }
}

/** @brief ����������, �������� �� ��� ����������. */
bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

/** @brief ��������� ������������ ����� �����. */
bool isValidFileName(const string& fileName) {
    const string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == string::npos;
}

/** @brief ������ ���������� �� �����. */
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
        cerr << "�� ������� ������� ���� ����������: " << filename << endl;
        cerr << "����������, ��������� ���� instructions.txt � �� �� �����, ��� ��������� ����������� ���� ���������.\n";
    }
    cout << "\n������� ����� ������� ��� �����������...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

/** @brief ������������ ���������� �� �����������.
 *  ������� � ������� ������ ����������� �� ������ ��������������� � �������.
 */
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

    auto writeToBuffer = [&](const string& str) {
        outputBuffer.push_back(str);
        };

    if (addrFile.is_open()) {
        string line;
        getline(addrFile, line); // ���������� ���������
        lineNumber++;

        while (getline(addrFile, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                string currentOrg = line.substr(0, pos1);
                if (currentOrg == orgName) {
                    addresses.push_back({ line.substr(pos1 + 1, pos2 - pos1 - 1),
                                         line.substr(pos2 + 1) });
                    orgFoundInAddress = true;
                }
            }
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        writeToBuffer("�� ������� ������� ���� �������: " + addrFilename);
        return;
    }

    if (!orgFoundInAddress) {
        if (selectiveOutput)
            writeToBuffer("����������� " + orgName + " �� ������� � ����� �������.");
        return;
    }

    if (selectiveOutput) {
        writeToBuffer("�����������: " + orgName);
        if (addresses.size() > 0) {
            for (size_t i = 0; i < addresses.size(); ++i) {
                if (i == 0) {
                    writeToBuffer("�����: " + addresses[i].first);
                    writeToBuffer("���������� ����: " + addresses[i].second);
                }
                else {
                    if (i == 1)
                        writeToBuffer("\n������ ��������� ������ ��������:\n");
                    writeToBuffer("\n�����: " + addresses[i].first);
                    writeToBuffer("���������� ����: " + addresses[i].second);
                }
            }
        }
    }
    else if (outfile) {
        if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
            printedOrganizations.insert(orgName);
            *outfile << "�����������: " << orgName << endl;
            if (addresses.size() > 0) {
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i == 0) {
                        *outfile << "�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                    else {
                        if (i == 1)
                            *outfile << "\n������ ��������� ������ ��������:\n" << endl;
                        *outfile << "\n�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                }
            }
        }
    }

    if (orgFoundInAddress) {
        if (selectiveOutput ||
            (outfile && printedOrganizations.find(orgName) !=
                printedOrganizations.end())) {
            if (corrFile.is_open()) {
                string line;
                getline(corrFile, line); // ���������� ���������
                lineNumber = 1;
                lineNumber++;
                bool foundCorrespondence = false;
                while (getline(corrFile, line)) {
                    size_t pos1 = line.find('\t');
                    size_t pos2 = line.find('\t', pos1 + 1);

                    if (pos1 != string::npos && pos2 != string::npos) {
                        string currentOrg = line.substr(pos2 + 1);
                        if (currentOrg == orgName) {
                            foundCorrespondence = true;
                            string type = line.substr(0, pos1);
                            string date = line.substr(pos1 + 1, pos2 - pos1 - 1);

                            if (selectiveOutput) {
                                writeToBuffer("\t��� ���������������: " + type);
                                writeToBuffer("\t����: " + date);
                            }
                            else if (outfile) {
                                *outfile << "\t��� ���������������: " << type << endl;
                                *outfile << "\t����: " << date << endl;
                            }
                        }
                    }
                    lineNumber++;
                }
                if (!foundCorrespondence && selectiveOutput)
                    writeToBuffer("��������������� ��� ����������� " + orgName + " �� �������.");
                corrFile.close();
            }
            else
                writeToBuffer("�� ������� ������� ���� ���������������: " + corrFilename);
        }
    }
    if (selectiveOutput)
        writeToBuffer("--------------------");
    else if (outfile)
        *outfile << "--------------------" << endl;
}

/** @brief ����������� ����� ������ � ������������. */
pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    correspondenceFilename = getLineWithEsc("������� ��� ����� � ��������� ���������������� (��� ����������) ��� ������� Esc ��� ������: ");

    if (correspondenceFilename.empty() || !isValidFileName(correspondenceFilename)) {
        cout << "������������ ��� ����� � ����������������.\n";
        cout << "��� ����������� ������� Enter.";
        system("PAUSE>nul");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return { "", "" };
    }

    addressesFilename = getLineWithEsc("������� ��� ����� � �������� ����������� (��� ����������) ��� ������� Esc ��� ������: ");

    if (addressesFilename.empty() || !isValidFileName(addressesFilename)) {
        cout << "������������ ��� ����� � ��������.\n";
        cout << "��� ����������� ������� Enter.";
        system("PAUSE>nul");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return { "", "" };
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt",
            folderPath + "AO_" + addressesFilename + ".txt" };
}

/** @brief ��������� �������� ������� ���������. */
void runProgram(const string& folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    string orgName;

    cout << "\n�������� ����� ������:\n";
    cout << "1. ������������� ����� �� �����\n";
    cout << "2. ������ ����� � ����\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "\n��� �����: ";

    char choice = _getch();
    cout << endl;

    switch (choice) {
    case '1': {
        orgName = getLineWithEsc("������� �������� ����������� ��� ������ ��� ������� Esc ��� ������: ");

        if (orgName.empty()) {
            break;
        }

        ofstream* outfile = nullptr;
        set<string> printedOrganizations;
        vector<string> outputBuffer;
        processOrganization(orgName, correspondenceFilename, addressesFilename,
            true, outfile, printedOrganizations, outputBuffer);

        // ����� � �������
        for (const string& line : outputBuffer)
            cout << line << endl;

        cout << "������� ����� ������� ��� �������� � ������� ����...\n"; // ���������
        _getch();                                                        // ���������
        system("cls");                                                    // ���������
        break;
    }
    case '2': {
        ofstream outfile(folderPath + "output.txt");
        if (!outfile.is_open()) {
            cerr << "������: �� ������� ������� ���� ��� ������.\n";
            break;
        }
        set<string> printedOrganizations;
        vector<string> outputBuffer;
        ifstream addrFile(addressesFilename);

        if (addrFile.is_open()) {
            string line;
            getline(addrFile, line);
            while (getline(addrFile, line)) {
                size_t pos = line.find('\t');
                if (pos != string::npos) {
                    string orgName = line.substr(0, pos);
                    if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
                        processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);
                    }
                }
            }
            addrFile.close();
        }
        else {
            cerr << "������: �� ������� ������� ���� �������.\n";
        }
        outfile.close();
        cout << "���������� ������� �������� � ���� output.txt.\n";

        cout << "������� ����� ������� ��� �������� � ������� ����...\n"; // ���������
        _getch();                                                        // ���������
        system("cls");                                                    // ���������

        break;
    }
    case 27:
        return;  // ����� �������, ���� ESC
    default:
        cerr << "�������� �����.\n";
        cout << "������� ����� ������� ��� �����������...\n";
        _getch();
    }
}

/** @brief ���������� ������� ����. */
void menu() {
    string correspondenceFilename, addressesFilename, outputFilename;
    string escPressed; //��� ������������ esc � getFileName
    bool folderPathSet = true;
    currentFolderPath = normalizePath(filesystem::current_path().string()) + "\\";
    const string instructionsFile = currentFolderPath + "instructions.txt";

    cout << "\n��������� ��� ��������� ������ � ��������������� � ������� �����������.\n";
    cout << "��� ��������� ������ ���������� �� ������������ � �� ���������������, � ����� ����������� ������.\n";

    while (true) {
        system("cls");
        cout << "������� ����:\n";
        cout << "1. ������ ������\n";
        cout << "2. �������� ���� � �����\n";
        cout << "3. ����������\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "Esc - �����\n\n";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': {
            pair<string, string> filenames;

            filenames = getFilenamesFromUser(currentFolderPath);
            if (filenames.first.empty() && filenames.second.empty()) {
                break;  // ��������� "������ ������", ���� ���� ������ �������
            }
            correspondenceFilename = filenames.first;
            addressesFilename = filenames.second;
            runProgram(currentFolderPath, correspondenceFilename, addressesFilename,
                outputFilename);
            break;
        }
        case '2': {
            string newPath = getLineWithEsc("������� ����� ���� � �����: ");
            newPath = normalizePath(newPath); // ������������ ���������� ����

            // �������� ������������� ����������
            struct stat buffer;
            if (stat(newPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR)) {
                currentFolderPath = newPath + "\\";
                cout << "���� ������� �������.\n";
            }
            else {
                cerr << "������: ��������� ���� �� ���������� ��� �� �������� �����������.\n";
            }
            cout << "������� ����� ������� ��� �����������...\n";
            _getch();
            break;
        }
        case '3':
            readInstructionsFromFile(instructionsFile);
            break;
        case 27:
            return;
        default:
            cerr << "�������� �����.\n";
        }
    }
}

string normalizePath(const string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);
    return normalizedPath.string();
}