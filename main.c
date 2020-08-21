#include "cselect.h"

static student stu_t;
static cselectptr csinfor;
static CONSOLE_SCREEN_BUFFER_INFO old_info;

int main(int argc, char *argv[]) {
    /** original console information */
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hand, &old_info);

    /** enter the mainbody part */
    char buffer[64] = {0};
    int flag_i = -1;
    char flag_c = -1;
    student stu = getStudent();

    /** get the account */
    printf("%s ", "Enter the student's account:>>>");
    scanf("%d", &stu.account);

    /** get the password */
    printf("%s ", "Enter the student's password:>>>");
    scanf("%s", buffer);
    while (getchar() != '\n')
        ;
    memcpy(stu.password, buffer, strlen(buffer) >= 31 ? 31 : strlen(buffer));

    if (stuLogin(&stu) == false) {
        printf("%s\n", "student login failed!\n");
        getchar();
        return -1;
    }

    stu_t = stu;
    if (strcmp(stu.name, "Administrator") == 0) {
        /** administrator ui */
        while (flag_c) {
            show_menu_for_admin();
            printf("%s", "Enter your chioce:>>>");
            scanf("%c", &flag_c);
            getchar();

            switch (flag_c) {
                case '1':
                    showCourse();
                    break;
                case '2':
                    showStudent();
                    break;
                case '3': {
                    if (appendCourse() != true) {
                        printf("%s\n", "Failed to add a new course object...");
                    } else {
                        printf("%s\n", "Add a course object successfully...");
                    }
                } break;
                case '4': {
                    if (appendStudent() != true) {
                        printf("%s\n", "Failed to add a new student object...");
                    } else {
                        printf("%s\n", "Add a student object successfully...");
                    }
                } break;
                case '5':
                    if (resetStudent() != true) {
                        printf("%s\n", "Failed to reset student object...");
                    } else {
                        printf("%s\n", "Reset student object successfully...");
                    }
                    break;
                case '6':
                    if (resetCourse() != true) {
                        printf("%s\n", "Reset course object failed...");
                    } else {
                        printf("%s\n", "reset course object successfully...");
                    }
                    break;

                case '7': {
                    if (removeStudent() != true) {
                        printf("%s\n", "Failed to remove student object...");
                    } else {
                        printf("%s\n", "Remove student object successfully...");
                    }
                } break;
                case '8': {
                    if (removeCourse() != true) {
                        printf("%s\n", "Failed to remove course object...");
                    } else {
                        printf("%s\n", "Remove course object successfully...");
                    }
                } break;
                case '9':
                    flag_c = 0;
                    break;
                default:
                    printf("%s\n", "Error Input");
                    break;
            }
            printf("%s\n", "Press any key to continue...");
            getchar();
            system("cls");
        }
        printf("Bye! Nice to see you next time!\n");
    } else {
        /** origin ui */
        while (flag_c) {
            show_menu_for_ouser();
            printf("%s", "Enter your choice:>>>");
            scanf("%c", &flag_c);
            getchar();

            switch (flag_c) {}
        }
        printf("Bye! Nice to see you next time!\n");
    }
    while (getchar() != '\n')
        ;
    getchar();
    return 0;
}
/**/