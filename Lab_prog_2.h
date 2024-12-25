#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <locale.h>
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <limits>
#include <sys/stat.h>
#include <set>

#ifdef max
#undef max
#endif

string currentFolderPath = "";

using namespace std;

// ���������
struct Correspondence {
    string type;
    string date;
    string organization;
};

struct Address {
    string organization;
    string address;
    string contactPerson;
};

void tabul(int x) {
    for (int i = x; i != 0; i--) printf("   ");
}

bool is_leap(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool isValidFio(const string& fio) {
    // �������� �� ������� ���� � ���
    bool valid_char = true;
    for (char c : fio) {
        if (not((c >= '�' && c <= '�') || (c >= '�' && c <= '�') ||
            (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            c == ' ' || c == '-' || c == '\x2D')) {
            valid_char = false;
        }
    }
    return valid_char;
}

bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year);

    if (count != 3) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (day < 1 || day > 31) {
        return false;
    }
    if (month == 2) {
        int daysInFeb = 28 + (is_leap(year) ? 1 : 0);
        if (day > daysInFeb) {
            return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }

    return true;
}

bool isValidFileName(const std::string& fileName) {
    // ����������� ������� � Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

void readInstructionsFromFile(const std::string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� ����������: " << filename << endl;
    }
    cout << "\n������� ����� ������� ��� �����������...";
    _getch();
}

void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile = nullptr, set<string>& printedOrganizations = *(new set<string>())) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<pair<string, string>> addresses;
    bool orgFoundInAddress = false;
    int lineNumber = 1;

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

                    addresses.push_back({ line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1) });
                    orgFoundInAddress = true;
                    string fio = line.substr(pos2 + 1);
                    if (!isValidFio(fio)) {
                        cout << "������ � �����: " << addrFilename << ", ������: " << lineNumber << ", ������������ �������� ���� ���: " << fio << endl;
                    }
                }
            }
            if (!selectiveOutput && outfile && printedOrganizations.find(orgName) != printedOrganizations.end()) {
                break;
            }
            lineNumber++;
        }
        addrFile.close();
    }
    else {
        cerr << "�� ������� ������� ���� �������: " << addrFilename << endl;
        return;
    }

    if (!orgFoundInAddress) {
        if (selectiveOutput)
            cerr << "����������� " << orgName << " �� ������� � ����� �������." << endl;
        return;
    }

    if (selectiveOutput) {
        cout << "�����������: " << orgName << endl;
        if (addresses.size() > 0) {
            for (size_t i = 0; i < addresses.size(); ++i) {
                if (i == 0) {
                    cout << "�����: " << addresses[i].first << endl;
                    cout << "���������� ����: " << addresses[i].second << endl;
                }
                else {
                    if (i == 1) cout << "\n������ ��������� ������ ��������:\n";
                    cout << "\n�����: " << addresses[i].first << endl;
                    cout << "���������� ����: " << addresses[i].second << endl;
                }
            }
        }
    }
    else if (outfile) {
        if (printedOrganizations.find(orgName) == printedOrganizations.end())
        {
            printedOrganizations.insert(orgName);
            *outfile << "�����������: " << orgName << endl;
            if (addresses.size() > 0) {
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i == 0) {
                        *outfile << "�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                    else {
                        if (i == 1) *outfile << "\n������ ��������� ������ ��������:\n";
                        *outfile << "\n�����: " << addresses[i].first << endl;
                        *outfile << "���������� ����: " << addresses[i].second << endl;
                    }
                }
            }
        }
    }

    if (orgFoundInAddress) {
        if (selectiveOutput || (outfile && printedOrganizations.find(orgName) != printedOrganizations.end())) {
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
                            bool dateError = !isValidDate(date);

                            if (dateError) {
                                cout << "������ � �����: " << corrFilename << ", ������: " << lineNumber << ", ������������ �������� ���� ����: " << date << endl;
                            }

                            if (selectiveOutput) {
                                cout << "\t��� ���������������: " << type << endl;
                                cout << "\t����: " << date << endl;
                            }
                            else if (outfile) {
                                *outfile << "\t��� ���������������: " << type << endl;
                                *outfile << "\t����: " << date << endl;
                            }
                        }
                    }
                    lineNumber++;
                }
                if (!foundCorrespondence && selectiveOutput) {
                    cerr << "��������������� ��� ����������� " << orgName << " �� �������." << endl;
                }
                corrFile.close();
            }
            else {
                cerr << "�� ������� ������� ���� ���������������: " << corrFilename << endl;
            }
        }
    }
    if (selectiveOutput) {
        cout << "--------------------" << endl;
    }
    else if (outfile) {
        *outfile << "--------------------" << endl;
    }
}

pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    cout << "������� ��� ����� � ��������� ���������������� (��� ����������): ";
    cin >> correspondenceFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(correspondenceFilename)) {
        tabul(11); printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
        printf("��� ����������� ������� ENTER."); system("PAUSE>nul");

    }

    cout << "������� ��� ����� � �������� ����������� (��� ����������): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11); printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
        printf("��� ����������� ������� ENTER."); system("PAUSE>nul");

    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt", folderPath + "AO_" + addressesFilename + ".txt" };

}

void runProgram(string& folderPath, string& correspondenceFilename, string& addressesFilename, string& outputFilename) {
    pair<string, string> filenames = getFilenamesFromUser(folderPath);
    correspondenceFilename = filenames.first;
    addressesFilename = filenames.second;

    if (folderPath.back() != '\\' && folderPath.back() != '/') {
        folderPath += '\\';
    }

    outputFilename = folderPath + "output.txt";

    char choice;
    cout << "\n�������� ����� ������:\n";
    cout << "1. ������������� ����� �� ����� (I)\n";
    cout << "2. ������ ����� � ���� (O)\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "��� �����: ";

    choice = _getch();
    cout << endl;

    switch (choice) {
    case '1': case 'i': case 'I': {
        string orgName;
        cout << "������� �������� ����������� ��� ������: ";
        getline(cin, orgName);
        processOrganization(orgName, correspondenceFilename, addressesFilename, true);
        break;
    }
    case '2': case 'o': case 'O': {
        ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            cerr << "������: �� ������� ������� ���� ��� ������: " << outputFilename << endl;
            return;
        }
        set<string> printedOrganizations;
        ifstream addrFile(addressesFilename);
        if (addrFile.is_open()) {
            string line;
            getline(addrFile, line); // ���������� ���������
            while (getline(addrFile, line)) {
                size_t pos1 = line.find('\t');
                if (pos1 != string::npos) {
                    string orgName = line.substr(0, pos1);
                    if (printedOrganizations.find(orgName) == printedOrganizations.end()) {
                        processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations);
                    }
                }
            }
            addrFile.close();
        }
        outfile.close();
        cout << "\n���������� ������� �������� � ����: " << outputFilename << endl;
        break;
    }
    case 27:
        return;
    default:
        cerr << "�������� �����.\n";
    }

    cout << "������� ����� ������� ��� �������� � ����...";
    _getch();
}

void menu() {
    bool folderPathSet = false;
    string correspondenceFilename, addressesFilename, outputFilename;
    string currentFolderPath = "";
    const string instructionsFile = "instructions.txt";

    while (true) {
        system("cls");
        cout << "������� ����:\n";
        cout << "1. ������ ������ (S) ";
        if (!folderPathSet) {
            cout << "(������� ������� ���� � �����)\n";
        }
        else {
            cout << '\n';
        }
        cout << "2. ���� � ����� (F)\n";
        cout << "3. ���������� (I)\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "Esc - �����\n\n";
        cout << "> ";

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': case 's': case 'S':
            if (!folderPathSet) {
                cerr << "������: ������� ������� ���� � �����.\n";
                _getch();
            }
            else {
                runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename);
            }
            break;
        case '2': case 'f': case 'F': {
            cout << "������� ���� � �����: ";
            getline(cin, currentFolderPath);

            // �������� ���� � �������������� stat
            struct stat buffer;
            if (stat(currentFolderPath.c_str(), &buffer) != 0) {
                if (errno == ENOENT) {
                    cerr << "������: ��������� ���� �� ����������.\n";
                }
                else {
                    cerr << "������: �� ������� ��������� ���� � �����. ��� ������: " << errno << "\n";
                }
                _getch();
                continue; // ������� � ������ �����, ���� ���� �����������
            }

            // ��������� '\' � ����� ����, ���� ��� ��� ���
            if (currentFolderPath.back() != '\\') {
                currentFolderPath += '\\';
            }

            folderPathSet = true;
            cout << "\n������� ���� � �����: " << currentFolderPath << endl;
            cout << "������� ����� ������� ��� �����������...";
            _getch();
            break;
        }
        case '3': case 'i': case 'I':
            readInstructionsFromFile(instructionsFile);
            break;
        case 27:
            return;
        default:
            cerr << "�������� �����.\n";
        }
    }
}