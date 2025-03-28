//Lab_prog_2.cpp
#include "Lab_prog_2.h"
#define _CRT_SECURE_NO_WARNINGS // ���������� �������������� ������������ CRT
#pragma warning(disable:4996) // ���������� ����������� �������������� C4996

using namespace std;

// --- ���������� ���������� ---
char folder_way[256] = { 0 }; // ���� � ������� �����
const char* file_extension = ".txt"; // ���������� ������ ������
const char* ocfe = "IC_"; // ������� ������ ���������������
const char* oa = "AO_";   // ������� ������ �������

// --- ������� ��������� �������� ---
bool isValidFileNameChar(char c) { // �������� ������� ��� ����� �����
    unsigned char uc = (unsigned char)c;
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

bool isValidPathChar(char c) { // �������� ������� ��� ����
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

bool isValidOrgNameChar(char c) { // �������� ������� ��� �������� �����������
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // ���.+���.
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // ���. �������: - _ . , " ( ) �(211) + ! & : �(171) �(187) #
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#');
}

bool isValidAddressChar(char c) { // �������� ������� ��� ������
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // ���. �������: - , . / �(211)
    return (is_letter || is_digit || is_space || uc == '-' || uc == ',' || uc == '.' || uc == '/' || uc == 211);
}

bool isValidContactPersonChar(char c) { // �������� ������� ��� ��� ������������
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_space = (uc == ' ');
    // ���. �������: -
    return (is_letter || is_space || uc == '-');
}

bool isValidCorrTypeChar(char c) { // �������� ������� ��� ���� ���������������
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // ���. �������: - _ . , ( )
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '(' || uc == ')');
}

bool isValidCorrDateChar(char c) { // �������� ������� ��� ���� ��������������� (��������� ����� ��� "��� ������")
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192);
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // ���. �������: .
    return (is_letter || is_digit || is_space || uc == '.');
}

// --- ������� �����/������ � ������ � ������ ---

// ��������� ������ � ������� � ����������� �������� � ���������� ������ ������ (Ctrl+V)
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction); // ����� �����������
    int i = 0;
    buffer[0] = '\0';
    while (true) {
        int key = _getch();
        if (key == 22) { // CTRL V
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        // ���������� ���������� ������� isValidCharFunc ��� �������� ����������� ��������
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            if (isValidCharFunc(pszText[j])) { //   �������� �����
                                buffer[i++] = pszText[j];
                                printf("%c", pszText[j]);
                            }
                        }
                        GlobalUnlock(hData);
                    }
                }
                CloseClipboard();
            }
        }
        else if (key == 0 || key == 224) { // �������� �� �����������
            if (_kbhit()) {
                _getch();
                continue;
            }
        }
        if (key == 27) { // Esc
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        else if (key == '\r') { // Enter
            buffer[i] = '\0';
            printf("\n");
            return;
        }
        else if (key == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                buffer[i] = '\0';
            }
        }
        else {
            char c = (char)key;
            // ���������� ���������� ������� isValidCharFunc ��� �������� �������� ��������
            if (isValidCharFunc(c)) { //   �������� �����
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
        }
    }
}


// ������������ ����: ����������, ��� ������� ������, � '\' �� �����
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path == nullptr || path[0] == '\0') { // ���� ���� ����, ����� ������� ����������
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "�� ������� ���������� ������� �����.\n");
            normalized_path[0] = '\0';
        }
        size_t len = strlen(normalized_path); // ��������� '\' � �����, ���� ���
        if (len > 0 && normalized_path[len - 1] != '\\' && len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        return;
    }

    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) { // �������� ������ ����
        SAFE_STRCPY(normalized_path, path, normalized_path_size); // ���� �� �����, �������� ��� ����
    }

    int j = 0; // �������� ������� ������
    bool last_was_slash = false;
    for (int i = 0; normalized_path[i] != '\0'; ++i) {
        if (normalized_path[i] == '\\') {
            if (!last_was_slash) normalized_path[j++] = normalized_path[i];
            last_was_slash = true;
        }
        else {
            normalized_path[j++] = normalized_path[i];
            last_was_slash = false;
        }
    }
    normalized_path[j] = '\0';

    size_t len = strlen(normalized_path); // ��������� '\' � �����, ���� ���
    if (len > 0 && normalized_path[len - 1] != '\\') {
        if (len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        else {
            fprintf(stderr, "��������������: ���� ������� ������� ��� ���������� '\\'.\n");
        }
    }
}

// ����� � �������� ���� � �����
void selectFolderPath() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251); // ��������� ���������
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256];

    // ������ ���� � ����������
    getLineWithRestrictedChars("������� ���� � ����� (��� �������� ������ ��� �������, Esc ��� ������): ",
        folder_way_new, sizeof(folder_way_new), isValidPathChar);

    if (folder_way_new[0] == '\0' && strlen(folder_way_new) == 0) { // �������� �� ������ (Esc ��� Enter �� ������ ����)
        printf("����� ���� �������.\n");
        if (folder_way[0] == '\0') normalizePath("", folder_way, sizeof(folder_way)); // ���������� �������, ���� ������ �� ���� �������
        else printf("������������ ��������� ���������� ����: %s\n", folder_way); // ����� ��������� ������
        cout << "������� ����� ������� ��� �����������..." << endl; _getch();
        return;
    }

    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp)); // ������������ ���������� ����

    struct stat path_stat; // ��������� ��� ���������� � �����/�����
    if (stat(normalized_path_temp, &path_stat) == 0) { // �������� ������������� ����
        if (path_stat.st_mode & S_IFDIR) { // ��������, �������� �� ���� �����������
            char test_file_path[512]; // ������� �������/������� ���� ��� �������� ���� �� ������
            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", normalized_path_temp);
            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) { // ����� ����
                fclose(test_file);
                remove(test_file_path);
                printf("���� � ����� ������: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way)); // ��������� ����
            }
            else { // ��� ���� �� ������
                printf("������: ��� ���� ��� ������ � ��������� �����.\n");
                if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
        }
        else { // ���� ����������, �� ��� �� �����
            printf("������: ��������� ���� �� �������� ������.\n");
            if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }
    else { // ���� �� ���������� ��� ����������
        printf("������: ��������� ���� �� ���������� ��� ����������.\n");
        // ���. �������� �� ������, ���� stat �� �������� ��-�� '\' �� �����
        size_t len = strlen(normalized_path_temp);
        if (len > 1 && normalized_path_temp[len - 1] == '\\') {
            normalized_path_temp[len - 1] = '\0'; // ������� ����
            if (stat(normalized_path_temp, &path_stat) == 0 && (path_stat.st_mode & S_IFDIR)) {
                normalized_path_temp[len - 1] = '\\'; // ���������� ����
                printf("���� � ����� ������ (����� ���. ��������): %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
            else { // ���� ��� ����� ��������
                if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
        }
        else { // ���� ����� ��������
            if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }
    cout << "������� ����� ������� ��� �����������..." << endl; _getch();
}

// --- ������� �������� ������ ---

// ������ � ����� ���������� �� �����
void readInstructionsFromFile(const string& filename) {
    system("cls"); // ������� ������
    FILE* file = fopen(filename.c_str(), "r"); // �������� ����� ����������
    if (file) { // ���� ���� ������
        char line[256];
        while (fgets(line, sizeof(line), file)) { // ������ � ����� �����
            printf("%s", line);
        }
        fclose(file);
    }
    else { // ������ �������� �����
        printf("�� ������� ������� ���� � �����������: %s\n", filename.c_str());
        char current_path[256]; // ������� �������� ������� ����� ���������
        if (_getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("������� ����� ���������: %s\n", current_path);
        }
        else {
            printf("�� ������� ���������� ������� ����� ���������.\n");
        }
    }
    printf("\n������� ����� ������� ��� �����������...\n");
    _getch();
    system("cls"); // ������� ������
}

// �������� �������� � ������ � ����� ������
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// ��������� ������ ��� ����� �����������: ����� � ������ � ������������ ������
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile, addrFile;
    vector<Address> addresses;
    vector<Correspondence> correspondences;
    bool addressFound = false, correspondenceFound = false;
    string addrFileError = "", corrFileError = "";

    // ������ ��� ������ � ����� ��� ����
    auto writeToTarget = [&](const string& str) {
        if (selectiveOutput) outputBuffer.push_back(str);
        else if (outfile) *outfile << str << endl;
        };

    // --- ����� � ����� ������� ---
    addrFile.open(addrFilename);
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            if (!(getline(ss, currentOrg, ';') && getline(ss, address, ';') && getline(ss, contactPerson, ';'))) continue; // ������� ������������ �����
            currentOrg = trim(currentOrg); // ������� ������� � �����
            if (!currentOrg.empty() && currentOrg == orgName) { // ���������� � ������� ������
                address = trim(address);
                contactPerson = trim(contactPerson);
                // ���������, ������� ������ ���� �� "��� ������"
                addresses.push_back({ currentOrg, address.empty() ? "��� ������" : address, contactPerson.empty() ? "��� ������" : contactPerson });
                addressFound = true;
            }
        }
        addrFile.close();
    }
    else {
        addrFileError = "������: �� ������� ������� ���� �������: " + addrFilename; // ��������� ������
    }

    // --- ����� � ����� ��������������� ---
    corrFile.open(corrFilename);
    if (corrFile.is_open()) {
        string line;
        while (getline(corrFile, line)) {
            stringstream ss(line);
            string type, date, currentOrg;
            if (!(getline(ss, type, ';') && getline(ss, date, ';') && getline(ss, currentOrg, ';'))) continue;
            currentOrg = trim(currentOrg);
            if (!currentOrg.empty() && currentOrg == orgName) {
                type = trim(type);
                date = trim(date);
                correspondences.push_back({ type.empty() ? "��� ������" : type, date.empty() ? "��� ������" : date, currentOrg });
                correspondenceFound = true;
            }
        }
        corrFile.close();
    }
    else {
        corrFileError = "������: �� ������� ������� ���� ���������������: " + corrFilename;
    }

    // --- ����� ���������� ---
    if (selectiveOutput) { // ����� ������ (� �����/�������)
        if (!addrFileError.empty()) writeToTarget(addrFileError); // ����� ������ ������
        if (!corrFileError.empty()) writeToTarget(corrFileError);
        if (addressFound || correspondenceFound || !addrFileError.empty() || !corrFileError.empty()) { // ���� ���� ��� ��������
            writeToTarget("�������� �����������: " + orgName);
            if (addressFound) { // ����� �������
                for (const auto& addr : addresses) {
                    writeToTarget("  �����: " + addr.address);
                    writeToTarget("  ������� ������������: " + addr.contactPerson);
                }
            }
            else if (addrFileError.empty()) { // ����� �� ������, �� ������ �� ����
                writeToTarget("  �����: ��� ������");
                writeToTarget("  ������� ������������: ��� ������");
            }
            writeToTarget("  ���������������:"); // ����� ���������������
            if (correspondenceFound) {
                for (const auto& corr : correspondences) {
                    writeToTarget("\t- ���: " + corr.type + ", ����: " + corr.date);
                }
            }
            else if (corrFileError.empty()) { // ��������������� �� �������, ������ �� ����
                writeToTarget("\t- ���: ��� ������, ����: ��� ������");
            }
            writeToTarget("--------------------"); // �����������
        }
    }
    else { // ����� ������ (� ����)
        if (printedOrganizations.count(orgName)) return; // ������� ����������
        printedOrganizations.insert(orgName); // �������� ����������� ��� ����������

        writeToTarget("�������� �����������: " + orgName);
        if (addressFound) { // ����� �������
            for (const auto& addr : addresses) {
                writeToTarget("  �����: " + addr.address);
                writeToTarget("  ������� ������������: " + addr.contactPerson);
            }
        }
        else if (addrFileError.empty()) { // ����� �� ������, ������ �� ����
            writeToTarget("  �����: ��� ������");
            writeToTarget("  ������� ������������: ��� ������");
        }
        else {
            writeToTarget("  " + addrFileError); // ����� ������ ������
        }

        if (!corrFileError.empty()) writeToTarget("  " + corrFileError); // ����� ������ ���������������

        writeToTarget("  ���������������:"); // ����� ���������������
        if (correspondenceFound) {
            for (const auto& corr : correspondences) {
                writeToTarget("\t- ���: " + corr.type + ", ����: " + corr.date);
            }
        }
        else if (corrFileError.empty()) { // ��������������� �� �������, ������ �� ����
            writeToTarget("\t- ���: ��� ������, ����: ��� ������");
        }
        writeToTarget("--------------------"); // �����������
    }
}

// ������ ���� ������ � ������������
pair<string, string> getFilenamesFromUser(const char* folderPath) {
    char corrBase[256], addrBase[256];
    char fullCorr[512], fullAddr[512];
    cout << "\n��� ������ ����� ����� �������� ���� ������ � ������� Enter, ��� ������� Esc.\n\n";

    while (true) { // ������ ����� ���������������
        getLineWithRestrictedChars("������� ��� ����� ��������� ��������������� (��� IC_ � .txt): ", corrBase, sizeof(corrBase), isValidFileNameChar);
        if (strlen(corrBase) == 0) return { "", "" }; // ������
        snprintf(fullCorr, sizeof(fullCorr), "%s%s%s%s", folderPath, ocfe, corrBase, file_extension); // ������������ ������� ����
        if (!filesystem::exists(fullCorr)) { // �������� �������������
            cerr << "������: ���� " << fullCorr << " �� ����������.\n���������� ��� ���.\n";
            continue;
        }
        break; // ���� ������
    }

    while (true) { // ������ ����� �������
        getLineWithRestrictedChars("������� ��� ����� ������� ����������� (��� AO_ � .txt): ", addrBase, sizeof(addrBase), isValidFileNameChar);
        if (strlen(addrBase) == 0) return { "", "" }; // ������
        snprintf(fullAddr, sizeof(fullAddr), "%s%s%s%s", folderPath, oa, addrBase, file_extension); // ������������ ������� ����
        if (!filesystem::exists(fullAddr)) { // �������� �������������
            cerr << "������: ���� " << fullAddr << " �� ����������.\n���������� ��� ���.\n";
            continue;
        }
        break; // ���� ������
    }

    return { string(fullCorr), string(fullAddr) }; // ������� ������ �����
}

// �������� ������� ���������: ����� ������, �����/��������� ������, �����
void runProgram(const char* folderPath, const string& correspondenceFilename, const string& addressesFilename, string& outputFilename) {
    while (true) { // ���� �������
        cout << "\n�������� ����� ������:\n"
            << "1. ����� �� ������ � ����� � �������\n"
            << "2. ������ ����� � ����\n"
            << "Esc - ��������� � ������� ����\n";
        char choice = _getch();
        cout << endl;

        switch (choice) {
        case '1': { // ����� 1: ������������� �����
            cout << "\n�������� ���� ��� ������:\n"
                << "1. �������� �����������\n"
                << "2. ����� �����������\n"
                << "3. ������� ������������\n"
                << "4. ��� ���������������\n"
                << "5. ���� ����������\n"
                << "Esc - ������\n";

            char searchFieldChoice;
            int searchFieldIndex;
            bool (*validatorFunc)(char) = nullptr;
            string searchFieldName = "", fileToScan = "";
            cout << endl;
            while (true) {

                searchFieldChoice = _getch();
                searchFieldIndex = searchFieldChoice - '0';

                if (searchFieldChoice == 27) break; // ������

                switch (searchFieldChoice) {
                case '1': validatorFunc = isValidOrgNameChar; searchFieldName = "�������� �����������"; break;
                case '2': validatorFunc = isValidAddressChar; searchFieldName = "����� �����������"; fileToScan = addressesFilename; break;
                case '3': validatorFunc = isValidContactPersonChar; searchFieldName = "������� ������������"; fileToScan = addressesFilename; break;
                case '4': validatorFunc = isValidCorrTypeChar; searchFieldName = "��� ���������������"; fileToScan = correspondenceFilename; break;
                case '5': validatorFunc = isValidCorrDateChar; searchFieldName = "���� ����������"; fileToScan = correspondenceFilename; break;
                default: continue;
                }
                break;
            }
            
            char searchQueryBuffer[256];
            getLineWithRestrictedChars(("������� " + searchFieldName + " ��� ������ (Esc - ������): ").c_str(),
                searchQueryBuffer, sizeof(searchQueryBuffer), validatorFunc);
            if (searchQueryBuffer[0] == '\0') break;

            string searchQuery = trim(string(searchQueryBuffer));
            if (searchQuery.empty()) {
                printf("��������� ������ �� ����� ���� ������.\n");
                break;
            }

            vector<string> matchingRecords;
            set<string> foundOrganizations;

            // --- ������������ ����� ---
            if (searchFieldIndex == 1) {
                foundOrganizations.insert(searchQuery);
                bool recordFound = false; ifstream tempFile;
                tempFile.open(addressesFilename);
                if (tempFile.is_open()) { string line; while (getline(tempFile, line)) { stringstream ss(line); string org, f1, f2; if (getline(ss, org, ';') && getline(ss, f1, ';') && getline(ss, f2, ';') && trim(org) == searchQuery) { matchingRecords.push_back("�������� �����������: " + trim(org)); recordFound = true; break; } } tempFile.close(); }
                if (!recordFound) {
                    tempFile.open(correspondenceFilename);
                    if (tempFile.is_open()) { string line; while (getline(tempFile, line)) { stringstream ss(line); string f1, f2, org; if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, org, ';') && trim(org) == searchQuery) { matchingRecords.push_back("�������� �����������: " + trim(org)); recordFound = true; break; } } tempFile.close(); }
                }
            }
            else {
                ifstream inputFile(fileToScan);
                if (!inputFile.is_open()) {
                    cerr << "������: �� ������� ������� ����: " << fileToScan << endl;
                }
                else {
                    string line;
                    while (getline(inputFile, line)) {
                        stringstream ss(line); string orgName, f1, f2, fieldToCompare, matchInfo = "";
                        if (fileToScan == addressesFilename) {
                            if (getline(ss, orgName, ';') && getline(ss, f1, ';') && getline(ss, f2, ';')) {
                                orgName = trim(orgName);
                                if (searchFieldIndex == 2) { fieldToCompare = trim(f1); if (!fieldToCompare.empty() && fieldToCompare.find(searchQuery) != string::npos) { foundOrganizations.insert(orgName); matchInfo = "�����: " + f1 + ", �������� �����������: " + orgName; } }
                                else if (searchFieldIndex == 3) { fieldToCompare = trim(f2); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "������� ������������: " + f2 + ", �������� �����������: " + orgName; } }
                            }
                        }
                        else {
                            if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, orgName, ';')) {
                                orgName = trim(orgName);
                                if (searchFieldIndex == 4) { fieldToCompare = trim(f1); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "���: " + f1 + ", ����: " + trim(f2) + ", �������� �����������: " + orgName; } }
                                else if (searchFieldIndex == 5) { fieldToCompare = trim(f2); if (!fieldToCompare.empty() && fieldToCompare == searchQuery) { foundOrganizations.insert(orgName); matchInfo = "����: " + f2 + ", ���: " + trim(f1) + ", �������� �����������: " + orgName; } }
                            }
                        }
                        if (!matchInfo.empty()) matchingRecords.push_back(matchInfo);
                    }
                    inputFile.close();
                }
            }

            // --- ����� ����������� ������ (��������!) ---
            vector<string> outputBuffer; // ����� ��� ������ ������ ����������
            set<string> dummyPrintedSet; // �������� ��� processOrganization

            // �������� ������ ���������� ��� ���� ��������� ������������
            for (const string& orgNameToProcess : foundOrganizations) {
                processOrganization(orgNameToProcess, correspondenceFilename, addressesFilename, true, nullptr, dummyPrintedSet, outputBuffer);
            }

            // ���������, ������� �� �����-���� ������
            bool dataFound = !outputBuffer.empty() || !matchingRecords.empty();

            if (!dataFound) {
                cout << "\n�� ���������� ���� ������� \"" << searchQuery << "\" ������ �� �������." << endl;
            }
            else {
                // ������ ������ � ����������� �� ���� ������
                if (searchFieldIndex == 4 || searchFieldIndex == 5) { // ��� ��� ����
                    cout << "\n�� ���������� ���� ������� " << searchFieldName << " \"" << searchQuery << "\" ������� ��������� ������:\n" << endl;
                    for (const string& record : matchingRecords) cout << record << endl;
                    cout << "\n��������� �������� � �� ������:\n" << endl;
                    for (const string& line : outputBuffer) cout << line << endl;
                }
                else if (searchFieldIndex == 2 || searchFieldIndex == 3) { // ����� ��� ������������
                    cout << "\n��������� �������� � �� ������:\n" << endl;
                    for (const string& line : outputBuffer) cout << line << endl;
                }
                else if (searchFieldIndex == 1) { // �������� �����������
                    // ������ ������� buffer, ��������� "��������� ��������..." �� �����
                    cout << endl; // ������ ����� �������
                    for (const string& line : outputBuffer) cout << line << endl;
                }
            }
            // ������ �����
            break; // ����� �� case '1' � ������� � ������ ����� while(true)
        }
        case '2': { // ����� 2: ������ ����� � ����
            // ������������ ����� ����� ������
            string corrName = filesystem::path(correspondenceFilename).filename().string();
            string addrName = filesystem::path(addressesFilename).filename().string();
            size_t pos = corrName.rfind(".txt"); if (pos != string::npos) corrName = corrName.substr(0, pos);
            pos = addrName.rfind(".txt"); if (pos != string::npos) addrName = addrName.substr(0, pos);
            string baseFilename = string(folderPath) + "�����_" + corrName + "_" + addrName;
            string filename = baseFilename + ".txt"; int i = 1;
            while (filesystem::exists(filename)) filename = baseFilename + "(" + to_string(i++) + ").txt";

            try {
                ofstream outfile(filename);
                if (!outfile.is_open()) {
                    cerr << "������: �� ������� �������/������� ���� ��� ������: " << filename << "\n";
                    break;
                }

                set<string> allUniqueOrgNames;
                set<string> printedOrganizations;
                ifstream fileReader; string line;

                // ������ ���� �� ����� �������
                fileReader.open(addressesFilename);
                if (fileReader.is_open()) { while (getline(fileReader, line)) { stringstream ss(line); string org; if (getline(ss, org, ';')) { org = trim(org); if (!org.empty()) allUniqueOrgNames.insert(org); } } fileReader.close(); }
                else { cerr << "��������������: �� ������� ������� ���� �������: " << addressesFilename << endl; outfile << "!!! ������ ������ ����� �������: " << addressesFilename << " !!!" << endl << endl; }

                // ������ ���� �� ����� ���������������
                fileReader.clear();
                fileReader.open(correspondenceFilename);
                if (fileReader.is_open()) { while (getline(fileReader, line)) { stringstream ss(line); string f1, f2, org; if (getline(ss, f1, ';') && getline(ss, f2, ';') && getline(ss, org, ';')) { org = trim(org); if (!org.empty()) allUniqueOrgNames.insert(org); } } fileReader.close(); }
                else { cerr << "��������������: �� ������� ������� ���� ���������������: " << correspondenceFilename << endl; outfile << "!!! ������ ������ ����� ���������������: " << correspondenceFilename << " !!!" << endl << endl; }

                // ��������� � ������ � ����
                vector<string> dummyOutputBuffer;
                for (const string& orgNameToProcess : allUniqueOrgNames) {
                    processOrganization(orgNameToProcess, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, dummyOutputBuffer);
                }
                outfile.close();
                cout << "����� ������� ����������� � ������� � ����: " << filename << ".\n";
            }
            catch (const exception& e) {
                cerr << "������ �������� ��������: " << e.what() << endl;
            }
            // ������ �����
            break; // ����� �� case '2' � ������� � ������ ����� while(true)
        }
        case 27: return; // Esc - ����� �� ������� (������� � ������� ����)
        default:
            cout << "�������� �����. ����������, ����������� 1, 2 ��� Esc." << endl;
            break;
        }
    }
}

// --- ������� ���� ---
void menu() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251); // ��������� ���������
    if (folder_way[0] == '\0') normalizePath("", folder_way, sizeof(folder_way)); // ��������� �������� ����, ���� �� ������

    while (true) { // ������� ���� ����
        system("cls"); // ������� ������
        // ����������� ��������� � ����
        printf("*************************************************************\n"
            "*    ��������� ��������� ������ ��������������� � �������   *\n"
            "*************************************************************\n"
            "* ����������� �����:\n"
            "* - ��������� ��������������� (������: %s*%s)\n"
            "* - ������ �����������        (������: %s*%s)\n"
            "*************************************************************\n\n"
            "������� ����:\n"
            "1. ������ ������\n"
            "2. �������� ���� � ����� � �������\n"
            "3. �������� ����������\n\n"
            "������� ���� � �����: %s\n\n"
            "Esc - ����� �� ���������\n\n",
            ocfe, file_extension, oa, file_extension, (folder_way[0] == '\0' ? "�� ������" : folder_way));

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': { // ������ ������
            printf("\n"); 
            if (folder_way[0] == '\0') { printf("������: ������� ���������� ������� ���� � ����� (����� 2).\n"); printf("������� ����� �������..."); _getch(); break; }
            pair<string, string> filenames = getFilenamesFromUser(folder_way); // ������ ���� ������
            if (filenames.first == "" || filenames.second == "")
                break;
            string dummyOutput;
            runProgram(folder_way, filenames.first, filenames.second, dummyOutput); // ������ �������� ������
            break; // ������� � ���� ����� runProgram
        }
        case '2': { // �������� ����
            printf("\n"); 
            selectFolderPath();
            break;
        }
        case '3': { // ����������
            printf("\n"); 
            readInstructionsFromFile(Constants::INSTRUCTIONS_FILE);
            break;
        }
        case 27: { // Esc - �����
            printf("\n���������� ������ ���������.\n");
            return; // ����� �� menu() � ���������
        }
        default: { // �������� �����
            break;
        }
        }
    }
}