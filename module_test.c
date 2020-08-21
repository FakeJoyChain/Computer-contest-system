#include "cselect.h"

typedef void (*fptr)(void);
void launch(fptr function);
void showMenu();
void as();
void rsts();
void rms();
void ss();

void ac();
void rstc();
void rmc();
void sc();

void cst();

int main(int argc, char *argv[]) {
    char ch = 0;
    fptr functions[15] = {
        NULL, as, rsts, rms, ss, ac, rstc, rmc, sc, cst
    };
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        showMenu();
        while(getchar() != '\n');
        printf("%s:>>>", "Enter your choice");
        ch = getchar();

        while(getchar() != '\n');

        functions[ch - '0']();

        /** eat the rest of the enter char */
        while(getchar() != '\n');
        printf("%s...\n", "Main Loop");
    }
    return 0;
}

void showMenu() {
    printf(
        "\
1. append student\n\
2. reset student\n\
3. remove student\n\
4. show students\n\
5. append course\n\
6. reset course\n\
7. remove course\n\
8. show courses\n\
9. cselect test\n\
");
}

/** append student */
void as() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        appendStudent();
        while(getchar() != '\n');
    }
    system("cls");
}

/** reset student */
void rsts() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        resetStudent();
        while(getchar() != '\n');
    }
    system("cls");
}

/** remove student */
void rms() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        removeStudent();
        while(getchar() != '\n');
    }
    system("cls");
}

void ss() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        showStudent();
    }
    system("cls");
}

/** append aourse */
void ac() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        appendCourse();
        while(getchar() != '\n');
    }
    system("cls");
}

void rstc() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        resetCourse();
        while(getchar() != '\n');
    }
    system("cls");
}

void rmc() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        removeCourse();
        while(getchar() != '\n');
    }
    system("cls");
}


void sc() {
    system("cls");
    while(printf("%s...", "Print any key(except #) to continue"), getchar() != '#') {
        showCourse();
    }
    system("cls");
}

void cst() {
    student stu = {1013, "Mingyue Wang", "0000000000000000"};
    cselectptr csinfo = NULL;
    while (readInformation(&csinfo, &stu), getchar() != '#');
}