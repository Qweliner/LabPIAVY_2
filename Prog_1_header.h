// Prog_1_header.h
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <consoleapi2.h>
#include <ctype.h>
#include <sys/stat.h>
#include "Instruction.h"

#define MAX_LENGTH 1000
#define COL_COUNT 3

using namespace std;

char folder_way[256];
char txt_name[256];
char text[COL_COUNT][MAX_LENGTH];
char full_name[512];
const char* file_extension = ".txt";
const char* ocfe = "IC_"; // »ÒıÓ‰ˇ˘‡ˇ ÍÓÂÒÔÓÌ‰ÂÌˆËˇ
const char* oa = "AO_"; // ¿‰ÂÒ‡ Ó„‡ÌËÁ‡ˆËÈ
char menu_out;
char menu_out_case;
char sel;

void menu();
void program_menu();

void tfm(int x) {
    for (int i = x; i != 0; i--) printf("   ");
}


void outgoing_correspondence() {
    do {
        system("cls");
        for (int i = 4; i != 0; i--) printf("\n");
        tfm(10); printf(" ___  _   _  ___  ___  _  _   ___   _   _\n");
        tfm(10); printf("/ __|| | | || __||_ _|| \\| | |_ _| | | | |\n");
        tfm(10); printf("\\__ \\| |_| || _|  | | | .` |  | |  | |_| |\n");
        tfm(10); printf("|___/ \\___/ |___| |___||_|\\_| |___|  \\___/\n");
        tfm(11); printf(" _____________________________________________\n");
        tfm(11); printf("/                                             \\\n");
        tfm(11); printf("| 1)          —Œ«ƒ¿“‹ ÕŒ¬€…                 |\n");
        tfm(11); printf("| 2)               Œ“ –€“‹                   |\n");
        tfm(11); printf("| Esc)                ¬€’Œƒ                   |\n");
        tfm(11); printf("\\_____________________________________________/\n");
        tfm(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1':
            printf("\n");
            tfm(11); printf("¬¬≈ƒ»“≈ »Ãﬂ ‘¿…À¿: "); scanf("%255s", txt_name); printf("\n");
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tfm(11); printf("‘¿…À ”—œ≈ÿÕŒ —Œ«ƒ¿Õ. ");
            }
            else {
                tfm(11); printf("Œÿ»¡ ¿ —Œ«ƒ¿Õ»ﬂ. ‘¿…À Õ≈ ¡€À —Œ«ƒ¿Õ. ");
            }
            printf("ƒÀﬂ œ–ŒƒŒÀ∆≈Õ»ﬂ Õ¿∆Ã»“≈ ENTER."); system("PAUSE>nul");
            break;

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tfm(11); printf("¬¬≈ƒ»“≈ »Ãﬂ ‘¿…À¿: "); scanf("%255s", txt_name);
            tfm(11); printf("¬€¡≈–»“≈ –≈∆»Ã –¿¡Œ“€ — ‘¿…ÀŒÃ: \n");
            tfm(11); printf("1) «¿œ»—‹ ¬ ‘¿…À\n");
            tfm(11); printf("2) ƒŒ¡¿¬»“‹ ¬ ‘¿…À\n");
            sprintf(full_name, "%s%s%s%s", folder_way, ocfe, txt_name, file_extension);
            tfm(11); printf("> ");
            sel = _getch();
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            }
            system("cls");
            printf("‘¿…À Œ“ –€“.\n");
            fprintf(file, "“»œ  Œ––≈—œŒÕƒ≈Õ÷»» | ƒ¿“¿ | Õ¿«¬¿Õ»≈ Œ–√¿Õ»«¿÷»»\n");
            while (1) {
                printf(" %s: ", (current_col == 0) ? "“»œ  Œ––≈—œŒÕƒ≈Õ÷»»" : (current_col == 1) ? "ƒ¿“¿ (ƒƒ.ÃÃ.√√√√)" : "Õ¿«¬¿Õ»≈ Œ–√¿Õ»«¿÷»»");
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_outgoing_correspondence;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';
                        if (current_col < COL_COUNT - 1) {
                            current_col++;
                            break;
                        }
                        else {
                            printf("\n2X1: Œÿ»¡ ¿ ¬¬Œƒ¿. ƒŒ—“»√Õ”“Œ Ã¿ —»Ã¿À‹ÕŒ≈  ŒÀ»◊≈—“¬Œ  ŒÀŒÕŒ . ƒÀﬂ œ–ŒƒŒÀ∆≈Õ»ﬂ Õ¿∆Ã»“≈ ENTER.");
                            while (_getch() != '\r');
                            break;
                        }
                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
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
                        if (current_col == 1 && !isdigit(c) && c != '.' && c != '\t' && c != '\r' && c != 8) {
                            continue;
                        }
                        if (i < MAX_LENGTH - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
            }
        exit_outgoing_correspondence:
            fclose(file);
            outgoing_correspondence();
            return;
            break;
        } // case 2
        } // switch
    } while (menu_out_case != 27);
}

void organization_addresses() {
    do {
        system("cls");
        for (int i = 4; i != 0; i--) printf("\n");
        tfm(10); printf("  ___ _  _ ___ ___ ___  _   _\n");
        tfm(10); printf(" | _ \\ \\| |/ __/ __/ _ \\| | | |\n");
        tfm(10); printf(" |  _/ | | | (_| (_| (_) | |_| |\n");
        tfm(10); printf(" |_|  |_|_|\\___\\___\\___/  \\___/\n");
        tfm(11); printf(" _____________________________________________\n");
        tfm(11); printf("/                                             \\\n");
        tfm(11); printf("| 1)          —Œ«ƒ¿“‹ ÕŒ¬€…                 |\n");
        tfm(11); printf("| 2)               Œ“ –€“‹                   |\n");
        tfm(11); printf("| Esc)                ¬€’Œƒ                   |\n");
        tfm(11); printf("\\_____________________________________________/\n");
        tfm(11); printf("> ");
        menu_out_case = _getch();
        FILE* file{};
        switch (menu_out_case) {
        case '1':
            printf("\n");
            tfm(11); printf("¬¬≈ƒ»“≈ »Ãﬂ ‘¿…À¿: "); scanf("%255s", txt_name); printf("\n");
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            file = fopen(full_name, "w");
            if (file) {
                fclose(file);
                tfm(11); printf("‘¿…À ”—œ≈ÿÕŒ —Œ«ƒ¿Õ. ");
            }
            else {
                tfm(11); printf("Œÿ»¡ ¿ —Œ«ƒ¿Õ»ﬂ. ‘¿…À Õ≈ ¡€À —Œ«ƒ¿Õ. ");
            }
            printf("ƒÀﬂ œ–ŒƒŒÀ∆≈Õ»ﬂ Õ¿∆Ã»“≈ ENTER."); system("PAUSE>nul");
            break;

        case '2': {
            int i = 0;
            int current_col = 0;
            printf("\n");
            tfm(11); printf("¬¬≈ƒ»“≈ »Ãﬂ ‘¿…À¿: "); scanf("%255s", txt_name);
            tfm(11); printf("¬€¡≈–»“≈ –≈∆»Ã –¿¡Œ“€ — ‘¿…ÀŒÃ: \n");
            tfm(11); printf("1) «¿œ»—‹ ¬ ‘¿…À\n");
            tfm(11); printf("2) ƒŒ¡¿¬»“‹ ¬ ‘¿…À\n");
            sprintf(full_name, "%s%s%s%s", folder_way, oa, txt_name, file_extension);
            tfm(11); printf("> ");
            sel = _getch();
            switch (sel) {
            case '1':
                file = fopen(full_name, "w");
                break;
            case '2':
                file = fopen(full_name, "a");
                break;
            }
            system("cls");
            printf("‘¿…À Œ“ –€“.\n");
            fprintf(file, "Õ¿«¬¿Õ»≈ Œ–√¿Õ»«¿÷»» | ¿ƒ–≈— | ‘»Œ Œ–√¿Õ»«¿“Œ–¿\n");
            while (1) {
                printf(" %s: ", (current_col == 0) ? "Õ¿«¬¿Õ»≈ Œ–√¿Õ»«¿÷»»" : (current_col == 1) ? "¿ƒ–≈—" : "‘»Œ Œ–√¿Õ»«¿“Œ–¿");
                i = 0;
                while (1) {
                    char c = _getch();

                    if (c == 27) {
                        goto exit_organization_addresses;
                    }
                    else if (c == '\t') {
                        text[current_col][i] = '\0';
                        if (current_col < COL_COUNT - 1) {
                            current_col++;
                            break;
                        }
                        else {
                            printf("\n2X1: Œÿ»¡ ¿ ¬¬Œƒ¿. ƒŒ—“»√Õ”“Œ Ã¿ —»Ã¿À‹ÕŒ≈  ŒÀ»◊≈—“¬Œ  ŒÀŒÕŒ . ƒÀﬂ œ–ŒƒŒÀ∆≈Õ»ﬂ Õ¿∆Ã»“≈ ENTER.");
                            while (_getch() != '\r');
                            break;
                        }
                    }
                    else if (c == '\r') {
                        text[current_col][i] = '\0';
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
                        if (current_col == 2 && isdigit(c)) {
                            continue;
                        }
                        if (i < MAX_LENGTH - 1) {
                            text[current_col][i++] = c;
                            printf("%c", c);
                        }
                    }
                }
            }
        exit_organization_addresses:
            fclose(file);
            organization_addresses();
            return;
            break;
        } // case 2
        } // switch
    } while (menu_out_case != 27);
}

void program_menu() {
    do {
        system("cls");
        for (int i = 6; i != 0; i--) printf("\n");
        tfm(11); printf(" _____________________________________________\n");
        tfm(11); printf("/           __  __  ___  _  _  _   _          \\\n");
        tfm(11); printf("|          |  \\/  || __|| \\| || | | |         |\n");
        tfm(11); printf("|          | |\\/| || _| | .` || |_| |         |\n");
        tfm(11); printf("|          |_|  |_||___||_|\\_| \\___/          |\n");
        tfm(11); printf("|---------------------------------------------|\n");
        tfm(11); printf("| 1) »—’ŒƒﬂŸ¿ﬂ  Œ––≈—œŒÕƒ≈Õ÷»ﬂ           |\n");
        tfm(11); printf("| 2)     ¿ƒ–≈—¿ Œ–√¿Õ»«¿÷»…              |\n");
        tfm(11); printf("| Esc)                ¬€’Œƒ                   |\n");
        tfm(11); printf("\\_____________________________________________/\n");
        tfm(11); printf("¬€¡–¿ÕÕ€… œ”“‹   œ¿œ ≈: %s\n", folder_way);
        tfm(11); printf("> ");
        menu_out = _getch();
        switch (menu_out) {
        case '1':
            if (folder_way != NULL) {
                outgoing_correspondence();
                program_menu();
                break;
            }
        case '2':
            if (folder_way != NULL) {
                organization_addresses();
                program_menu();
                break;
            }
        }
    } while (menu_out != 27);
}

void program_way() {
    printf("\n");
    tfm(11); printf("¬¬≈ƒ»“≈ œ”“‹   œ¿œ ≈: "); scanf("%255s", folder_way);
    struct stat buffer;
    if (stat(folder_way, &buffer) == 0) {
        tfm(11); printf("œ”“‹   œ¿œ ≈ ¬€¡–¿Õ. ");
    }
    else {
        tfm(11); printf("1’1: Œÿ»¡ ¿ ¬¬Œƒ¿. Õ≈¬≈–Õ€… œ”“‹   œ¿œ ≈. ");
        memset(folder_way, 0, sizeof(folder_way));
    }
    printf("ƒÀﬂ œ–ŒƒŒÀ∆≈Õ»ﬂ Õ¿∆Ã»“≈ ENTER."); system("PAUSE>nul");
}

void menu() {
    do {
        system("cls");
        for (int i = 3; i != 0; i--) printf("\n");
        // ... („‡ÙË˜ÂÒÍËÈ ‚˚‚Ó‰ ÏÂÌ˛ - ÚÓÚ ÊÂ, ˜ÚÓ Ë ‡Ì¸¯Â) ...

        tfm(11); printf(" _____________________________________________\n");
        tfm(11); printf("/        __  __  ______  _   _  _    _        \\\n");
        tfm(11); printf("|       |  \\/  ||  ____|| \\ | || |  | |       |\n");
        tfm(11); printf("|       | \\  / || |__   |  \\| || |  | |       |\n");
        tfm(11); printf("|       | |\\/| ||  __|  | . ` || |  | |       |\n");
        tfm(11); printf("|       | |  | || |____ | |\\  || |__| |       |\n");
        tfm(11); printf("|       |_|  |_||______||_| \\_| \\____/        |\n");
        tfm(11); printf("|                                             |\n");
        tfm(11); printf("|---------------------------------------------|\n");
        tfm(11); printf("| 1)                 Õ¿◊¿“‹                   |\n");
        tfm(11); printf("| 2)               œ”“‹   œ¿œ ≈              |\n");
        tfm(11); printf("| 3)               »Õ—“–” ÷»ﬂ                 |\n");
        tfm(11); printf("| Esc)                ¬€’Œƒ                   |\n");
        tfm(11); printf("\\_____________________________________________/\n");
        tfm(11); printf("¬€¡–¿ÕÕ€… œ”“‹   œ¿œ ≈: %s\n", folder_way);
        tfm(11); printf("> ");
        menu_out = _getch();
        switch (menu_out) {
        case '1':
            program_menu();
            menu();
            break;
        case '2':
            program_way();
            break;
        case '3':
            instruction();
            break;
        }
    } while (menu_out != 27);
}