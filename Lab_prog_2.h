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

// ���������
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


void printInstructions() {
    system("cls");
    std::cout << "\n���������� �� ������ � ����������:\n";
    std::cout << "1. ���� � �����: ������� ������ ���� � �����, ���������� ����� � ���������������� � ��������.\n";
    std::cout << "   ������: C:\\Users\\YourName\\Documents\\Correspondence\n";
    std::cout << "2. �������� ������: ������� �������� ������ ��� ���������� .txt.\n";
    std::cout << "   ��������� ������������� ������� �������� (IC_ � AO_) � ����������.\n";
    std::cout << "3. ������������� �����: ��� ������ ����� ������ ������� �������� ����������� ��� ������.\n";
    std::cout << "4. ������ �����: ��� ������ ����� ������ ������ ����� �������� � ���� output.txt � ��������� �����.\n";
    std::cout << "5. ������:\n";
    std::cout << "   - �������� ���� � �����: ��������� ������� �� ������.\n";
    std::cout << "   - �������� ������ ������: ��������� ��������� ������ � ������������ �������� ������ � ������� ��������������.\n";
    std::cout << "   - ����������� �� �������: ��������� �������, ���� �� ������ ���������� � ����������� �����������.\n";
    std::cout << "6. ��������� �� ����: ����������� ������� 1, 2, 3, I, S, O ��� Esc ��� ���������.\n";
    std::cout << "   Esc - ����� �� ��������� ��� ������� � ���������� ����.\n";
    std::cout << "\n������� ����� ������� ��� �����������...";
    _getch();
}

std::vector<Correspondence> readCorrespondence(const std::string& filename) {
    std::vector<Correspondence> correspondence;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // ���������� ���������

        while (std::getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                Correspondence entry;
                entry.type = line.substr(0, pos1);
                entry.date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.organization = line.substr(pos2 + 1);
                correspondence.push_back(entry);
            }
            else {
                std::cerr << "������ ������� ����� ��������������� � ������: " << line << std::endl;
            }
        }
        file.close();
    }
    else {
        std::cerr << "�� ������� ������� ���� ���������������: " << filename << std::endl;
    }
    return correspondence;
}

std::vector<Address> readAddresses(const std::string& filename) {
    std::vector<Address> addresses;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // ���������� ���������

        while (std::getline(file, line)) {
            size_t pos1 = line.find('\t');
            size_t pos2 = line.find('\t', pos1 + 1);

            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                Address entry;
                entry.organization = line.substr(0, pos1);
                entry.address = line.substr(pos1 + 1, pos2 - pos1 - 1);
                entry.contactPerson = line.substr(pos2 + 1);
                addresses.push_back(entry);
            }
            else {
                std::cerr << "������ ������� ����� ������� � ������: " << line << std::endl;
            }
        }
        file.close();
    }
    else {
        std::cerr << "�� ������� ������� ���� �������: " << filename << std::endl;
    }
    return addresses;
}

void printSelectiveInfo(const std::vector<Correspondence>& correspondence, const std::vector<Address>& addresses) {
    std::string orgName;
    std::cout << "������� �������� ����������� ��� ������: ";
    std::cin >> orgName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (const auto& corr : correspondence) {
        if (corr.organization == orgName) {
            for (const auto& addr : addresses) {
                if (addr.organization == orgName) {
                    std::cout << "��� ���������������: " << corr.type << std::endl;
                    std::cout << "����: " << corr.date << std::endl;
                    std::cout << "�����: " << addr.address << std::endl;
                    std::cout << "���������� ����: " << addr.contactPerson << std::endl;
                    std::cout << "--------------------" << std::endl;
                    return;
                }
            }
            std::cerr << "����� ��� ����������� " << orgName << " �� ������." << std::endl;
            return;
        }
    }
    std::cerr << "��������������� ��� ����������� " << orgName << " �� �������." << std::endl;
}

void printAllInfoToFile(const std::vector<Correspondence>& correspondence, const std::vector<Address>& addresses, const std::string& filename) {
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "�� ������� ������� ���� ��� ������: " << filename << std::endl;
        return;
    }

    std::map<std::string, std::vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        outfile << "�����������: " << addr.organization << std::endl;
        outfile << "�����: " << addr.address << std::endl;
        outfile << "���������� ����: " << addr.contactPerson << std::endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                outfile << "\t��� ���������������: " << corr.type << std::endl;
                outfile << "\t����: " << corr.date << std::endl;
            }
        }
        else {
            outfile << "\t��������������� �� �������." << std::endl;
        }
        outfile << "--------------------" << std::endl;
    }

    outfile.close();
    std::cout << "���������� ������� �������� � ����: " << filename << std::endl;
}




std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath) {
    std::string correspondenceFilename, addressesFilename;

    std::cout << "������� ��� ����� � ��������� ���������������� (��� ����������): ";
    std::cin >> correspondenceFilename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    std::cout << "������� ��� ����� � �������� ����������� (��� ����������): ";
    std::cin >> addressesFilename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    return { folderPath + "IC_" + correspondenceFilename + ".txt", folderPath + "AO_" + addressesFilename + ".txt" };

}



void runProgram(std::string& folderPath, std::string& correspondenceFilename, std::string& addressesFilename, std::string& outputFilename) {


    tie(correspondenceFilename, addressesFilename) = getFilenamesFromUser(folderPath);
    if (folderPath.back() != '\\' && folderPath.back() != '/') {
        folderPath += '\\';
    }

    outputFilename = folderPath + "output.txt";
    std::vector<Correspondence> correspondence = readCorrespondence(correspondenceFilename);
    std::vector<Address> addresses = readAddresses(addressesFilename);

    if (correspondence.empty() || addresses.empty()) {
        std::cerr << "������ ������ ������. ��������� ����� � ��������� �������.\n";
        std::cout << "������� ����� ������� ��� �������� � ����...";
        _getch();

        return;
    }


    char choice;
    std::cout << "\n�������� ����� ������:\n";
    std::cout << "1. ������������� ����� �� ����� (I)\n";
    std::cout << "2. ������ ����� � ���� (O)\n";
    std::cout << "Esc - ��������� � ������� ����\n";
    std::cout << "��� �����: ";

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
        std::cerr << "�������� �����.\n";
    }
    std::cout << "������� ����� ������� ��� �������� � ����...";
    _getch();


}