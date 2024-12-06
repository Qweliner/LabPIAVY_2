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

#ifdef max
#undef max
#endif

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

bool isValidDate(const char* dateStr) {
    if (strlen(dateStr) != 10) {
        return false;
    }

    int day, month, year;
    int count = sscanf_s(dateStr, "%2d.%2d.%4d", &day, &month, &year);

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

void printSelectiveInfo(const vector<Correspondence>& correspondence, const vector<Address>& addresses) {
    string orgName;
    cout << "������� �������� ����������� ��� ������: ";
    cin >> orgName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (const auto& corr : correspondence) {
        if (corr.organization == orgName) {
            for (const auto& addr : addresses) {
                if (addr.organization == orgName) {
                    cout << "��� ���������������: " << corr.type << endl;
                    cout << "����: " << corr.date << endl;
                    cout << "�����: " << addr.address << endl;
                    cout << "���������� ����: " << addr.contactPerson << endl;
                    cout << "--------------------" << endl;
                    return;
                }
            }
            cerr << "����� ��� ����������� " << orgName << " �� ������." << endl;
            return;
        }
    }
    cerr << "��������������� ��� ����������� " << orgName << " �� �������." << endl;
}

void printAllInfoToFile(const vector<Correspondence>& correspondence, const vector<Address>& addresses, const string& filename) {
    ofstream outfile(filename);

    if (!outfile.is_open()) {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
        return;
    }

    map<string, vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        outfile << "�����������: " << addr.organization << endl;
        outfile << "�����: " << addr.address << endl;
        outfile << "���������� ����: " << addr.contactPerson << endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                outfile << "\t��� ���������������: " << corr.type << endl;
                outfile << "\t����: " << corr.date << endl;
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
        //break;
    }

    cout << "������� ��� ����� � �������� ����������� (��� ����������): ";
    cin >> addressesFilename;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!isValidFileName(addressesFilename)) {
        tabul(11); printf("������������ ������� � ����� �����. ����������, ����������� ������ ���.\n");
        printf("��� ����������� ������� ENTER."); system("PAUSE>nul");
        //break;
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt", folderPath + "AO_" + addressesFilename + ".txt" };

}



void runProgram(string& folderPath, string& correspondenceFilename, string& addressesFilename, string& outputFilename) {


    tie(correspondenceFilename, addressesFilename) = getFilenamesFromUser(folderPath);
    if (folderPath.back() != '\\' && folderPath.back() != '/') {
        folderPath += '\\';
    }

    outputFilename = folderPath + "output.txt";
    vector<Correspondence> correspondence = readCorrespondence(correspondenceFilename);
    vector<Address> addresses = readAddresses(addressesFilename);

    if (correspondence.empty() || addresses.empty()) {
        cerr << "������ ������ ������. ��������� ����� � ��������� �������.\n";
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
        printSelectiveInfo(correspondence, addresses);
        break;
    case '2': case 'o': case 'O':
        printAllInfoToFile(correspondence, addresses, outputFilename);
        break;
    case 27: // Esc
        return;
    default:
        cerr << "�������� �����.\n";
    }
    cout << "������� ����� ������� ��� �������� � ����...";
    _getch();


}