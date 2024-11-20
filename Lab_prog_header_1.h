//Lab_prog_1.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <consoleapi2.h>
#include <ctype.h>
#include <sys/stat.h>
#include <locale>
#include <string> // Äëÿ std::string
#include <algorithm> // Äëÿ std::replace
#include "Instruction.h"


#define MAX_LENGTH 1000
#define COL_COUNT 3

using namespace std;

char folder_way[256];
char txt_name[256];
char text[COL_COUNT][MAX_LENGTH];
char full_name[512];
const char* file_extension = ".txt";
const char* ocfe = "IC_"; // Èñõîäÿùàÿ êîğğåñïîíäåíöèÿ
const char* oa = "AO_"; // Àäğåñà îğãàíèçàöèé
char menu_out;
char menu_out_case;
char sel;

void menu();
void menu_choises();

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
    int count = sscanf(dateStr, "%2d.%2d.%4d", &day, &month, &year);

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
    // Çàïğåùåííûå ñèìâîëû â Windows
    const std::string invalidChars = "\\/:*?\"<>|";
    return fileName.find_first_of(invalidChars) == std::string::npos;
}

void outgoing_correspondence() {
    do {
        system("cls");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::locale loc("ru_RU.UTF-8");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(11); printf("ÈÑÕÎÄßÙÀß ÊÎĞĞÅÑÏÎÍÄÅÍÖÈß\n\n");
        tabul(11); printf("1)   ÑÎÇÄÀÒÜ ÍÎÂÛÉ ÔÀÉË\n");
        tabul(11); printf("2)   ÎÒÊĞÛÒÜ ÔÀÉË\n");
        tabul(11); printf("Esc) ÂÛÕÎÄ Â ÂÛÁÎĞ ÒÈÏÀ ÔÀÉËÀ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("ÂÂÅÄÈÒÅ ÈÌß ÔÀÉËÀ: ");
            string fileNameInput;
            cin >> fileNameInput;
            if (!isValidFileName(fileNameInput)) {
                tabul(11); printf("Íåäîïóñòèìûå ñèìâîëû â èìåíè ôàéëà. Ïîæàëóéñòà, èñïîëüçóéòå äğóãîå èìÿ.\n");
                printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER."); system("PAUSE>nul");
                break;
            }
            strcpy(txt_name, fileNameInput.c_str());

            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("ÔÀÉË ÓÑÏÅØÍÎ ÑÎÇÄÀÍ. ");
            }
            else {
                tabul(11); printf("ÎØÈÁÊÀ ÑÎÇÄÀÍÈß. ÔÀÉË ÍÅ ÁÛË ÑÎÇÄÀÍ. ");
            }
            printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("ÂÂÅÄÈÒÅ ÈÌß ÔÀÉËÀ: "); scanf("%255s", txt_name);
            tabul(11); printf("ÂÛÁÅĞÈÒÅ ĞÅÆÈÌ ĞÀÁÎÒÛ Ñ ÔÀÉËÎÌ: \n");
            tabul(11); printf("1)   ÇÀÏÈÑÜ Â ÔÀÉË\n");
            tabul(11); printf("2)   ÄÎÁÀÂÈÒÜ Â ÔÀÉË\n");
            tabul(11); printf("Esc) ÂÛÕÎÄ Â ÂÛÁÎĞ ÔÀÉËÀ\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; } // Äîáàâëåí âûõîä ïî Esc
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "ÒÈÏ ÊÎĞĞÅÑÏÎÍÄÅÍÖÈÈ | ÄÀÒÀ | ÍÀÇÂÀÍÈÅ ÎĞÃÀÍÈÇÀÖÈÈ\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // Äîáàâëåí default case äëÿ îáğàáîòêè íåêîğğåêòíîãî ââîäà
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("ÔÀÉË ÎÒÊĞÛÒ.\n");

            while (1) {
                printf(" %s: ", (current_col == 0) ? "ÒÈÏ ÊÎĞĞÅÑÏÎÍÄÅÍÖÈÈ" : (current_col == 1) ? "ÄÀÒÀ (ÄÄ.ÌÌ.ÃÃÃÃ)" : "ÍÀÇÂÀÍÈÅ ÎĞÃÀÍÈÇÀÖÈÈ");
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_outgoing_correspondence;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';

                        if (current_col == 1 && !isValidDate(text[1])) {
                            printf("\nÍåâåğíûé ôîğìàò äàòû. Ïîæàëóéñòà, ââåäèòå äàòó â ôîğìàòå ÄÄ.ÌÌ.ÃÃÃÃ\n");
                            memset(text[current_col], 0, sizeof(text[current_col]));
                            i = 0;
                            continue;
                        }


                        current_col++;
                        if (current_col == COL_COUNT) {
                            for (int j = 0; j < COL_COUNT; ++j) {
                                if (strlen(text[j]) == 0) {
                                    strcpy(text[j], "íåò äàííûõ");
                                }
                            }
                            fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                            current_col = 0;
                            memset(text, 0, sizeof(text));
                            printf("\n");


                            break;
                        }
                        break;

                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
                        for (int j = 0; j < COL_COUNT; ++j) {
                            if (strlen(text[j]) == 0) {
                                strcpy(text[j], "íåò äàííûõ");
                            }
                        }

                        if (current_col == 1 && !isValidDate(text[1]))
                        {
                            printf("\nÍåâåğíûé ôîğìàò äàòû. Ïîæàëóéñòà, ââåäèòå äàòó â ôîğìàòå ÄÄ.ÌÌ.ÃÃÃÃ\n");
                            memset(text[current_col], 0, sizeof(text[current_col]));
                            i = 0;
                            continue;
                        }
                        fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                        current_col = 0;
                        memset(text, 0, sizeof(text));
                        printf("\n");
                        break;
                    }
                    else if (c == 8) {
                        if (i > 0) {
                            i--;
                            printf("\b \b");
                        }
                    }
                    else {
                        if (current_col == 1) { // Òîëüêî äëÿ êîëîíêè ñ äàòîé
                            if (!isdigit(c) && c != '.' && c != '\t' && c != '\r' && c != 8) {
                                continue;
                            }
                            if (i < MAX_LENGTH - 1) {
                                if (c >= -64 && c <= -1) {
                                    continue;
                                }
                                text[current_col][i++] = c;
                                printf("%c", c);
                            }
                        }
                        else { // Äëÿ îñòàëüíûõ êîëîíîê
                            if (i < MAX_LENGTH - 1) {
                                text[current_col][i++] = c;
                                printf("%c", c);
                            }
                        }
                    }
                }
            }
        exit_outgoing_correspondence:
            if (file) fclose(file);
            outgoing_correspondence();
            return;
            break;
        }
        }
    } while (menu_out_case != 27);
}

void organization_addresses() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::locale loc("ru_RU.UTF-8");


    do {
        system("cls");
        for (int i = 4; i != 0; i--) printf("\n");
        tabul(10); printf("ÀÄĞÅÑÀ ÎĞÃÀÍÈÇÀÖÈÈ\n\n");
        tabul(11); printf("1)   ÑÎÇÄÀÒÜ ÍÎÂÛÉ ÔÀÉË\n");
        tabul(11); printf("2)   ÎÒÊĞÛÒÜ ÔÀÉË\n");
        tabul(11); printf("Esc) ÂÛÕÎÄ Â ÂÛÁÎĞ ÒÈÏÀ ÔÀÉËÀ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1': {
            printf("\n");
            tabul(11); printf("ÂÂÅÄÈÒÅ ÈÌß ÔÀÉËÀ: ");
            string fileNameInput;
            cin >> fileNameInput;
            if (!isValidFileName(fileNameInput)) {
                tabul(11); printf("Íåäîïóñòèìûå ñèìâîëû â èìåíè ôàéëà. Ïîæàëóéñòà, èñïîëüçóéòå äğóãîå èìÿ.\n");
                printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER."); system("PAUSE>nul");
                break;  // Âûõîäèì èç case '1'
            }
            strcpy(txt_name, fileNameInput.c_str());


            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tabul(11); printf("ÔÀÉË ÓÑÏÅØÍÎ ÑÎÇÄÀÍ. ");
            }
            else {
                tabul(11); printf("ÎØÈÁÊÀ ÑÎÇÄÀÍÈß. ÔÀÉË ÍÅ ÁÛË ÑÎÇÄÀÍ. ");
            }
            printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER."); system("PAUSE>nul");
            break;
        }

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tabul(11); printf("ÂÂÅÄÈÒÅ ÈÌß ÔÀÉËÀ: "); scanf("%255s", txt_name);
            tabul(11); printf("ÂÛÁÅĞÈÒÅ ĞÅÆÈÌ ĞÀÁÎÒÛ Ñ ÔÀÉËÎÌ: \n");
            tabul(11); printf("1) ÇÀÏÈÑÜ Â ÔÀÉË\n");
            tabul(11); printf("2) ÄÎÁÀÂÈÒÜ Â ÔÀÉË\n");
            tabul(11); printf("Esc)ÂÛÕÎÄ Â ÂÛÁÎĞ ÔÀÉËÀ\n\n");
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            tabul(11); printf("> ");
            sel = _getch();
            if (sel == 27) { break; } // Äîáàâëåí âûõîä ïî Esc
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                fprintf(file, "ÍÀÇÂÀÍÈÅ ÎĞÃÀÍÈÇÀÖÈÈ | ÀÄĞÅÑ | ÔÈÎ ÎĞÃÀÍÈÇÀÒÎĞÀ\n");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            default:
                break; // Äîáàâëåí default case äëÿ îáğàáîòêè íåêîğğåêòíîãî ââîäà
            }
            if (sel != '1' && sel != '2') { break; }
            system("cls");
            printf("ÔÀÉË ÎÒÊĞÛÒ.\n");
            while (1) {
                printf(" %s: ", (current_col == 0) ? "ÍÀÇÂÀÍÈÅ ÎĞÃÀÍÈÇÀÖÈÈ" : (current_col == 1) ? "ÀÄĞÅÑ" : "ÔÈÎ ÎĞÃÀÍÈÇÀÒÎĞÀ");
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_organization_addresses;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';
                        current_col++;
                        if (current_col == COL_COUNT) {
                            for (int j = 0; j < COL_COUNT; ++j) {
                                if (strlen(text[j]) == 0) {
                                    strcpy(text[j], "íåò äàííûõ");
                                }
                            }
                            fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                            current_col = 0;
                            memset(text, 0, sizeof(text));
                            printf("\n");
                            break;
                        }
                        break;

                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
                        for (int j = 0; j < COL_COUNT; ++j) {
                            if (strlen(text[j]) == 0) {
                                strcpy(text[j], "íåò äàííûõ");
                            }
                        }
                        fprintf(file, "%s\t%s\t%s\n", text[0], text[1], text[2]);
                        current_col = 0;
                        memset(text, 0, sizeof(text));
                        printf("\n");
                        break;
                    }
                    else if (c == 8) {
                        if (i > 0) {
                            i--;
                            printf("\b \b");
                        }
                    }
                    else {

                        if (current_col == 2) {
                            bool valid_char = false;
                            if ((c >= 'à' && c <= 'ÿ') || (c >= 'À' && c <= 'ß') ||
                                (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                                c == ' ' || c == '-' || c == '\x2D') {
                                valid_char = true;
                            }

                            if (!valid_char) {
                                continue;
                            }
                        }
                        if (i < MAX_LENGTH - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
            }
        exit_organization_addresses:
            if (file) fclose(file); // Çàêğûâàåì ôàéë òîëüêî åñëè îí áûë îòêğûò.
            organization_addresses();
            return;
            break;
        }
        }
    } while (menu_out_case != 27);
}

void menu_choises() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 6; i != 0; i--) printf("\n");
        tabul(11); printf("ÌÅÍŞ ÂÛÁÎĞÀ ÒÈÏÀ ÄÀÍÍÛÕ\n\n");
        tabul(11); printf("1)   ÈÑÕÎÄßÙÀß ÊÎĞĞÅÑÏÎÍÄÅÍÖÈß\n");
        tabul(11); printf("2)   ÀÄĞÅÑÀ ÎĞÃÀÍÈÇÀÖÈÉ\n");
        tabul(11); printf("Esc) ÂÛÕÎÄ Â ÌÅÍŞ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("ÂÛÁĞÀÍÍÛÉ ÏÓÒÜ Ê ÏÀÏÊÅ: %s\n", folder_way);
        tabul(11); printf("> ");
        menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way != NULL) {
                outgoing_correspondence();

                break;
            }
        case '2':
            if (folder_way != NULL) {
                organization_addresses();

                break;
            }
        }
    } while (menu_out != 27);
}

void program_way() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("\n");
    tabul(11); printf("ÂÂÅÄÈÒÅ ÏÓÒÜ Ê ÏÀÏÊÅ: ");
    scanf("%255s", folder_way);

    size_t len = strlen(folder_way);
    if (len > 0 && folder_way[len - 1] != '\\') {
        folder_way[len] = '\\';
        folder_way[len + 1] = '\0';
    }

    struct stat program_way_buffer;
    if (stat(folder_way, &program_way_buffer) == 0) {
        tabul(11); printf("ÏÓÒÜ Ê ÏÀÏÊÅ ÂÛÁĞÀÍ. ");
    }
    else {
        tabul(11); printf("1Õ1: ÎØÈÁÊÀ ÂÂÎÄÀ. ÍÅÂÅĞÍÛÉ ÏÓÒÜ Ê ÏÀÏÊÅ. ");
        memset(folder_way, 0, sizeof(folder_way));
    }
    printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER."); system("PAUSE>nul");
}

void menu() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    do {
        system("cls");
        for (int i = 3; i != 0; i--) printf("\n");
        tabul(11); printf("ÌÅÍŞ\n\n");
        tabul(11); printf("1)   ÍÀ×ÀÒÜ ĞÀÁÎÒÓ Ñ ÄÀÍÍÛÌÈ\n");
        tabul(11); printf("2)   ÏÓÒÜ Ê ÏÀÏÊÅ ÕĞÀÍÅÍÈß ÔÀÉËÎÂ\n");
        tabul(11); printf("3)   ÈÍÑÒĞÓÊÖÈß\n");
        tabul(11); printf("Esc) ÂÛÕÎÄ ÈÇ ÏĞÎÃĞÀÌÌÛ\n\n");
        tabul(11); printf("_____________________________________________\n\n");
        tabul(11); printf("ÂÛÁĞÀÍÍÛÉ ÏÓÒÜ Ê ÏÀÏÊÅ: %s\n", folder_way);
        tabul(11); printf("> ");

        menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way[0] != '\0')
            {
                menu_choises();
                menu();
                break;
            }
            else
            {
                printf("Ñíà÷àëà âûáåğèòå ïóòü ê ïàïêå. ");
                printf("ÄËß ÏĞÎÄÎËÆÅÍÈß ÍÀÆÌÈÒÅ ENTER.");
                system("PAUSE>nul");
                menu();
                break;
            }

        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        }
    } while (menu_out != 27);
}