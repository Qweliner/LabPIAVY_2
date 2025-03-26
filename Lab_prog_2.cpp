//Lab_prog_2.cpp
#include "Lab_prog_2.h"
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

// --- ���������� ���������� (�����������) ---
char folder_way[256] = { 0 };
const char* file_extension = ".txt";
const char* ocfe = "IC_";
const char* oa = "AO_";

// --- ���������� ������� �������� �������� ---

// ������� �������� ������� ��� ����� �����
bool isValidFileNameChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '\\' && uc != '/' && uc != ':' && uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// ������� �������� ������� ��� ����
bool isValidPathChar(char c) {
    unsigned char uc = (unsigned char)c;
    return (uc != '*' && uc != '?' && uc != '"' && uc != '<' && uc != '>' && uc != '|');
}

// ������� �������� ������� ��� �������� ����������� (�� ������ ���������)
bool isValidOrgNameChar(char c) { // <-- ���������� ����� �������
    unsigned char uc = (unsigned char)c;
    bool is_letter = (uc >= 'A' && uc <= 'Z') || (uc >= 'a' && uc <= 'z') || (uc >= 192); // �������� + ��������� (�����)
    bool is_digit = (uc >= '0' && uc <= '9');
    bool is_space = (uc == ' ');
    // ������ ������ �� openFileForAppend ��� ���� "�������� �����������"
    return (is_letter || is_digit || is_space || uc == '-' || uc == '_' || uc == '.' || uc == ',' || uc == '"' || uc == '(' || uc == ')' || uc == 211 || uc == '+' || uc == '!' || uc == '&' || uc == ':' || uc == 171 || uc == 187 || uc == '#'); // �(211), �(171), �(187)
}

// --- ���������� ������� ����� � ������ � ����� ---

// ������� ��� ��������� ������ � ������� � ����������� ��������
// ��������� ��������� �� ������� �������� isValidCharFunc
void getLineWithRestrictedChars(const char* instruction, char* buffer, int buffer_size, bool (*isValidCharFunc)(char)) {
    printf("%s", instruction);
    int i = 0;
    buffer[0] = '\0';

    while (true) {
        int key = _getch();
        if (key == 0 || key == 224) {
            if (_kbhit()) {
                _getch();
                continue;
            }
        }
        if (key == 27) {
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        else if (key == '\r') {
            buffer[i] = '\0';
            printf("\n");
            return;
        }
        else if (key == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
                buffer[i] = '\0';
            }
        }
        else if (key == 22) {
            if (OpenClipboard(NULL)) {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData != NULL) {
                    char* pszText = (char*)GlobalLock(hData);
                    if (pszText != NULL) {
                        // ���������� ���������� ������� isValidCharFunc ��� �������� ����������� ��������
                        for (int j = 0; pszText[j] != '\0' && i < buffer_size - 1; ++j) {
                            if (isValidCharFunc(pszText[j])) { // <--- �������� �����
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
        else {
            char c = (char)key;
            // ���������� ���������� ������� isValidCharFunc ��� �������� �������� ��������
            if (isValidCharFunc(c)) { // <--- �������� �����
                if (i < buffer_size - 1) {
                    buffer[i] = c;
                    printf("%c", c);
                    i++;
                }
            }
        }
    }
}

// ������� ������������ ���� (��� ���������)
void normalizePath(const char* path, char* normalized_path, size_t normalized_path_size) {
    if (path[0] == '\0') {
        if (_getcwd(normalized_path, normalized_path_size) == NULL) {
            fprintf(stderr, "�� ������� ���������� ������� �����.\n");
            normalized_path[0] = '\0';
        }
        size_t len = strlen(normalized_path);
        if (len > 0 && normalized_path[len - 1] != '\\' && len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        return;
    }

    if (_fullpath(normalized_path, path, normalized_path_size) == NULL) {
        SAFE_STRCPY(normalized_path, path, normalized_path_size);
    }

    int j = 0;
    bool last_was_slash = false;
    for (int i = 0; normalized_path[i] != '\0'; ++i) {
        if (normalized_path[i] == '\\') {
            if (!last_was_slash) {
                normalized_path[j++] = normalized_path[i];
            }
            last_was_slash = true;
        }
        else {
            normalized_path[j++] = normalized_path[i];
            last_was_slash = false;
        }
    }
    normalized_path[j] = '\0';

    size_t len = strlen(normalized_path);
    if (len > 0 && normalized_path[len - 1] != '\\') {
        if (len < normalized_path_size - 1) {
            normalized_path[len] = '\\';
            normalized_path[len + 1] = '\0';
        }
        else {
            fprintf(stderr, "��������������: ���� ������� ������� ��� ���������� ������������ '\\'.\n");
        }
    }
}

// ������� ������ ���� � �����
void selectFolderPath() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    char folder_way_new[256] = { 0 };
    char normalized_path_temp[256];

    // ���������� isValidPathChar ��� �������� ����
    getLineWithRestrictedChars("������� ���� � �����: ", folder_way_new, sizeof(folder_way_new), isValidPathChar); // <--- �������� �������

    if (folder_way_new[0] == '\0') {
        return;
    }

    normalizePath(folder_way_new, normalized_path_temp, sizeof(normalized_path_temp));

    struct stat path_stat;
    if (stat(normalized_path_temp, &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) {
            char test_file_path[512];
            snprintf(test_file_path, sizeof(test_file_path), "%stest_access.tmp", normalized_path_temp);

            FILE* test_file = fopen(test_file_path, "w");
            if (test_file) {
                fclose(test_file);
                remove(test_file_path);
                printf("���� � ����� ������: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
            else {
                printf("��� ���� ��� ������� � ��������� �����. �������� ������ ����� ��� ���������� � ��������������.\n");
                if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
        }
        else {
            printf("��������� ���� �� �������� ������.\n");
            if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }
    else {
        size_t len = strlen(normalized_path_temp);
        if (len > 0 && normalized_path_temp[len - 1] == '\\') {
            normalized_path_temp[len - 1] = '\0';
            if (stat(normalized_path_temp, &path_stat) != 0 || !(path_stat.st_mode & S_IFDIR)) {
                printf("��������� ���� �� ���������� ��� �� �������� ������. ��������� ������������ ���������.\n");
                if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
            }
            else {
                normalized_path_temp[len - 1] = '\\'; // ���������� ���� �������
                printf("���� � ����� ������: %s\n", normalized_path_temp);
                SAFE_STRCPY(folder_way, normalized_path_temp, sizeof(folder_way));
            }
        }
        else {
            printf("��������� ���� �� ����������. ��������� ������������ ���������.\n");
            if (folder_way[0] != '\0') printf("������������ ��������� ���������� ����: %s\n", folder_way);
        }
    }

    printf("��� ����������� ������� Enter.");
    // ������� ������ ����� ����� _getch
    int c; while ((c = getchar()) != '\n' && c != EOF);
}

// --- ���������� ������� �� ������ ��������� (��������������) ---

// readInstructionsFromFile (��� ���������)
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
        cerr << "����������, ��������� ���� " << Constants::INSTRUCTIONS_FILE << " � �����: " << folder_way << "\n";
    }
    cout << "\n������� ����� ������� ��� �������� � ������� ����...\n";
    _getch();
    system("mode con cols=120 lines=30");
}


// processOrganization (��� ���������)
void processOrganization(const string& orgName, const string& corrFilename, const string& addrFilename, bool selectiveOutput, ofstream* outfile, set<string>& printedOrganizations, vector<string>& outputBuffer) {
    ifstream corrFile;
    ifstream addrFile;

    vector<Address> addresses;
    vector<Correspondence> correspondences;
    bool addressFound = false;
    bool correspondenceFound = false;
    string addrFileError = "";
    string corrFileError = "";

    auto writeToTarget = [&](const string& str) {
        if (selectiveOutput) {
            outputBuffer.push_back(str);
        }
        else if (outfile) {
            *outfile << str << endl;
        }
        };

    // --- ���� 1: ����� � ����� ������� ---
    addrFile.open(addrFilename);
    if (addrFile.is_open()) {
        string line;
        while (getline(addrFile, line)) {
            stringstream ss(line);
            string currentOrg, address, contactPerson;
            if (!(getline(ss, currentOrg, ';') && getline(ss, address, ';') && getline(ss, contactPerson, ';'))) {
                continue;
            }

            size_t first = currentOrg.find_first_not_of(" \t");
            if (string::npos != first) {
                size_t last = currentOrg.find_last_not_of(" \t");
                currentOrg = currentOrg.substr(first, (last - first + 1));
            }
            else {
                currentOrg.clear();
            }

            if (!currentOrg.empty() && currentOrg == orgName) {
                first = address.find_first_not_of(" \t"); address = (first == string::npos) ? "" : address.substr(first);
                first = contactPerson.find_first_not_of(" \t"); contactPerson = (first == string::npos) ? "" : contactPerson.substr(first);
                if (!address.empty() && address.back() == ';') address.pop_back();
                if (!contactPerson.empty() && contactPerson.back() == ';') contactPerson.pop_back();

                addresses.push_back({ currentOrg, address.empty() ? "��� ������" : address, contactPerson.empty() ? "��� ������" : contactPerson }); // ����� ������ �������� ��� �������������
                addressFound = true;
            }
        }
        addrFile.close();
    }
    else {
        addrFileError = "�� ������� ������� ���� �������: " + addrFilename;
    }

    // --- ���� 2: ����� � ����� ��������������� ---
    corrFile.open(corrFilename);
    if (corrFile.is_open()) {
        string line;
        while (getline(corrFile, line)) {
            stringstream ss(line);
            string type, date, currentOrg;
            if (!(getline(ss, type, ';') && getline(ss, date, ';') && getline(ss, currentOrg, ';'))) {
                continue;
            }

            size_t first = currentOrg.find_first_not_of(" \t");
            if (string::npos != first) {
                size_t last = currentOrg.find_last_not_of(" \t");
                currentOrg = currentOrg.substr(first, (last - first + 1));
            }
            else {
                currentOrg.clear();
            }

            if (!currentOrg.empty() && currentOrg == orgName) {
                first = type.find_first_not_of(" \t"); type = (first == string::npos) ? "" : type.substr(first);
                first = date.find_first_not_of(" \t"); date = (first == string::npos) ? "" : date.substr(first);
                if (!type.empty() && type.back() == ';') type.pop_back();
                if (!date.empty() && date.back() == ';') date.pop_back();

                correspondences.push_back({ type.empty() ? "��� ������" : type, date.empty() ? "��� ������" : date, currentOrg }); // ����� ������ �������� ��� �������������
                correspondenceFound = true;
            }
        }
        corrFile.close();
    }
    else {
        corrFileError = "�� ������� ������� ���� ���������������: " + corrFilename;
    }

    // --- ���� 3: ����� ���������� ---

    if (selectiveOutput) { // ����� ������ (� �������)
        // ������� ������� ������ �������� ������, ���� ����
        if (!addrFileError.empty()) writeToTarget(addrFileError);
        if (!corrFileError.empty()) writeToTarget(corrFileError);

        // ���������, ���� �� ���� �����-�� ���������� ��� ������ ��� ������
        if (!addressFound && !correspondenceFound && addrFileError.empty() && corrFileError.empty()) {
            // ������ �� ������� � ������ �� ����
            writeToTarget("����������� \"" + orgName + "\" �� ������� �� � ����� �� ������.");
        }
        else {
            // ������� ��������� ���������� ��� ��������
            writeToTarget("�������� �����������: " + orgName);

            // ����� ���������� �� ������ (��� ��������)
            if (addressFound) {
                for (const auto& addr : addresses) { // ����� ���� ���������, ������� ��� ���������
                    writeToTarget("  �����: " + addr.address); // �������� ��� ������ ���������
                    writeToTarget("  ������� ������������: " + addr.contactPerson); // �������� ��� ������ ���������
                }
            }
            else if (addrFileError.empty()) { // ���� �������, �� ���������� ���
                writeToTarget("  �����: ��� ������");
                writeToTarget("  ������� ������������: ��� ������");
            }
            // ���� ���� ������ addrFileError, ��� ��� ��������

            // ����� ���������� � ��������������� (��� ��������)
            writeToTarget("  ���������������:"); // ������ ������� ���������
            if (correspondenceFound) {
                for (const auto& corr : correspondences) { // ����� ���� ���������, ������� ��� ���������
                    writeToTarget("\t- ���: " + corr.type + ", ����: " + corr.date); // �������� ��� ������ ���������
                }
            }
            else if (corrFileError.empty()) { // ���� �������, �� ���������� ���
                writeToTarget("\t- ���: ��� ������, ����: ��� ������");
            }
            // ���� ���� ������ corrFileError, ��� ��� ��������

            writeToTarget("--------------------");
        }
    }
    else { // ����� ������� ������ (� ����)
        // ������ ������� ������ �������� ��� ���������, ��� ��� ������ ���������
        // ������������ ��������� ������ (������� ������ � ��������� ����������).
        // ������������ - ������� �������� �� ������ ���� ��� ������, ����� ��������������� ������.
        if (printedOrganizations.count(orgName)) {
            return;
        }

        printedOrganizations.insert(orgName);
        writeToTarget("�������� �����������: " + orgName);

        if (addressFound) {
            for (const auto& addr : addresses) {
                writeToTarget("  �����: " + addr.address); // �������� ��� ������
                writeToTarget("  ������� ������������: " + addr.contactPerson); // �������� ��� ������
            }
        }
        else if (addrFileError.empty()) {
            writeToTarget("  �����: ��� ������");
            writeToTarget("  ������� ������������: ��� ������");
        }
        else {
            // ������� ������, ���� ����� �� ������ ��-�� ���
            writeToTarget("  " + addrFileError);
        }


        if (!corrFileError.empty()) {
            writeToTarget("  " + corrFileError);
        }
        writeToTarget("  ���������������:"); // ������� ���������
        if (correspondenceFound) {
            for (const auto& corr : correspondences) {
                writeToTarget("\t- ���: " + corr.type + ", ����: " + corr.date); // �������� ��� ������
            }
        }
        else if (corrFileError.empty()) {
            writeToTarget("\t- ���: ��� ������, ����: ��� ������");
        }
        // ���� ���� ������ corrFileError, ��� ��� ��������

        writeToTarget("--------------------");
    }
}

// getFilenamesFromUser (����������� ��� �������� ������ ������� ��������)
pair<string, string> getFilenamesFromUser(const char* folderPath) {
    char correspondenceFilenameBase[256];
    char addressesFilenameBase[256];
    char fullCorrFilename[512];
    char fullAddressesFilename[512];

    while (true) {
        // ���������� isValidFileNameChar ��� ����� �����
        getLineWithRestrictedChars("������� ��� ����� � ��������� ���������������� (��� �������� IC_ � ���������� .txt) ��� ������� Esc ��� ������: ",
            correspondenceFilenameBase, sizeof(correspondenceFilenameBase), isValidFileNameChar); // <--- �������� �������
        if (correspondenceFilenameBase[0] == '\0') {
            return { "", "" };
        }
        if (strlen(correspondenceFilenameBase) == 0) {
            printf("��� ����� �� ����� ���� ������.\n");
            continue;
        }

        snprintf(fullCorrFilename, sizeof(fullCorrFilename), "%s%s%s%s", folderPath, ocfe, correspondenceFilenameBase, file_extension);

        if (!filesystem::exists(fullCorrFilename)) {
            cerr << "������: ���� " << fullCorrFilename << " �� ����������.\n";
            cout << "����������, ���������� ��� ���.\n";
            continue;
        }
        break;
    }

    while (true) {
        // ���������� isValidFileNameChar ��� ����� �����
        getLineWithRestrictedChars("������� ��� ����� � �������� ����������� (��� �������� AO_ � ���������� .txt) ��� ������� Esc ��� ������: ",
            addressesFilenameBase, sizeof(addressesFilenameBase), isValidFileNameChar); // <--- �������� �������
        if (addressesFilenameBase[0] == '\0') {
            return { "", "" };
        }
        if (strlen(addressesFilenameBase) == 0) {
            printf("��� ����� �� ����� ���� ������.\n");
            continue;
        }

        snprintf(fullAddressesFilename, sizeof(fullAddressesFilename), "%s%s%s%s", folderPath, oa, addressesFilenameBase, file_extension);

        if (!filesystem::exists(fullAddressesFilename)) {
            cerr << "������: ���� " << fullAddressesFilename << " �� ����������.\n";
            cout << "����������, ���������� ��� ���.\n";
            continue;
        }
        break;
    }

    return { string(fullCorrFilename), string(fullAddressesFilename) };
}

// runProgram (����������� ��� �������� ������ ������� ��������)
void runProgram(const char* folderPath, const string& correspondenceFilename,
    const string& addressesFilename, string& outputFilename) {

    while (true) {
        cout << "\n�������� ����� ������:\n";
        cout << "1. ����� �� �������� ����������� � ����� � �������\n";
        cout << "2. ������ ����� � ����\n";
        cout << "Esc - ��������� � ������� ����\n";
        cout << "\n��� �����: ";

        char choice = _getch();
        if (choice != '1' && choice != '2' && choice != 27) {
            cout << "\n�������� �����. ���������� �����." << endl;
            continue;
        }
        cout << endl;

        switch (choice) {
        case '1': {
            char orgNameBuffer[256];
            // ���������� isValidOrgNameChar ��� �������� �����������
            getLineWithRestrictedChars("������� �������� ����������� ��� ������ ��� ������� Esc ��� ������: ",
                orgNameBuffer, sizeof(orgNameBuffer), isValidOrgNameChar); // <--- �������� �������

            if (orgNameBuffer[0] == '\0') {
                break;
            }
            if (strlen(orgNameBuffer) == 0) {
                printf("�������� ����������� �� ����� ���� ������.\n");
                cout << "������� ����� ������� ��� �����������..." << endl; _getch();
                break;
            }

            string orgName(orgNameBuffer);

            ofstream* outfile = nullptr;
            set<string> printedOrganizations;
            vector<string> outputBuffer;
            processOrganization(orgName, correspondenceFilename, addressesFilename,
                true, outfile, printedOrganizations, outputBuffer);

            for (const string& line : outputBuffer) {
                cout << line << endl;
            }
            cout << "\n������� ����� ������� ��� �����������..." << endl;
            _getch();
            break;
        }
        case '2': {
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

            string baseFilenameStr = string(folderPath) + "�����_" + corrFileNameOnly + "_" + addrFileNameOnly;
            string filename = baseFilenameStr + ".txt";
            int i = 1;
            while (filesystem::exists(filename)) {
                filename = baseFilenameStr + "(" + to_string(i++) + ")" + ".txt";
            }

            try {
                ofstream outfile(filename);
                if (!outfile.is_open()) {
                    cerr << "������: �� ������� ������� ���� ��� ������: " << filename << "\n";
                    cout << "������� ����� ������� ��� �����������..." << endl; _getch();
                    break;
                }

                set<string> printedOrganizations;
                vector<string> outputBuffer; // �� ������������

                ifstream addrFile(addressesFilename);
                if (addrFile.is_open()) {
                    string line;
                    while (getline(addrFile, line)) {
                        stringstream ss(line);
                        string orgName, address, contactPerson;
                        getline(ss, orgName, ';');
                        getline(ss, address, ';');
                        getline(ss, contactPerson, ';');

                        size_t first = orgName.find_first_not_of(" \t");
                        if (string::npos != first)
                        {
                            size_t last = orgName.find_last_not_of(" \t");
                            orgName = orgName.substr(first, (last - first + 1));
                        }
                        else {
                            orgName.clear();
                        }

                        if (!orgName.empty() && printedOrganizations.find(orgName) == printedOrganizations.end()) {
                            processOrganization(orgName, correspondenceFilename, addressesFilename, false, &outfile, printedOrganizations, outputBuffer);
                        }
                    }
                    addrFile.close();
                }
                else {
                    cerr << "������: �� ������� ������� ���� �������: " << addressesFilename << ".\n";
                    outfile.close();
                    cout << "������� ����� ������� ��� �����������..." << endl; _getch();
                    break;
                }

                outfile.close();
                cout << "���������� ������� �������� � ���� " << filename << ".\n";
                cout << "������� ����� ������� ��� �����������..." << endl; _getch();

            }
            catch (const exception& e)
            {
                cerr << "������: �� ������� ������� ��� �������� ����.\n";
                cerr << e.what() << endl;
                cout << "������� ����� ������� ��� �����������..." << endl; _getch();
            }
            break;
        }
        case 27:
            return;
        }
    }
}

// menu (��� ��������� � ������ �������)
void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (folder_way[0] == '\0') {
        normalizePath("", folder_way, sizeof(folder_way));
    }

    printf("\n��������� ��� ��������� ������ � ��������������� � ������� �����������.\n");
    printf("��� ��������� ������ ���������� �� ������������ � �� ���������������, � ����� ����������� ������.\n");

    while (true) {
        system("cls");

        printf("*************************************************************\n");
        printf("* ��������� ��� ��������� ������ ��������������� � �������  *\n");
        printf("* ��������� ��������� ������ � ������������ ���������� ��   *\n");
        printf("* ������������ �� ������ ���� ������ -                      *\n");
        printf("* ��������� ��������������� (%s***%s)                    *\n", ocfe, file_extension);
        printf("* ������� ����������� (%s***%s)                          *\n", oa, file_extension);
        printf("*************************************************************\n\n");

        printf("������� ����:\n");
        printf("1. ������ ������\n");
        printf("2. �������� ���� � �����\n");
        printf("3. ����������\n\n");
        printf("������� ����: %s\n\n", folder_way);
        printf("Esc - �����\n\n");

        char mainChoice = _getch();

        switch (mainChoice) {
        case '1': {
            if (folder_way[0] == '\0') {
                printf("������� �������� ���� � ����� (����� 2).\n");
                printf("������� ����� ������� ��� �����������..."); _getch();
                break;
            }
            pair<string, string> filenames = getFilenamesFromUser(folder_way);
            if (filenames.first.empty() && filenames.second.empty()) {
                break;
            }
            string outputFilenameDummy;
            runProgram(folder_way, filenames.first, filenames.second, outputFilenameDummy);
            break;
        }
        case '2': {
            selectFolderPath();
            break;
        }
        case '3': {
            string instructionsFilePath = string(folder_way) + Constants::INSTRUCTIONS_FILE;
            readInstructionsFromFile(instructionsFilePath);
            break;
        }
        case 27:
            return;

        default:
            break;
        }
    }
}