//Lab_prog_2.cpp
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

string getLineWithEsc(const string& instruction) {
    string input;
    cout << instruction;
    while (true) {
        int key = _getch();
        char ch = static_cast<char>(key);
        if (key == 27) { // ESC
            cin.clear();
            input = "";
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
        // ��������� �������� ������� (������� �������)
        else if (ch >= 32 && ch <= 126) {
            input += ch;
            cout << ch;
        }
        // ���������
        else if ((ch >= '�' && ch <= '�') || (ch >= '�' && ch <= '�'))
        {
            input += ch;
            cout << ch;
        }
    }
}

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
    cout << "\n������� ����� ������� ��� �������� � ������� ����...\n";
    _getch();
    system("mode con cols=120 lines=30");
}

void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile(corrFilename);
    ifstream addrFile(addrFilename);
    vector<Address> addresses;
    bool orgFoundInAddress = false;

    auto writeToBuffer = [&](const string& str) {
        if (selectiveOutput) {
            outputBuffer.push_back(str); // ��� ����������� ������
        }
        else if (outfile) {
            *outfile << str << endl;    // ��� ��������� ������
        }
        };


    // ������� ���� ����������� � ����� �������.  ��� ����� ��� ����� ������� ������.
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            getline(ss, currentOrg, ';');
            getline(ss, address, ';');
            getline(ss, contactPerson, ';');
            currentOrg.erase(0, currentOrg.find_first_not_of(" \t"));  //������� �������
            currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1);
            if (currentOrg == orgName) {
                addresses.push_back({ currentOrg, address, contactPerson });
                orgFoundInAddress = true;
            }
        }
        addrFile.close();
    }
    else {
        writeToBuffer("�� ������� ������� ���� �������: " + addrFilename);
        return; // ���� �� ������� ������� ���� �������, ������ �� ���
    }

    if (!orgFoundInAddress && selectiveOutput) {
        writeToBuffer("�������� ����������� \"" + orgName + "\" �� ������� � ����� �������.");
        return;
    }

    //���� ��� ������ �����, ������� ������ ��� ������ ������
    if (!selectiveOutput)
    {
        if (printedOrganizations.count(orgName)) {
            return; // �������� ����������� ��� ���� ����������
        }
        printedOrganizations.insert(orgName); // �������� ����������� ��� ������������
        // ����� ���������� �� ����������� (�� ����� �������)
        writeToBuffer("�������� �����������: " + orgName);

        for (const auto& addr : addresses) {
            writeToBuffer("�����: " + addr.address);
            writeToBuffer("������� ������������: " + addr.contactPerson);
        }

        // ����� � ����� ��������������� ��� �����������
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                currentOrg.erase(0, currentOrg.find_first_not_of(" \t")); //������� �������
                currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1);
                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\t��� ���������������: " + type);
                    writeToBuffer("\t���� ����������: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\t��������������� �� �������."); // ��� ������� ������
            }
            corrFile.close();
        }
        else {
            writeToBuffer("�� ������� ������� ���� ���������������: " + corrFilename);
        }
        writeToBuffer("--------------------");
    }

    //���� ��� �����
    if (selectiveOutput && orgFoundInAddress)
    {
        writeToBuffer("�������� �����������: " + orgName);
        if (!addresses.empty()) {
            writeToBuffer("�����: " + addresses[0].address);
            writeToBuffer("������� ������������: " + addresses[0].contactPerson);

            if (addresses.size() > 1) {
                writeToBuffer("\n������ ��������� ������ ��������:\n");
                for (size_t i = 1; i < addresses.size(); ++i) {
                    writeToBuffer("�����: " + addresses[i].address);
                    writeToBuffer("������� ������������: " + addresses[i].contactPerson);
                }
            }
        }
        if (corrFile.is_open()) {
            string line;
            bool foundCorrespondence = false;
            while (getline(corrFile, line)) {
                stringstream ss(line);
                string type, date, currentOrg;
                getline(ss, type, ';');
                getline(ss, date, ';');
                getline(ss, currentOrg, ';');
                currentOrg.erase(0, currentOrg.find_first_not_of(" \t"));  //������� �������
                currentOrg.erase(currentOrg.find_last_not_of(" \t") + 1); //

                if (currentOrg == orgName) {
                    foundCorrespondence = true;
                    writeToBuffer("\t��� ���������������: " + type);
                    writeToBuffer("\t���� ����������: " + date);
                }
            }
            if (!foundCorrespondence) {
                writeToBuffer("\t��������������� �� �������."); // ���������, ���� �� �������
            }
            corrFile.close();
        }
        else {
            writeToBuffer("�� ������� ������� ���� ���������������: " + corrFilename);
        }
        writeToBuffer("--------------------"); // ��������� �����������
    }
}

pair<string, string> getFilenamesFromUser(const string& folderPath) {
    string correspondenceFilename, addressesFilename;

    while (true) {
        correspondenceFilename = getLineWithEsc("������� ��� ����� � ��������� ���������������� (�������� IC_ � ��� ���������� .txt) ��� ������� Esc ��� ������: ");
        if (correspondenceFilename.empty()) {
            return { "", "" };
        }

        string fullCorrFilename = folderPath + "IC_" + correspondenceFilename + ".txt";
        if (!filesystem::exists(fullCorrFilename)) {
            cerr << "������: ���� " << fullCorrFilename << " �� ����������.\n";
            cout << "����������, ���������� ��� ���.\n";
            continue;
        }
        break;
    }

    while (true) {
        addressesFilename = getLineWithEsc("������� ��� ����� � �������� ����������� (�������� AO_ � ��� ���������� .txt) ��� ������� Esc ��� ������: ");
        if (addressesFilename.empty()) {
            return { "", "" };
        }

        string fullAddressesFilename = folderPath + "AO_" + addressesFilename + ".txt";
        if (!filesystem::exists(fullAddressesFilename)) {
            cerr << "������: ���� " << fullAddressesFilename << " �� ����������.\n";
            cout << "����������, ���������� ��� ���.\n";
            continue;
        }
        break;
    }

    return { folderPath + "IC_" + correspondenceFilename + ".txt",
             folderPath + "AO_" + addressesFilename + ".txt" };
}

void runProgram(const string& folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) {
        cout << "\n�������� ����� ������:\n";
        cout << "1. ����� �� �������� � ����� � �������\n";
        cout << "2. ������ ����� � ����\n";
        cout << "Esc - ��������� � ������� ����\n";
        cout << "\n��� �����: ";

        char choice = _getch();
        char choiceChar = static_cast<char>(choice);

        while (choiceChar != '1' && choiceChar != '2' && choice != 27) {
            choice = _getch();
            choiceChar = static_cast<char>(choice);
        }

        cout << endl;

        switch (choice) {
        case '1': {
            string orgName = getLineWithEsc("������� �������� ����������� ��� ������ ��� ������� Esc ��� ������: ");
            if (orgName.empty()) {
                break;
            }

            ofstream* outfile = nullptr; // ��������� �� ofstream (�� ������ ������!)
            set<string> printedOrganizations;
            vector<string> outputBuffer; // ����� ��� ������
            processOrganization(orgName, correspondenceFilename, addressesFilename,
                true, outfile, printedOrganizations, outputBuffer);

            // ����� ������ �� �������
            for (const string& line : outputBuffer) {
                cout << line << endl;
            }
            break;
        }
        case '2': {
            // �������� ����� ������ ��� ����� � ���������� ��� ������������ ����� ��������� �����
            string corrFileNameOnly = filesystem::path(correspondenceFilename).filename().string();
            string addrFileNameOnly = filesystem::path(addressesFilename).filename().string();

            size_t pos = corrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                corrFileNameOnly = corrFileNameOnly.substr(0, pos);
            }

            pos = addrFileNameOnly.rfind(".txt");
            if (pos != string::npos) {
                addrFileNameOnly = addrFileNameOnly.substr(0, pos);
            }

            // ��������� ��� ��������� ����� (� ����)
            string baseFilename = folderPath + "�����_" + corrFileNameOnly + "_" + addrFileNameOnly;
            string filename = baseFilename + ".txt";
            int i = 1;
            while (filesystem::exists(filename)) { // ���������, ���������� �� ����
                filename = baseFilename + "(" + to_string(i++) + ")" + ".txt"; // ��������� (1), (2) � �.�.
            }

            try { //������� ��������� ����������

                ofstream outfile(filename); // ������ ofstream *�����* ������������ ����� �����

                if (!outfile.is_open()) {
                    cerr << "������: �� ������� ������� ���� ��� ������.\n";
                    break;
                }

                set<string> printedOrganizations; // ��������� ��� ������������ ��� ���������� �����������
                vector<string> outputBuffer;       //������ ��� ����������� ������

                ifstream addrFile(addressesFilename); // ��������� ���� �������
                if (addrFile.is_open()) {
                    string line;
                    while (getline(addrFile, line)) {
                        stringstream ss(line);
                        string orgName, address, contactPerson;
                        getline(ss, orgName, ';');
                        getline(ss, address, ';');
                        getline(ss, contactPerson, ';');

                        orgName.erase(0, orgName.find_first_not_of(" \t"));  //������� �������
                        orgName.erase(orgName.find_last_not_of(" \t") + 1);

                        //���� ����������� �� �����������, �� ������� � ��� ����������
                        if (printedOrganizations.find(orgName) == printedOrganizations.end()) {

                            processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);

                        }
                    }
                    addrFile.close();
                }
                else {
                    cerr << "������: �� ������� ������� ���� �������: " << addressesFilename << ".\n";
                    break;
                }

                outfile.close();
                cout << "���������� ������� �������� � ���� " << filename << ".\n";
                break;
            }
            catch (const exception& e)
            {
                cerr << "������: �� ������� ������� ��� �������� ����.\n";
                cerr << e.what() << endl;
                break;
            }
        }
        case 27:
            return;  // ����� �� runProgram
        }
    }
}

void menu() {
    string correspondenceFilename, addressesFilename, outputFilename;
    string escPressed;
    bool folderPathSet = true; // ����, ��� ���� � ����� ����������
    currentFolderPath = normalizePath(filesystem::current_path().string()) + "\\"; // ����� �����������
    const string instructionsFile = currentFolderPath + "instructions.txt";

    cout << "\n��������� ��� ��������� ������ � ��������������� � ������� �����������.\n";
    cout << "��� ��������� ������ ���������� �� ������������ � �� ���������������, � ����� ����������� ������.\n";

    while (true) {
        system("cls"); // ������� �����

        cout << "*************************************************************\n";
        cout << "* ��������� ��� ��������� ������ ��������������� � �������  *\n";
        cout << "* ��������� ��������� ������ � ������������ ���������� ��   *\n";
        cout << "* ������������ �� ������ ���� ������ -                      *\n";
        cout << "* ��������� ��������������� (IC_***.txt)                    *\n";
        cout << "* ������� ����������� (AO_***.txt)                          *\n";
        cout << "*************************************************************\n\n";

        cout << "������� ����:\n";
        cout << "1. ������ ������\n";
        cout << "2. �������� ���� � �����\n";
        cout << "3. ����������\n\n";
        cout << "������� ����: " << currentFolderPath << "\n\n";
        cout << "Esc - �����\n\n";

        int mainChoice = _getch(); // ��������� ������
        char mainChoiceChar = static_cast<char>(mainChoice); // char

        switch (mainChoice) {
        case '1': { // "������ ������"
            // �������� ����� ������ �� ������������
            pair<string, string> filenames = getFilenamesFromUser(currentFolderPath);
            if (filenames.first.empty() && filenames.second.empty()) {
                break; // ���� ������������ ����� Esc, ������� �� case '1'
            }
            correspondenceFilename = filenames.first;
            addressesFilename = filenames.second;
            runProgram(currentFolderPath, correspondenceFilename, addressesFilename, outputFilename);
            break;
        }
        case '2': { // "�������� ���� � �����"
            string newPath = getLineWithEsc("������� ����� ���� � �����: ");
            if (newPath.empty()) break;
            newPath = normalizePath(newPath); // ����������� ����

            struct stat buffer;

            // �������� �� ��, ���������� �� ����� � �������� �� ��� �����������
            if (stat(newPath.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR)) {
                if (newPath.back() != '\\') // �������� ���� �� \ � ����� ����
                    currentFolderPath = newPath + "\\"; // ��������� �����������
                cout << "���� ������� �������.\n";
            }
            else {
                cerr << "������: ��������� ���� �� ���������� ��� �� ��������.\n";
            }
            cout << "������� ����� ������� ��� �����������...\n"; // ��������� ���������
            _getch(); // ��� ������� �������
            break;
        }

        case '3': // ����������
            readInstructionsFromFile(instructionsFile);
            break;

        case 27: // Esc - �����
            return;

        default:
            continue; // ������������ ���� - ���������� ����
        }
    }
}

string normalizePath(const string& path) {
    namespace fs = std::filesystem;
    fs::path normalizedPath = fs::absolute(path);  // �������� ���������� ����
    return normalizedPath.string(); // ���������� ��� ������
}