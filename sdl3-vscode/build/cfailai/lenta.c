/*#include "lenta.h"
#include "colors.h"
#include "veiksmai.h"
#include "korteles.h"
#include <string.h>
 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
 
int is_buildable_cell(int pos)
{
    int buildable[] = {1,2,3,5,7,8,12,14,16,18,19,21,23,25,27,30,31,32,34,35,38,41,43};
    int count = (int)(sizeof(buildable) / sizeof(buildable[0]));
    for (int i = 0; i < count; i++)
        if (buildable[i] == pos) return 1;
    return 0;
}
 
int get_house_cost(int pos)
{
    Biblioteka *bib = gauti_biblioteka_pagal_pozicija(pos);
    return bib ? bib->namo_kaina : 20480;
}
 
void buildCells(cell cells[])
{
    const char *ttext[44] = {
        "", "ASSEMBLY","ASSEMBLY","ASSEMBLY","", "HTML","", "HTML","HTML","", "",
        "", "PYTHON","", "PYTHON","", "PYTHON","", "FORTRAN","FORTRAN","", "FORTRAN",
        "", "JAVA","", "JAVA","", "JAVA","", "", "ADA","ADA","ADA",
        "", "C++","C++","", "", "C++","", "", "C","", "C"
    };
 
    const char *btext[44] = {
        "START PROGRAM\n+102,400 baitai", "LINUX_SYSCALLS.INC\n61,440baitai", "IO.INC\n61,440baitai", "MACRO.INC\n102,400baitai",
        "CHAT GPT", "FONTAWESOME.MIN.CSS\n122,880baitai", "VS CODE\n409,600baitai", "TAILWIND.MIN.CSS\n143,360baitai",
        "BOOTSTRAP.MIN.CSS\n163,840baitai", "RAND()", "DAR KARTA MESK",
        "ERROR!", "SYS\n184,320baitai", "WINDOWS 11\n153,600baitai", "OS\n184,320baitai", "CODEBLOCKS\n409,600baitai", "MATH\n204,800baitai",
        "SYSTEM LOGS", "IEEE_ARITHMETIC\n225,280baitai", "ISO_C_BINDING\n245,760baitai", "SSD", "ISO_FORTRAN_ENV\n266,240baitai",
        "FREE TRIAL", "JAVA.UTIL\n286,720baitai", "RAND()", "JAVA.NET\n307,200baitai", "GDB ONLINE\n409,600baitai", "JAVA.IO\n307,200baitai",
        "SYSTEM LOGS", "MAC OS\n153,600baitai", "INTERFACES.C\n327,680baitai", "ADA.INTEGER_TEXT_IO\n358,400baitai", "ADA.TEXT_IO\n358,400baitai",
        "Eik i ERROR!", "STRING\n389,120baitai", "IOSTREAM\n409,600baitai", "LINUX\n153,600baitai", "SYSTEM LOGS",
        "BITS/STDC++.H\n430,080baitai", "RIPES\n409,600baitai", "RAND()", "STDLIB.H\n466,944baitai", "DEEP SEEK", "STDIO.H\n1,048,576baitai"
    };
 
    SDL_Color propertyColors[44] = {
        white, assembly, assembly, assembly, white, html, white, html, html, white, white,
        white, python, white, python, white, python, python, fortran, fortran, white, fortran,
        white, java, white, java, white, java, white, white, ada, ada, ada,
        white, cpp, cpp, white, white, cpp, white, white, c, white, c
    };
 
    for (int side = 0; side < 4; side++)
    {
        for (int i = 0; i < CELL_COUNT + 2; i++)
        {
            SDL_FRect r;
            switch (side)
            {
                case 0:
                    r.y = 0; r.h = CORNER_SIZE;
                    if (i == 0) { r.x = 0; r.w = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE; }
                    else { r.x = CORNER_SIZE + (i - 1) * CELL_SIZE; r.w = CELL_SIZE; }
                    break;
                case 1:
                    r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE;
                    if (i == 0) { r.y = 0; r.h = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE; }
                    else { r.y = CORNER_SIZE + (i - 1) * CELL_SIZE; r.h = CELL_SIZE; }
                    break;
                case 2:
                    r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE;
                    if (i == 0) { r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.x = 0; r.w = CORNER_SIZE; }
                    else { r.x = BOARD_SIZE - CORNER_SIZE - i * CELL_SIZE; r.w = CELL_SIZE; }
                    break;
                default:
                    r.x = 0; r.w = CORNER_SIZE;
                    if (i == 0) { r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.y = 0; r.h = CORNER_SIZE; }
                    else { r.y = BOARD_SIZE - CORNER_SIZE - i * CELL_SIZE; r.h = CELL_SIZE; }
                    break;
            }
 
            int index = side * (CELL_COUNT + 2) + i;
            if (index < 0 || index >= 44) continue;
 
            cells[index].rect = r;
            cells[index].owner = -1;
            cells[index].tcolor = propertyColors[index];
            cells[index].bcolor = white;
 
            // Nauji laukai (kad sutaptų su zaidimo_ciklas.c)
            cells[index].type = is_buildable_cell(index) ? 0 : 1;
            cells[index].houses = 0;
 
            Biblioteka *bib = gauti_biblioteka_pagal_pozicija(index);
            cells[index].price = bib ? bib->kaina : 0;
            cells[index].house_price = bib ? bib->namo_kaina : 0;
 
            strncpy(cells[index].ttext, ttext[index], sizeof(cells[index].ttext) - 1);
            cells[index].ttext[sizeof(cells[index].ttext) - 1] = '\0';
            strncpy(cells[index].btext, btext[index], sizeof(cells[index].btext) - 1);
            cells[index].btext[sizeof(cells[index].btext) - 1] = '\0';
        }
    }
}
 
board_layout_t compute_board_layout(void)
{
    board_layout_t L;
    int margin_x = (SCREEN_WIDTH - (BOARD_SIZE + SIDEBAR_WIDTH)) / 2;
    int margin_y = (SCREEN_HEIGHT - BOARD_SIZE) / 2;
    L.centerRect.x = margin_x + (BOARD_SIZE - BOARD_SIZE / 2) / 2;
    L.centerRect.y = margin_y + (BOARD_SIZE - BOARD_SIZE / 2) / 2;
    L.centerRect.w = BOARD_SIZE / 2;
    L.centerRect.h = BOARD_SIZE / 2;
    L.sidebarRect.x = margin_x + BOARD_SIZE + 10;
    L.sidebarRect.y = margin_y;
    L.sidebarRect.w = SIDEBAR_WIDTH - 20;
    L.sidebarRect.h = BOARD_SIZE;
    return L;
}
 
int detect_clicked_property(int mx, int my, cell cells[], int n)
{
    SDL_FPoint pt = {(float)mx, (float)my};
    for (int i = 0; i < n; i++) {
        SDL_FRect rect = cells[i].rect;
        if (pt.x >= rect.x && pt.x <= rect.x + rect.w &&
            pt.y >= rect.y && pt.y <= rect.y + rect.h)
            return i;
    }
    return -1;
}
 
int handle_center_card_click(int mx, int my, SDL_FRect centerRect)
{
    SDL_FPoint pt = {(float)mx, (float)my};
    float rectW = centerRect.w / 3;
    float rectH = centerRect.h / 6;
 
    float cx1 = centerRect.x + centerRect.w / 3;
    float cy1 = centerRect.y + centerRect.h / 2;
    SDL_FRect randRect = {cx1 - rectW/2, cy1 - rectH/2, rectW, rectH};
 
    float cx2 = centerRect.x + 2 * centerRect.w / 3;
    float cy2 = centerRect.y + centerRect.h / 2;
    SDL_FRect logsRect = {cx2 - rectW/2, cy2 - rectH/2, rectW, rectH};
 
    if (pt.x >= randRect.x && pt.x <= randRect.x + randRect.w &&
        pt.y >= randRect.y && pt.y <= randRect.y + randRect.h) return RAND_KORTA;
 
    if (pt.x >= logsRect.x && pt.x <= logsRect.x + logsRect.w &&
        pt.y >= logsRect.y && pt.y <= logsRect.y + logsRect.h) return SYSTEM_LOGS_KORTA;
 
    return 0;
}*/


#include "lenta.h"
#include "colors.h"
#include "veiksmai.h"
#include "korteles.h"
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int is_buildable_cell(int pos)
{
    int buildable[] = {1,2,3,5,7,8,12,14,16,18,19,21,23,25,27,30,31,32,34,35,38,41,43};
    int count = (int)(sizeof(buildable) / sizeof(buildable[0]));
    for (int i = 0; i < count; i++)
        if (buildable[i] == pos) return 1;
    return 0;
}

int get_house_cost(int pos)
{
    Biblioteka *bib = gauti_biblioteka_pagal_pozicija(pos);
    return bib ? bib->namo_kaina : 20480;
}

void buildCells(cell cells[])
{
    const char *ttext[44] = {
        "", "ASSEMBLY","ASSEMBLY","ASSEMBLY","", "HTML","", "HTML","HTML","", "",
        "", "PYTHON","", "PYTHON","", "PYTHON","", "FORTRAN","FORTRAN","", "FORTRAN",
        "", "JAVA","", "JAVA","", "JAVA","", "", "ADA","ADA","ADA",
        "", "C++","C++","", "", "C++","", "", "C","", "C"
    };

    const char *btext[44] = {
        "START PROGRAM\n+102,400 baitai", "LINUX_SYSCALLS.INC\n61,440baitai", "IO.INC\n61,440baitai", "MACRO.INC\n102,400baitai",
        "CHAT GPT", "FONTAWESOME.MIN.CSS\n122,880baitai", "VS CODE\n409,600baitai", "TAILWIND.MIN.CSS\n143,360baitai",
        "BOOTSTRAP.MIN.CSS\n163,840baitai", "RAND()", "DAR KARTA MESK",
        "ERROR!", "SYS\n184,320baitai", "WINDOWS 11\n153,600baitai", "OS\n184,320baitai", "CODEBLOCKS\n409,600baitai", "MATH\n204,800baitai",
        "SYSTEM LOGS", "IEEE_ARITHMETIC\n225,280baitai", "ISO_C_BINDING\n245,760baitai", "SSD", "ISO_FORTRAN_ENV\n266,240baitai",
        "FREE TRIAL", "JAVA.UTIL\n286,720baitai", "RAND()", "JAVA.NET\n307,200baitai", "GDB ONLINE\n409,600baitai", "JAVA.IO\n307,200baitai",
        "SYSTEM LOGS", "MAC OS\n153,600baitai", "INTERFACES.C\n327,680baitai", "ADA.INTEGER_TEXT_IO\n358,400baitai", "ADA.TEXT_IO\n358,400baitai",
        "Eik i ERROR!", "STRING\n389,120baitai", "IOSTREAM\n409,600baitai", "LINUX\n153,600baitai", "SYSTEM LOGS",
        "BITS/STDC++.H\n430,080baitai", "RIPES\n409,600baitai", "RAND()", "STDLIB.H\n466,944baitai", "DEEP SEEK", "STDIO.H\n1,048,576baitai"
    };

    SDL_Color propertyColors[44] = {
        white, assembly, assembly, assembly, white, html, white, html, html, white, white,
        white, python, white, python, white, python, python, fortran, fortran, white, fortran,
        white, java, white, java, white, java, white, white, ada, ada, ada,
        white, cpp, cpp, white, white, cpp, white, white, c, white, c
    };

    for (int side = 0; side < 4; side++)
    {
        for (int i = 0; i < CELL_COUNT + 2; i++)
        {
            SDL_FRect r;
            switch (side)
            {
                case 0:
                    r.y = 0; r.h = CORNER_SIZE;
                    if (i == 0) { r.x = 0; r.w = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE; }
                    else { r.x = CORNER_SIZE + (i - 1) * CELL_SIZE; r.w = CELL_SIZE; }
                    break;
                case 1:
                    r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE;
                    if (i == 0) { r.y = 0; r.h = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE; }
                    else { r.y = CORNER_SIZE + (i - 1) * CELL_SIZE; r.h = CELL_SIZE; }
                    break;
                case 2:
                    r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE;
                    if (i == 0) { r.x = BOARD_SIZE - CORNER_SIZE; r.w = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.x = 0; r.w = CORNER_SIZE; }
                    else { r.x = BOARD_SIZE - CORNER_SIZE - i * CELL_SIZE; r.w = CELL_SIZE; }
                    break;
                default:
                    r.x = 0; r.w = CORNER_SIZE;
                    if (i == 0) { r.y = BOARD_SIZE - CORNER_SIZE; r.h = CORNER_SIZE; }
                    else if (i == CELL_COUNT + 1) { r.y = 0; r.h = CORNER_SIZE; }
                    else { r.y = BOARD_SIZE - CORNER_SIZE - i * CELL_SIZE; r.h = CELL_SIZE; }
                    break;
            }

            int index = side * (CELL_COUNT + 2) + i;
            if (index < 0 || index >= 44) continue;
            int margin_x = (SCREEN_WIDTH - (BOARD_SIZE + SIDEBAR_WIDTH)) / 2;int margin_y = (SCREEN_HEIGHT - BOARD_SIZE) / 2;
            r.x += (float)margin_x;
            r.y += (float)margin_y;

            cells[index].rect = r;
            cells[index].owner = -1;
            cells[index].tcolor = propertyColors[index];
            cells[index].bcolor = white;

            cells[index].houses = 0;
            cells[index].type = is_buildable_cell(index) ? 0 : 1;

            Biblioteka *bib = gauti_biblioteka_pagal_pozicija(index);
            int price = bib ? bib->kaina : 0;
            int hprice = bib ? bib->namo_kaina : 0;

            cells[index].kaina = price;
            cells[index].price = price;
            cells[index].house_price = hprice;

            strncpy(cells[index].ttext, ttext[index], sizeof(cells[index].ttext) - 1);
            cells[index].ttext[sizeof(cells[index].ttext) - 1] = '\0';
            strncpy(cells[index].btext, btext[index], sizeof(cells[index].btext) - 1);
            cells[index].btext[sizeof(cells[index].btext) - 1] = '\0';
        }
    }
}

board_layout_t compute_board_layout(void)
{
    board_layout_t L;
    int margin_x = (SCREEN_WIDTH - (BOARD_SIZE + SIDEBAR_WIDTH)) / 2;
    int margin_y = (SCREEN_HEIGHT - BOARD_SIZE) / 2;
    L.centerRect.x = margin_x + (BOARD_SIZE - BOARD_SIZE / 2) / 2;
    L.centerRect.y = margin_y + (BOARD_SIZE - BOARD_SIZE / 2) / 2;
    L.centerRect.w = BOARD_SIZE / 2;
    L.centerRect.h = BOARD_SIZE / 2;
    L.sidebarRect.x = margin_x + BOARD_SIZE + 10;
    L.sidebarRect.y = margin_y;
    L.sidebarRect.w = SIDEBAR_WIDTH - 20;
    L.sidebarRect.h = BOARD_SIZE;
    return L;
}

int detect_clicked_property(int mx, int my, cell cells[], int n)
{
    SDL_FPoint pt = {(float)mx, (float)my};
    for (int i = 0; i < n; i++) {
        SDL_FRect rect = cells[i].rect;
        if (pt.x >= rect.x && pt.x <= rect.x + rect.w &&
            pt.y >= rect.y && pt.y <= rect.y + rect.h)
            return i;
    }
    return -1;
}

int handle_center_card_click(int mx, int my, SDL_FRect centerRect)
{
    SDL_FPoint pt = {(float)mx, (float)my};
    float rectW = centerRect.w / 3;
    float rectH = centerRect.h / 6;

    float cx1 = centerRect.x + centerRect.w / 3;
    float cy1 = centerRect.y + centerRect.h / 2;
    SDL_FRect randRect = {cx1 - rectW/2, cy1 - rectH/2, rectW, rectH};

    float cx2 = centerRect.x + 2 * centerRect.w / 3;
    float cy2 = centerRect.y + centerRect.h / 2;
    SDL_FRect logsRect = {cx2 - rectW/2, cy2 - rectH/2, rectW, rectH};

    if (pt.x >= randRect.x && pt.x <= randRect.x + randRect.w &&
        pt.y >= randRect.y && pt.y <= randRect.y + randRect.h) return RAND_KORTA;

    if (pt.x >= logsRect.x && pt.x <= logsRect.x + logsRect.w &&
        pt.y >= logsRect.y && pt.y <= logsRect.y + logsRect.h) return SYSTEM_LOGS_KORTA;

    return 0;
}