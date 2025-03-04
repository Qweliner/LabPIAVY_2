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

/** @brief ��������� ������ ������ �� ������� � ���������� Esc � Backspace � ����������� ������������ ��������.
 *  @param instruction ���������, ������������ ������������ ����� ������.
 *  @return ��������� ������, ��� ������ ������, ���� ��� ����� Esc.
 */
string getLineWithEsc(const string& instruction) {
    string input;
    cout << instruction;
    while (true) {
        int key = _getch();
        char ch = static_cast<char>(key);
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
        // ���������: ��������, ���������, �����, ������, �����, ����, �������������
         else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                   (ch >= '�' && ch <= '�') || (ch >= '�' && ch <= '�') ||
                   (ch >= '0' && ch <= '9') || ch == ' ' || ch == '.' || ch == '-' || ch == '_')
        {
            input += ch;
            cout << ch;
        }
    }
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
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename,
    bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer, const string& folderPath) {
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
        writeToBuffer("�� ������� ������� ���� �������: " + folderPath+ addrFilename);
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
            ifstream corrFile(folderPath + corrFilename);

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
                writeToBuffer("�� ������� ������� ���� ���������������: " + folderPath + corrFilename);
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
    if (correspondenceFilename.empty()) {
        return { "", "" };
    }

    addressesFilename = getLineWithEsc("������� ��� ����� � �������� ����������� (��� ����������) ��� ������� Esc ��� ������: ");
    if (addressesFilename.empty()) {
        return { "", "" };
    }

    return { correspondenceFilename, addressesFilename };
}

/** @brief ��������� �������� ������� ���������. */
void runProgram(const string& folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) { // �������� ����������� ���� ��� ������ ������ ������
        bool escPressed = false; // ��������� ���������� ��� runProgram
        cout << "\n�������� ����� ������:\n";
        cout << "1. ������������� ����� �� �����\n";
        cout << "2. ������ ����� � ����\n";
        cout << "Esc - ��������� � ������� ����\n";
        cout << "\n��� �����: ";

        char choice = _getch();
        char choiceChar = static_cast<char>(choice);
        if (choiceChar != '1' && choiceChar != '2' && choice != 27) {
            cerr << "�������� �����.\n";
            cout << "������� ����� ������� ��� �����������...\n";
            _getch();
             continue;
        }
        cout << endl;

        switch (choice) {
            case '1': {
                string orgName = getLineWithEsc("������� �������� ����������� ��� ������ ��� ������� Esc ��� ������: ");
                if (orgName.empty()) {
                    break; // ����� �� case '1', ������� � ���� ������ ������
                }

                ofstream* outfile = nullptr;  // ��������� �� ofstream, �������������� nullptr
                set<string> printedOrganizations;
                vector<string> outputBuffer;
                processOrganization(orgName, correspondenceFilename, addressesFilename,
                                    true, outfile, printedOrganizations, outputBuffer, folderPath);

                // ����� � �������
                for (const string& line : outputBuffer) {
                    cout << line << endl;
                }

                cout << "������� ����� ������� ��� �������� � ������� ����...\n"; // ���������
                _getch();                                                        // ���������
                system("cls");                                                    // ���������
                break; // ����� �� case '1', ������� � ���� ������ ������
            }
            case '2': {
                // ��������� ������� ��� �����
                string baseFilename = folderPath + "�����_IC_" + correspondenceFilename + "_AO_" + addressesFilename;
                string filename = baseFilename + ".txt";
                int i = 1;
                while (filesystem::exists(filename)) {
                    filename = baseFilename + "(" + to_string(i++) + ")" + ".txt";
                }
                try {
                    ofstream outfile(filename);

                    if (!outfile.is_open()) {
                        cerr << "������: �� ������� ������� ���� ��� ������.\n";
                        break; // ����� �� case '2', ������� � ���� ������ ������
                    }
                    set<string> printedOrganizations;
                    vector<string> outputBuffer; // ���� ����� ������ �� �����.
                    ifstream addrFile(folderPath + addressesFilename);

                    if (addrFile.is_open()) {
                        string line;
                        getline(addrFile, line); // ���������� ���������
                        while (getline(addrFile, line)) {
                            size_t pos = line.find('\t');
                            if (pos != string::npos) {
                                string orgName = line.substr(0, pos);
                                if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
                                    processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer, folderPath);  // �������� &outfile
                                }
                            }
                        }
                        addrFile.close();
                    }
                    else {
                        cerr << "������: �� ������� ������� ���� �������: " + addressesFilename + ".\n";
                            break;
                    }
                    outfile.close(); // ��������� ���� *�����* ���������
                    cout << "���������� ������� �������� � ���� " << filename << ".\n";

                    cout << "������� ����� ������� ��� �������� � ������� ����...\n";
                    _getch();
                    break; // ����� �� case '2', ������� � ���� ������ ������
                }
                catch (const exception& e)
                {
                    cerr << "Exception occurred: " << e.what() << endl;
                    break;
                }
            }
            case 27:
                return;  // ����� �������, ���� ESC

            default:
                cerr << "�������� �����.\n";
                cout << "������� ����� ������� ��� �����������...\n";
                _getch();
        }
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

        // ����� ��������� ���������
        cout << "*************************************************************\n";
        cout << "* ��������� ��� ��������� ������ ��������������� � �������  *\n";
        cout << "* ��������� ��������� ������ � ������������ ���������� ��   *\n";
        cout << "* ������������ �� ������ ���� ������ -                      *\n";
        cout << "* ��������� ��������������� (IC_ ***.txt)                   *\n";
        cout << "* ������� ����������� (AO_***.txt)                          *\n";
        cout << "*************************************************************\n\n";

        cout << "������� ����:\n";
        cout << "1. ������ ������\n";
        cout << "2. �������� ���� � �����\n";
        cout << "3. ����������\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "Esc - �����\n\n";

        char mainChoice = _getch();
        char mainChoiceChar = static_cast<char>(mainChoice);

        if (mainChoiceChar != '1' && mainChoiceChar != '2' && mainChoice != 27 && mainChoiceChar != '3') {
            cerr << "�������� �����.\n";
            cout << "������� ����� ������� ��� �����������...\n";
            _getch();
             continue;
        }

        switch (mainChoice) {
            case '1': {
                pair<string, string> filenames = getFilenamesFromUser(currentFolderPath);
                if (filenames.first.empty() && filenames.second.empty()) {
                    break;  // ��������� "������ ������", ���� ���� ������ �������
                }
                correspondenceFilename = filenames.first;
                addressesFilename = filenames.second;
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename); // �������� filenames
                break; // ����� �� case '1'
            }
            case '2': {
                string newPath = getLineWithEsc("������� ����� ���� � �����: ");
                if(newPath.empty()) break; //���� ����� Esc, �� ������� �� case '2'
                newPath = normalizePath(newPath); // ������������ ���������� ����

                // �������� ������������� ����������
                struct stat buffer;
                if (stat(newPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR)) {
                    currentFolderPath = newPath + "\\";
                    cout << "���� ������� �������.\n";
                } else {
                    cerr << "������: ��������� ���� �� ���������� ��� �� �������� �����������.\n";
                }
                cout << "������� ����� ������� ��� �����������...\n";
                _getch();
                break;
            }
            case 3: {
                readInstructionsFromFile(instructionsFile);
                break;
            }
            case 27:
                return;
            default:
                cerr << "�������� �����.\n";
        }
    }
}

/** @brief ����������� ���� � �����, ����� ��� ����������.
 */
string normalizePath(const string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);
    return normalizedPath.string();
}