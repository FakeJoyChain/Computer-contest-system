#ifndef _CSELECT_C_
#define _CSELECT_C_ "cselect.mainbody"
#define LSCORE 20
#define HSCORE 25
#include "cselect.h"

#include <unistd.h>

cselect getCSelect() {
    cselect cs = {{0, {0}, {0}}, {0, 0, 0, 0, {0}, {0}, {0}}, {0}};
    return cs;
}

bool selected(stuptr ref_login, cselectptr* out_csobj) {
    bool ret = true;
    FILE* file = fopen("./cselect", "rb+");

    if (file == NULL) {
        /** if the file does not exist */
        printf("%s\n", "Do not have any course selection informations!");
        (*out_csobj) = NULL;
        ret = false;
    } else {
        /** create a temp cselect object */
        cselect cs = getCSelect();
        bool find = false;

        while (fread(&cs, sizeof(cselect), 1, file) == 1) {
            /** check the user information */
            if (cs.stu.account == ref_login->account &&
                strcmp(cs.stu.password, ref_login->password) == 0) {
                /** try to update the course select information object*/
                *out_csobj = (cselectptr)calloc(1, sizeof(cselect));
                **out_csobj = cs;

                /** update the object */
                *ref_login = cs.stu;

                /** make a flag that find the object */
                find |= true;
                break;
            }
        }

        /** if not find the object */
        if (find == false) {
            printf("%s\n", "Do not find such student user!");
            ret &= false;
            *out_csobj = NULL;
        } else {
            /** find such cselect object */
            ret |= true;
        }

        fclose(file);
        file = NULL;
    }
    return ret;
}

bool selecter(cselectptr ref_csobj) {
    bool ret = true;

    /** if course array is full return the outcome immediately*/
    if (ref_csobj->where < 0 || ref_csobj->where >= 12) {
        ret &= false;
        return ret;
    }

    /** if do not match the suitable score return the outcome immediately */
    if (ref_csobj->tscore < LSCORE || ref_csobj->tscore > HSCORE) {
        printf("Your total score is %d (20~25).\n", ref_csobj->tscore);
    }

    int opt = -1, cir_cnt = 0;
    courptr cour_array = NULL;
    int nmemb = 0;

    nmemb = readCourse(&cour_array);

    if (nmemb == 0 && cour_array == NULL) {
        printf("%s\n", "There has no any course information!");
        ret &= false;
    } else {
        /** give some tips of course information */
        showCourse();

        printf("%s ", "Enter the course id you need to choose:>>>");
        scanf("%d", &opt);

        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (cour_array[cir_cnt].cid == opt) {
                break;
            }
        }

        if (opt == -1 || cir_cnt >= nmemb) {
            printf("%s\n", "Not find such course!");
            ret &= false;
            free(cour_array);
            cour_array == NULL;
            return ret;
        }

        ref_csobj->tscore += cour_array[cir_cnt].cscore;
        ref_csobj->cours[ref_csobj->where++] = cour_array[cir_cnt];

        ret &= saveInformation(ref_csobj);
        free(cour_array);
        cour_array == NULL;
    }

    return ret;
}

bool unselecter(cselectptr ref_csobj) {
    bool ret = true;
    int opt = -1, cir = 0;

    if (ref_csobj->where <= 0) {
        /** if never select any courses return false */
        printf("%s\n", "You never select any courses!");
        ret &= false;
    } else {
        /** select some courses */
        printf("%s\n", "Here are your selected courses:>");
        showCSelect(ref_csobj);

        printf("\n%s\n", "Enter the course id that you need to unselect:>>>");
        scanf("%d", &opt);

        for (cir = 0; cir < ref_csobj->where; cir++) {
            if (ref_csobj->cours[cir].cid == opt) {
                /** find the match one */
                /** reset the total score */
                ref_csobj->tscore -= ref_csobj->cours[cir].cscore;

                /** let the behind one move to front */
                while (cir < ref_csobj->where - 1) {
                    ref_csobj->cours[cir] = ref_csobj->cours[cir + 1];
                }

                /** clear the block */
                memset(&ref_csobj->cours[ref_csobj->where - 1], 0,
                       sizeof(course));
                /** pointer minus one */
                ref_csobj->where--;
                /** get the save outcome, use it as the total return */
                ret &= saveInformation(ref_csobj);
                return ret;
            }
        }

        printf("%s\n", "Do not find such course id!");
    }
    return ret;
}

bool saveInformation(cselectptr ref_csobj) {
    bool ret = true;
    if (access("./cselect", F_OK) == 0) {
        /** to save the memory sizeof(cselect) ==~ 1k too large */
        FILE* file_new = fopen("./cselectt", "ab+");
        FILE* file_old = fopen("./cselect", "rb+");

        if (file_new == NULL || file_old == NULL) {
            printf("%s\n", "Open file failed!");
            file_new == NULL ? 1 : fclose(file_new);
            file_old == NULL ? 1 : fclose(file_old);
            ret &= false;
        } else {
            /** create a temp object */
            cselect cs = getCSelect();
            fwrite(ref_csobj, sizeof(cselect), 1, file_new);

            while (fread(&cs, sizeof(cselect), 1, file_old) == 1) {
                if (cs.stu.account != ref_csobj->stu.account) {
                    fwrite(&cs, sizeof(cselect), 1, file_new);
                }
            }
            fclose(file_new);
            fclose(file_old);
            unlink("./cselect");
            rename("./cselectt", "./cselect");
            file_old = file_new = NULL;
        }
    } else {
        FILE* file = fopen("./cselect", "ab+");
        if (file == NULL) {
            printf("%s\n", "Open file failed!");
            ret &= false;
        } else {
            fwrite(ref_csobj, sizeof(cselect), 1, file);
            fclose(file);
            file = NULL;
        }
    }
    return ret;
}

void showCSelect(cselectptr ref_csobj) {
    char buf[256] = {0};
    char* buf_t = NULL;
    unsigned char csize[8] = {0};
    int cir = 0;
    int max_len = 0;
    char* title[] = {"No.",  "Course ID",    "Course Name", "Scores", "Hours",
                     "Type", "Teacher Name", "Department",  NULL};
    buf_t = (char*)calloc(256, sizeof(char));

    /** get the titles' length */
    for (cir = 0; cir < 8; cir++) {
        csize[cir] = max(csize[cir], strlen(title[cir]));
    }

    sprintf(buf, "%d", ref_csobj->where - 1);
    csize[0] = max(csize[0], strlen(buf));

    /** get suitable column length */
    for (cir = 0; cir < ref_csobj->where; cir++) {
        /** id suitable column size */
        sprintf(buf, "%d", ref_csobj->cours[cir].cid);
        csize[1] = max(csize[1], strlen(buf));

        /** name suitable column size */
        csize[2] = max(csize[2], strlen(ref_csobj->cours[cir].cname));

        /** score suitable column size */
        sprintf(buf, "%d", ref_csobj->cours[cir].cscore);
        csize[3] = max(csize[3], strlen(buf));

        /** hour suitable column size */
        sprintf(buf, "%d", ref_csobj->cours[cir].cperiod);
        csize[4] = max(csize[4], strlen(buf));

        /** type suitable column size */
        sprintf(buf, "%d", ref_csobj->cours[cir].ctype);
        csize[5] = max(csize[5], strlen(buf));

        /** teacher name suitable column size */
        csize[6] = max(csize[6], strlen(ref_csobj->cours[cir].ctname));

        /** department name suitable column size */
        csize[7] = max(csize[7], strlen(ref_csobj->cours[cir].cdepart));
    }

    /** the char that need to set '-' */
    max_len = (int)sum(csize, 8, add) + 9;

    /** get the two part of legth */
    sprintf(buf_t, "Student ID:%d  Student Name:%s", ref_csobj->stu.account,
            ref_csobj->stu.name);

    /** build table border */
    memset(buf, '-', max_len);

    for (cir = 0; cir < 9; cir++) {
        if (cir == 0) {
            buf[cir] = '+';
            buf[csize[cir] + cir + 1] = '+';
        } else {
            int total = (int)sum(csize, cir, add);
            buf[total + cir] = '+';
        }
    }

    buf[max_len] = 0;

    printf("%s\n", buf);
    printf("|%-*s|\n", max_len - 2, buf_t);

    for (cir = 0; cir < ref_csobj->where * 2 + 1; cir++) {
        if (cir % 2 == 0) {
            printf("%s\n", buf);
        } else {
            if (cir == 1) {
                printf("|%-*s", csize[0], title[0]);
                printf("|%-*s", csize[1], title[1]);
                printf("|%-*s", csize[2], title[2]);
                printf("|%-*s", csize[3], title[3]);
                printf("|%-*s", csize[4], title[4]);
                printf("|%-*s", csize[5], title[5]);
                printf("|%-*s", csize[6], title[6]);
                printf("|%-*s|\n", csize[7], title[7]);
                printf("%s\n", buf);
                printf("|%-*d", csize[0], (cir - 1) / 2);
                printf("|%-*d", csize[1], ref_csobj->cours[(cir - 1) / 2].cid);
                printf("|%-*s", csize[2],
                       ref_csobj->cours[(cir - 1) / 2].cname);
                printf("|%-*d", csize[3],
                       ref_csobj->cours[(cir - 1) / 2].cscore);
                printf("|%-*d", csize[4],
                       ref_csobj->cours[(cir - 1) / 2].cperiod);
                printf("|%-*d", csize[5],
                       ref_csobj->cours[(cir - 1) / 2].ctype);
                printf("|%-*s", csize[6],
                       ref_csobj->cours[(cir - 1) / 2].ctname);
                printf("|%-*s|\n", csize[7],
                       ref_csobj->cours[(cir - 1) / 2].cdepart);
            } else {
                printf("|%-*d", csize[0], (cir - 1) / 2);
                printf("|%-*d", csize[1], ref_csobj->cours[(cir - 1) / 2].cid);
                printf("|%-*s", csize[2],
                       ref_csobj->cours[(cir - 1) / 2].cname);
                printf("|%-*d", csize[3],
                       ref_csobj->cours[(cir - 1) / 2].cscore);
                printf("|%-*d", csize[4],
                       ref_csobj->cours[(cir - 1) / 2].cperiod);
                printf("|%-*d", csize[5],
                       ref_csobj->cours[(cir - 1) / 2].ctype);
                printf("|%-*s", csize[6],
                       ref_csobj->cours[(cir - 1) / 2].ctname);
                printf("|%-*s|\n", csize[7],
                       ref_csobj->cours[(cir - 1) / 2].cdepart);
            }
        }
    }
    free(buf_t);
}
/** end of ordinary functions */

/** process handler function */
void show_menu_for_admin() {
    printf(
        "+-------------------------------------+\n"
        "|   Welcome to administrator system   |\n"
        "+----+--------------------------------+\n"
        "| 1. | show the courses' information  |\n"
        "+----+--------------------------------+\n"
        "| 2. | show the students' information |\n"
        "+----+--------------------------------+\n"
        "| 3. | add a new course object        |\n"
        "+----+--------------------------------+\n"
        "| 4. | add a new student object       |\n"
        "+----+--------------------------------+\n"
        "| 5. | reset student object           |\n"
        "+----+--------------------------------+\n"
        "| 6. | reset course object            |\n"
        "+----+--------------------------------+\n"
        "| 7. | remove student object          |\n"
        "+----+--------------------------------+\n"
        "| 8. | remove course object           |\n"
        "+----+--------------------------------+\n"
        "| 9. | exit the administrator system  |\n"
        "+----+--------------------------------+\n");
}

void show_menu_for_ouser() {
    printf(
        "+---------------------------------+\n"
        "|     Welcome to the system       |\n"
        "+---+-----------------------------+\n"
        "| 1.| personal information        |\n"
        "+---+-----------------------------+\n"
        "| 2.| show course only            |\n"
        "+---+-----------------------------+\n"
        "| 3.| select course               |\n"
        "+---+-----------------------------+\n"
        "| 4.| remove selected course      |\n"
        "+---+-----------------------------+\n"
        "| 5.| exit the system             |\n"
        "+---+-----------------------------+\n");
}

#ifndef _ADD_CHAR_
double add(void* array, int len) {
    double sum = 0;
    char* arr = (char*)array;
    int cir = 0;
    for (cir = 0; cir < len; cir++) {
        sum += (int)arr[cir];
    }
    return sum;
}
#endif

#endif