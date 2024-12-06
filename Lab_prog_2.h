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

bool isValidDate(const string& dateStr) { // ��������: ������ ��������� std::string
    if (dateStr.length() != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr.c_str(), "%2d.%2d.%4d", &day, &month, &year); // ���������� c_str()

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

void printInstructions() {
    system("cls");
    cout << "\n���������� �� ������ � ����������:\n";
    cout << "1. ���� � �����: ������� ������ ���� � �����, ���������� ����� � ���������������� � ��������.\n";
    cout << "   ������: C:\\Users\\YourName\\Documents\\Correspondence\n";
    cout << "2. �������� ������: ������� �������� ������ ��� ���������� .txt.\n";
    cout << "   ��������� ������������� ������� �������� (IC_ � AO_) � ����������.\n";
    cout << "3. ������������� �����: ��� ������ ����� ������ ������� �������� ����������� ��� ������.\n";
    cout << "4. ������ �����: ��� ������ ����� ������ ������ ����� �������� � ���� output.txt � ��������� �����.\n";
    cout << "5. ������:\n";
    cout << "   - �������� ���� � �����: ��������� ������� �� ������.\n";
    cout << "   - �������� ������ ������: ��������� ��������� ������ � ������������ �������� ������ � ������� ��������������.\n";
    cout << "   - ����������� �� �������: ��������� �������, ���� �� ������ ���������� � ����������� �����������.\n";
    cout << "6. ��������� �� ����: ����������� ������� 1, 2, 3, I, S, O ��� Esc ��� ���������.\n";
    cout << "   Esc - ����� �� ��������� ��� ������� � ���������� ����.\n";
    cout << "\n������� ����� ������� ��� �����������...";
    _getch();
}

void correctData(string& data, const string& fieldName) {
    char choice;
    bool new_is_correct = false;
    string new_data;

    cout << "������������ �������� ���� " << fieldName << ": " << data << endl;
    cout << "������� ��������? (y/n): ";
    choice = _getch();
    if (not(choice == 'y' || choice == 'Y' || choice == '�' || choice == '�' || choice == '�' || choice == '�')) {
        return; //������� �� �������, ���� ���������� �� �����
    }
    while (!new_is_correct) {
        cout << "������� ������������ ��������: ";
        getline(cin, new_data);
        if (fieldName == "����") {
            new_is_correct = isValidDate(new_data);
        }
        else if (fieldName == "���") {
            new_is_correct = isValidFio(new_data);
        }
        else {
            new_is_correct = true;
        }
    }
    data = new_data;
}

vector<Correspondence> readCorrespondence(const string& filename) {
    vector<Correspondence> correspondence;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        getline(file, line); // ���������� ���������

        while (getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                Correspondence entry;
                entry.type = line.substr(0, pos1);
                entry.date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.organization = line.substr(pos2 + 1);
                correspondence.push_back(entry);
            }
            else {
                cerr << "������ ������� ����� ��������������� � ������: " << line << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� ���������������: " << filename << endl;
    }
    return correspondence;
}

vector<Address> readAddresses(const string& filename) {
    vector<Address> addresses;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        getline(file, line); // ���������� ���������

        while (getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                Address entry;
                entry.organization = line.substr(0, pos1);
                entry.address = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.contactPerson = line.substr(pos2 + 1);
                addresses.push_back(entry);
            }
            else {
                cerr << "������ ������� ����� ������� � ������: " << line << endl;
            }
        }
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� �������: " << filename << endl;
    }
    return addresses;
}

void printSelectiveInfo(vector<Correspondence> correspondence, vector<Address> addresses, const string& corrFile, const string& addrFile) { // ������ & �� ��������
    string orgName;
    cout << "������� �������� ����������� ��� ������: ";
    getline(cin, orgName);

    bool found = false;
    for (size_t i = 0; i < correspondence.size(); ++i) {
        if (correspondence[i].organization == orgName) {
            found = true;
            for (size_t j = 0; j < addresses.size(); ++j) {
                if (addresses[j].organization == orgName) {
                    bool dateError = !isValidDate(correspondence[i].date);
                    bool fioError = !isValidFio(addresses[j].contactPerson);

                    if (dateError) {
                        correctData(correspondence[i].date, "����");
                    }
                    if (fioError) {
                        correctData(addresses[j].contactPerson, "���");
                    }
                    cout << "\n--------------------" << endl;
                    cout << "��� ���������������: " << correspondence[i].type << endl;
                    cout << "����: " << correspondence[i].date << endl;
                    cout << "�����: " << addresses[j].address << endl;
                    cout << "���������� ����: " << addresses[j].contactPerson << endl;
                    cout << "--------------------" << endl;
                }
            }
        }
    }
    if (!found) {
        cerr << "��������������� ��� ����������� " << orgName << " �� �������." << endl;
    }
}

void printAllInfoToFile(const vector<Correspondence>& correspondence, const vector<Address>& addresses, const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        cerr << "������: �� ������� ������� ���� ��� ������: " << filename << endl;
        return;
    }

    map<string, vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        string fioCopy = addr.contactPerson;
        if (!isValidFio(fioCopy)) {
            correctData(fioCopy, "���");
        }
        outfile << "�����������: " << addr.organization << endl;
        outfile << "�����: " << addr.address << endl;
        outfile << "���������� ����: " << fioCopy << endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                string dateCopy = corr.date;
                if (!isValidDate(dateCopy)) {
                    correctData(dateCopy, "����");
                }
                outfile << "\t��� ���������������: " << corr.type << endl;
                outfile << "\t����: " << dateCopy << endl;
            }
        }
        else {
            outfile << "\t��������������� �� �������." << endl;
        }
        outfile << "--------------------" << endl;
    }
    outfile.close();
    cout << "���������� ������� �������� � ����: " << filename << endl;
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

    vector<Correspondence> correspondence;
    vector<Address> addresses;

    try {
        correspondence = readCorrespondence(correspondenceFilename);
        addresses = readAddresses(addressesFilename);
    }
    catch (const std::runtime_error& error) {
        cerr << "������ ��� ������ ������: " << error.what() << endl;
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    if (correspondence.empty() || addresses.empty()) {
        cerr << "������: ����� ��������������� ��� ������� �����." << endl;
        cout << "������� ����� ������� ��� �������� � ����...";
        _getch();
        return;
    }

    char choice;
    cout << "\n�������� ����� ������:\n";
    cout << "1. ������������� ����� �� ����� (I)\n";
    cout << "2. ������ ����� � ���� (O)\n";
    cout << "Esc - ��������� � ������� ����\n";
    cout << "��� �����: ";

    choice = _getch();

    switch (choice) {
    case '1': case 'i': case 'I':
        printSelectiveInfo(correspondence, addresses, correspondenceFilename, addressesFilename);
        break;
    case '2': case 'o': case 'O':
        printAllInfoToFile(correspondence, addresses, outputFilename);
        break;
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
            printInstructions();
            break;
        case 27:
            return;
        default:
            cerr << "�������� �����.\n";
        }
    }
}