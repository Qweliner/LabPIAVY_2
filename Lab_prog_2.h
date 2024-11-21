#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <limits>


using namespace std;

// ��������� ��� �����������
struct Organization {
    string name;
    string address;
    string contactPerson;
};

// ��������� ��� ���������������
struct Correspondence {
    string type;
    string date;
    string organizationName;
};

// ��������� ��� ������������ ������
struct MergedData {
    vector<Organization> organizations;
    vector<Correspondence> correspondences;
};

void instruction() {
    system("cls");
    cout << "���������� � ���������\n\n";
    cout << "������ ��������� ������������� ��� ������ � ������� �� ���� ������: \"��������� ���������������\" (IC_data.txt) � \"������ �����������\" (AO_data.txt).\n\n";

    cout << "�������������� �����������:\n";
    cout << "1. ������������� ����� �� �����: ������� ���������� �� ����������� � �� ���������������.\n";
    cout << "2. ������ ����� � ����: ������� ����� �� ���� �����������.\n\n";

    cout << "�������������:\n";
    cout << "1. ��������� ���������.\n";
    cout << "2. ������� ���� � ����� � ������� (���� ������ ������������� �� '\\').\n";
    cout << "3. �������� ����� ������ (1 ��� 2).\n";
    cout << "4. �������� ����������� �� ������.\n\n";

    cout << "���������� � ������:\n";
    cout << "* ����� ������ ���� � ������� TSV.\n";
    cout << "* ���� � ������� ��.��.����.\n";
    cout << "* � ��� ������������ ����������� �����.\n\n";

    cout << "��������� ������:\n";
    cout << "* �������� ���� � �����.\n";
    cout << "* ����� �� ������� ��� ����������.\n";
    cout << "* ������������ ������ ������.\n\n";

    cout << "������� ����� ������� ��� �����������...\n";
    system("pause");
}

// ������� ������� ���� � �����
string getFolderPath() {
    string folderPath;
    while (true) {
        system("cls");
        cout << "������� ���� � ����� (��������������� �� \\): ";
        getline(cin, folderPath);

        if (folderPath.empty()) {
            cout << "���� �� ����� ���� ������!\n";
            system("pause");
            continue;
        }

        if (folderPath.back() != '\\') {
            folderPath += '\\';
        }

        struct stat buffer;
        if (stat(folderPath.c_str(), &buffer) != 0) {
            cout << "������: ����� �� �������!\n";
            system("pause");
            continue;
        }
        return folderPath;
    }
}

// ������� �������� ������ �� �����
bool loadFile(const string& filePath, MergedData& data) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "������ �������� �����: " << filePath << endl;
        return false;
    }

    string line;
    getline(file, line); // ���������� ���������

    if (filePath.find("IC_") != string::npos) { // ��������� ���������������
        while (getline(file, line)) {
            stringstream ss(line);
            string type, date, orgName;
            getline(ss, type, '\t');
            getline(ss, date, '\t');
            getline(ss, orgName, '\t');
            data.correspondences.push_back({ type, date, orgName });
        }
    }
    else if (filePath.find("AO_") != string::npos) { // ������ �����������
        while (getline(file, line)) {
            stringstream ss(line);
            string name, address, contact;
            getline(ss, name, '\t');
            getline(ss, address, '\t');
            getline(ss, contact, '\t');
            data.organizations.push_back({ name, address, contact });
        }
    }
    else {
        cerr << "����������� ��� �����: " << filePath << endl;
        return false;
    }
    file.close();
    return true;
}

// ������� ����������� ������
void mergeData(const MergedData& icData, const MergedData& aoData, MergedData& mergedData) {
    mergedData.organizations = aoData.organizations;
    mergedData.correspondences = icData.correspondences;
}

// ������� �������������� ������ �� �����
void selectiveOutput(const MergedData& mergedData) {
    string orgName;
    cout << "�������� �����������: ";
    getline(cin, orgName);

    auto itOrg = find_if(mergedData.organizations.begin(), mergedData.organizations.end(),
        [&](const Organization& org) { return org.name == orgName; });

    if (itOrg != mergedData.organizations.end()) {
        cout << "�����������: " << itOrg->name << endl;
        cout << "�����: " << itOrg->address << endl;
        cout << "���: " << itOrg->contactPerson << endl;
        cout << "���������������:\n";
        for (const auto& corr : mergedData.correspondences) {
            if (corr.organizationName == orgName) {
                cout << "  ���: " << corr.type << endl;
                cout << "  ����: " << corr.date << endl;
            }
        }
    }
    else {
        cout << "����������� �� �������!\n";
    }
    system("pause");
}

// ������� ������� ������ � ����
void fullOutputToFile(const MergedData& mergedData, const string& outputFilePath) {
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "������ �������� �����: " << outputFilePath << endl;
        return;
    }

    for (const auto& org : mergedData.organizations) {
        outputFile << "�����������: " << org.name << endl;
        outputFile << "�����: " << org.address << endl;
        outputFile << "���: " << org.contactPerson << endl;
        outputFile << "���������������:\n";
        for (const auto& corr : mergedData.correspondences) {
            if (corr.organizationName == org.name) {
                outputFile << "  ���: " << corr.type << endl;
                outputFile << "  ����: " << corr.date << endl;
            }
        }
        outputFile << endl;
    }
    outputFile.close();
    cout << "������ ������� �������� � " << outputFilePath << endl;
    system("pause");
}

// ������� ����������� ����
void displayMenu() {
    char choice;
    do {
        system("cls");
        cout << "\n����\n";
        cout << "1) ������������� �����\n";
        cout << "2) ������ ����� � ����\n";
        cout << "3) ����������\n";
        cout << "Esc) �����\n";
        cout << "> ";

        choice = cin.get(); 

        if (choice == 27) break; 

        MergedData mergedData;
        string folderPath = getFolderPath();
        if (folderPath.empty()) return;

        MergedData icData, aoData;
        string icFilePath = folderPath + "IC_data.txt";
        string aoFilePath = folderPath + "AO_data.txt";

        if (!loadFile(icFilePath, icData) || !loadFile(aoFilePath, aoData)) {
            cerr << "������ �������� ������!\n";
            system("pause");
            return;
        }
        mergeData(icData, aoData, mergedData);

        try {
            switch (choice) {
            case '1':
                selectiveOutput(mergedData);
                break;
            case '2': {
                string outputFileName;
                cout << "��� ��������� ����� (��� ����������): ";
                cin >> outputFileName;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //https://stackoverflow.com/questions/25020129/cin-ignorenumeric-limitsstreamsizemax-n
                if (outputFileName.empty()) {
                    cout << "��� ����� �� ����� ���� ������!\n";
                    system("pause");
                    break;
                }
                string outputFilePath = folderPath + outputFileName + ".txt";
                fullOutputToFile(mergedData, outputFilePath);
                break;
            }
            case '3':
                instruction();
                break;
            default:
                cout << "�������� �����!\n";
                system("pause");
            }
        }
        catch (const exception& e) {
            cerr << "��������� ������: " << e.what() << endl;
            system("pause");
        }
    } while (true);
}

// ������� ������� ���������
void runProgram() {
    displayMenu();
}

// ������� �������� ������������ ����
bool isValidDate(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '.' || dateStr[5] != '.') return false;
    for (char c : dateStr) {
        if (!isdigit(c) && c != '.') return false;
    }
    try {
        int d = stoi(dateStr.substr(0, 2));
        int m = stoi(dateStr.substr(3, 2));
        int y = stoi(dateStr.substr(6, 4));
        if (m < 1 || m > 12 || d < 1 || d > 31) return false;
        if (m == 2) {
            bool leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
            if (d > (leap ? 29 : 28)) return false;
        }
        if ((m == 4 || m == 6 || m == 9 || m == 11) && d == 31) return false;
        return true;
    }
    catch (const invalid_argument& e) {
        return false;
    }
}