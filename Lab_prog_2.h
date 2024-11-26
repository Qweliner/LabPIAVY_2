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

// Структуры
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
    std::cout << "\nИнструкция по работе с программой:\n";
    std::cout << "1. Путь к папке: Укажите полный путь к папке, содержащей файлы с корреспонденцией и адресами.\n";
    std::cout << "   Пример: C:\\Users\\YourName\\Documents\\Correspondence\n";
    std::cout << "2. Название файлов: Введите названия файлов БЕЗ расширения .txt.\n";
    std::cout << "   Программа автоматически добавит префиксы (IC_ и AO_) и расширение.\n";
    std::cout << "3. Избирательный вывод: При выборе этого режима введите название организации для поиска.\n";
    std::cout << "4. Полный вывод: При выборе этого режима данные будут записаны в файл output.txt в указанной папке.\n";
    std::cout << "5. Ошибки:\n";
    std::cout << "   - Неверный путь к папке: Программа сообщит об ошибке.\n";
    std::cout << "   - Неверный формат файлов: Программа пропустит строки с некорректным форматом данных и выведет предупреждение.\n";
    std::cout << "   - Организация не найдена: Программа сообщит, если не найдет информацию о запрошенной организации.\n";
    std::cout << "6. Навигация по меню: Используйте клавиши 1, 2, 3, I, S, O или Esc для навигации.\n";
    std::cout << "   Esc - выход из программы или возврат в предыдущее меню.\n";
    std::cout << "\nНажмите любую клавишу для продолжения...";
    _getch();
}

std::vector<Correspondence> readCorrespondence(const std::string& filename) {
    std::vector<Correspondence> correspondence;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Пропустить заголовок

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
                std::cerr << "Ошибка формата файла корреспонденции в строке: " << line << std::endl;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл корреспонденции: " << filename << std::endl;
    }
    return correspondence;
}

std::vector<Address> readAddresses(const std::string& filename) {
    std::vector<Address> addresses;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Пропустить заголовок

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
                std::cerr << "Ошибка формата файла адресов в строке: " << line << std::endl;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл адресов: " << filename << std::endl;
    }
    return addresses;
}

void printSelectiveInfo(const std::vector<Correspondence>& correspondence, const std::vector<Address>& addresses) {
    std::string orgName;
    std::cout << "Введите название организации для поиска: ";
    std::cin >> orgName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (const auto& corr : correspondence) {
        if (corr.organization == orgName) {
            for (const auto& addr : addresses) {
                if (addr.organization == orgName) {
                    std::cout << "Тип корреспонденции: " << corr.type << std::endl;
                    std::cout << "Дата: " << corr.date << std::endl;
                    std::cout << "Адрес: " << addr.address << std::endl;
                    std::cout << "Контактное лицо: " << addr.contactPerson << std::endl;
                    std::cout << "--------------------" << std::endl;
                    return;
                }
            }
            std::cerr << "Адрес для организации " << orgName << " не найден." << std::endl;
            return;
        }
    }
    std::cerr << "Корреспонденция для организации " << orgName << " не найдена." << std::endl;
}

void printAllInfoToFile(const std::vector<Correspondence>& correspondence, const std::vector<Address>& addresses, const std::string& filename) {
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }

    std::map<std::string, std::vector<Correspondence>> correspondenceByOrganization;
    for (const auto& corr : correspondence) {
        correspondenceByOrganization[corr.organization].push_back(corr);
    }

    for (const auto& addr : addresses) {
        outfile << "Организация: " << addr.organization << std::endl;
        outfile << "Адрес: " << addr.address << std::endl;
        outfile << "Контактное лицо: " << addr.contactPerson << std::endl;

        if (correspondenceByOrganization.count(addr.organization)) {
            for (const auto& corr : correspondenceByOrganization[addr.organization]) {
                outfile << "\tТип корреспонденции: " << corr.type << std::endl;
                outfile << "\tДата: " << corr.date << std::endl;
            }
        }
        else {
            outfile << "\tКорреспонденция не найдена." << std::endl;
        }
        outfile << "--------------------" << std::endl;
    }

    outfile.close();
    std::cout << "Информация успешно записана в файл: " << filename << std::endl;
}




std::pair<std::string, std::string> getFilenamesFromUser(const std::string& folderPath) {
    std::string correspondenceFilename, addressesFilename;

    std::cout << "Введите имя файла с исходящей корреспонденцией (без расширения): ";
    std::cin >> correspondenceFilename;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    std::cout << "Введите имя файла с адресами организаций (без расширения): ";
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
        std::cerr << "Ошибка чтения данных. Проверьте файлы и повторите попытку.\n";
        std::cout << "Нажмите любую клавишу для возврата в меню...";
        _getch();

        return;
    }


    char choice;
    std::cout << "\nВыберите режим вывода:\n";
    std::cout << "1. Избирательный вывод на экран (I)\n";
    std::cout << "2. Полный вывод в файл (O)\n";
    std::cout << "Esc - Вернуться в главное меню\n";
    std::cout << "Ваш выбор: ";

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
        std::cerr << "Неверный выбор.\n";
    }
    std::cout << "Нажмите любую клавишу для возврата в меню...";
    _getch();


}