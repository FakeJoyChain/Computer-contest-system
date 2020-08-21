#include "course.h"
#ifndef _COURSE_C_
#define _COURSE_C_ "course.mainbody"

course getCourse() {
    course c = {0, 0, 0, 0, {0}, {0}, {0}};
    return c;
}

int readCourse(courptr *out_courptr) {
    /** open the default file */
    FILE *file = fopen("./course", "rb+");

    /** if the file does not exists */
    if (file == NULL) {
        /** avoid the wild pointer */
        *out_courptr = NULL;
        /** object memb cnt */
        return 0;
    } else {
        /** course list */
        course temp_cour = {0, 0, 0, 0, {0}, {0}, {0}};
        courptr cptr = NULL;
        int cnt = 0;

        /** get the course objects count */
        while (fread(&temp_cour, sizeof(course), 1, file) == 1) {
            cnt++;
        }

        /** if has object try to malloc memory */
        if (cnt > 0) {
            rewind(file);
            cptr = (courptr)calloc(cnt, sizeof(course));
            fread(cptr, sizeof(course), cnt, file);
        }

        /** remember to clase the file stream */
        fclose(file);

        /** modify the pointer */
        *out_courptr = cptr;
        return cnt;
    }
}

clistptr convertToList(courptr cour_array, int nmemb, bool shouldDestory) {
    clistptr clst = NULL;
    clistptr rhead = NULL;

    /** if the array need to convert is 0 object or object is null */
    if (nmemb <= 0 || cour_array == NULL) {
        return clst;
    } else {
        /** init the head node */
        clst = (clistptr)calloc(1, sizeof(clist));
        rhead = clst;
        int cir_cnt = 0;

        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++, clst = clst->next) {
            clst->next = (clistptr)calloc(1, sizeof(course));
            clst->next->cour = cour_array[cir_cnt];
        }
    }

    if (cour_array == NULL || shouldDestory == false) {
        /** refuse to destory */
    } else {
        if (cour_array != NULL && shouldDestory == true) {
            free(cour_array);
        }
    }

    return rhead;
}

void setNextCourID(courptr ref_cour) {
    courptr cour_array = NULL;
    int nmemb = readCourse(&cour_array);
    if (nmemb == 0 && cour_array == NULL) {
        /** default number */
        ref_cour->cid = 1001;
    } else {
        ref_cour->cid = cour_array[nmemb - 1].cid + 1;
        /** important! */
        free(cour_array);
    }
}

bool appendCourse() {
    char buffer[256] = {0};
    char size_rec[7] = {0};
    int max_len = 0, cir_cnt = 0;
    int opt = 0;
    bool ret = true;

    course cour = {0, 0, 0, 0, {0}, {0}, {0}};

    /** give the tips first! */
    printf("%s\n", "Enter the course information!");
    printf("%s\n",
           "Course name & course department must be less than 32 char!");
    printf("%s\n", "Course teacher name must be less than 16 char!");
    printf("%s\n", "The information over size will be cut!");

    /** set default course id */
    setNextCourID(&cour);

    printf("%s ", "Please enter the course's name:>>>");
    scanf("%[^\n]s", buffer);
    memcpy(cour.cname, buffer, (strlen(buffer) >= 31 ? 31 : strlen(buffer)));
    while (getchar() != '\n')
        ;

    printf("%s ", "Please enter the course's teacher name:>>>");
    scanf("%[^\n]s", buffer);
    memcpy(cour.ctname, buffer, (strlen(buffer) >= 15 ? 15 : strlen(buffer)));
    while (getchar() != '\n')
        ;

    printf("%s ", "Please enter the course's department:>>>");
    scanf("%[^\n]s", buffer);
    memcpy(cour.cdepart, buffer, (strlen(buffer) >= 31 ? 31 : strlen(buffer)));
    while (getchar() != '\n')
        ;

    printf("%s ", "Please enter the course's score:>>>");
    scanf("%d", &opt);
    while (getchar() != '\n')
        ;
    cour.cscore = (char)opt;

    printf("%s ", "Please enter the course's class hour:>>>");
    scanf("%hd", &opt);
    while (getchar() != '\n')
        ;
    cour.cperiod = (char)opt;

    printf("%s\n", "Please enter the course's type.");
    printf("%s\n", "Enter 1 represents major course,");
    printf("%s\n", "Enter 2 represents optianal course,");
    printf("%s\n", "Enter 3 represents art course,");
    printf("%s\n", "Enter 4 represents humanities course,");
    printf("%s\n", "Enter 5 represents sencondary course.");
    opt = 0;

    while (opt < 1 || opt > 5) {
        printf("%s ", "Enter you choice:>>>");
        scanf("%d", &opt);
        cour.ctype = (char)opt;
        while (getchar() != '\n')
            ;
    }

    printf("%s\n", "Confrim the informations you enter:>>");
    /** build table */
    sprintf(buffer, "%d", cour.cid);
    size_rec[0] = max(strlen("course id"), strlen(buffer));

    sprintf(buffer, "%d", cour.cscore);
    size_rec[1] = max(strlen("score"), strlen(buffer)) + 1;

    sprintf(buffer, "%d", cour.ctype);
    size_rec[2] = max(strlen("course type"), strlen(buffer));

    sprintf(buffer, "%d", cour.cperiod);
    size_rec[3] = max(strlen("course period"), strlen(buffer));

    size_rec[4] = max(strlen("course name"), strlen(cour.cname)) + 2;
    size_rec[5] = max(strlen("teacher name"), strlen(cour.ctname)) + 2;
    size_rec[6] = max(strlen("department"), strlen(cour.cdepart)) + 2;

    for (cir_cnt = 0, max_len = 0; cir_cnt < 7; cir_cnt++) {
        max_len += size_rec[cir_cnt];
    }

    max_len += 8;

    memset(buffer, '-', 256);

    buffer[0] = buffer[size_rec[0] + 1] =
        buffer[size_rec[0] + size_rec[1] + 2] = '+';
    buffer[size_rec[0] + size_rec[1] + size_rec[2] + 3] = '+';
    buffer[size_rec[0] + size_rec[1] + size_rec[2] + size_rec[3] + 4] = '+';
    buffer[size_rec[0] + size_rec[1] + size_rec[2] + size_rec[3] + size_rec[4] +
           5] = '+';
    buffer[max_len - 8 - size_rec[6] + 6] = '+';
    buffer[max_len - 1] = '+';
    buffer[max_len] = 0;

    for (cir_cnt = 0; cir_cnt < 5; cir_cnt++) {
        if (cir_cnt % 2 == 0) {
            /** printf table border */
            printf("%s\n", buffer);
        } else {
            if (cir_cnt == 1) {
                printf("|%-*s", size_rec[0], "course id");
                printf("|%-*s", size_rec[1], "score");
                printf("|%-*s", size_rec[2], "course type");
                printf("|%-*s", size_rec[3], "course period");
                printf("|%-*s", size_rec[4], "course name");
                printf("|%-*s", size_rec[5], "teacher name");
                printf("|%-*s|\n", size_rec[6], "department");
            } else {
                printf("|%-*d", size_rec[0], cour.cid);
                printf("|%-*d", size_rec[1], cour.cscore);
                printf("|%-*d", size_rec[2], cour.ctype);
                printf("|%-*d", size_rec[3], cour.cperiod);
                printf("|%-*s", size_rec[4], cour.cname);
                printf("|%-*s", size_rec[5], cour.ctname);
                printf("|%-*s|\n", size_rec[6], cour.cdepart);
            }
        }
    }

    buffer[255] = '!';
    while (buffer[255] != 'Y' || buffer[255] != 'y' || buffer[255] != 'N' ||
           buffer[255] != 'n') {
        printf("%s ", "Enter your choice(Y/N):>>>");
        if ((buffer[255] = getchar()) == 'Y' || buffer[255] == 'y') {
            /** try to open the file */
            FILE *file = fopen("./course", "ab+");

            if (file == NULL) {
                ret &= false;
            } else {
                fwrite(&cour, sizeof(course), 1, file);
                fclose(file);
                ret |= true;
            }
            break;

        } else if (buffer[255] == 'N' || buffer[255] == 'n') {
            printf("User cancel the input!\n");
            ret &= false;
            break;
        }
        while (getchar() != '\n')
            ;
    }

    return ret;
}

void showCourse() {
    char buffer[256] = {0};
    char csize[8] = {0};
    int max_len = 0, cir_cnt = 0;
    courptr cour_array = NULL;
    int nmemb = readCourse(&cour_array);

    if (nmemb == 0 && cour_array == NULL) {
        printf("No any courses, please create course first!\n");
        printf("Empty Set!\n");
    } else {
        /** give the course types! */
        showCourType();

        /** get the title's length */
        sprintf(buffer, "%d", nmemb);
        csize[0] = max(strlen("No."), strlen(buffer));
        csize[1] = max(strlen("Course ID"), csize[1]);
        csize[2] = max(strlen("Course Name"), csize[2]);
        csize[3] = max(strlen("Score"), csize[3]);
        csize[4] = max(strlen("Hours"), csize[4]);
        csize[5] = max(strlen("Teacher"), csize[5]);
        csize[6] = max(strlen("Course Type"), csize[6]);
        csize[7] = max(strlen("Department"), csize[7]);

        /** find suitable width */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            sprintf(buffer, "%d", cour_array[cir_cnt].cid);
            csize[1] = max(strlen(buffer), csize[1]);

            csize[2] = max(strlen(cour_array[cir_cnt].cname), csize[2]);

            sprintf(buffer, "%d", cour_array[cir_cnt].cscore);
            csize[3] = max(strlen(buffer), csize[3]);

            sprintf(buffer, "%d", cour_array[cir_cnt].cperiod);
            csize[4] = max(strlen(buffer), csize[4]);

            csize[5] = max(strlen(cour_array[cir_cnt].ctname), csize[5]);

            sprintf(buffer, "%d", cour_array[cir_cnt].ctype);
            csize[6] = max(strlen(buffer), csize[6]);

            csize[7] = max(strlen(cour_array[cir_cnt].cdepart), csize[7]);
        }

#if 0
        for(cir_cnt = 0; cir_cnt < 8; cir_cnt++) {
            /** width expand*/
            csize[cir_cnt] += 1;
        }
#endif

        /** get max length */
        for (cir_cnt = 0, max_len = 0; cir_cnt < 8; cir_cnt++) {
            max_len += csize[cir_cnt];
        }

        max_len += 9;
        memset(buffer, '-', 256);

        /** build table border */
        for (cir_cnt = 0; cir_cnt < 9; cir_cnt++) {
            if (cir_cnt == 0) {
                buffer[cir_cnt] = '+';
                buffer[csize[cir_cnt] + cir_cnt + 1] = '+';
            } else {
                int total = 0;
                int y = 0;
                for (y = 0; y < cir_cnt; y++) {
                    total += csize[y];
                }

                buffer[total + cir_cnt] = '+';
            }
        }

        buffer[max_len] = 0;

        for (cir_cnt = 0; cir_cnt < 2 * nmemb + 1; cir_cnt++) {
            if (cir_cnt % 2 == 0) {
                printf("%s\n", buffer);
            } else {
                if (cir_cnt == 1) {
                    /** printf title and first set */
                    printf("|%-*s", csize[0], "No.");
                    printf("|%-*s", csize[1], "Course ID");
                    printf("|%-*s", csize[2], "Course Name");
                    printf("|%-*s", csize[3], "Score");
                    printf("|%-*s", csize[4], "Hours");
                    printf("|%-*s", csize[5], "Teacher");
                    printf("|%-*s", csize[6], "Course Type");
                    printf("|%-*s|\n", csize[7], "Department");
                    printf("%s\n", buffer);
                    printf("|%-*d", csize[0], (cir_cnt - 1) / 2);
                    printf("|%-*d", csize[1],
                           cour_array[(cir_cnt - 1) / 2].cid);
                    printf("|%-*s", csize[2],
                           cour_array[(cir_cnt - 1) / 2].cname);
                    printf("|%-*d", csize[3],
                           cour_array[(cir_cnt - 1) / 2].cscore);
                    printf("|%-*d", csize[4],
                           cour_array[(cir_cnt - 1) / 2].cperiod);
                    printf("|%-*s", csize[5],
                           cour_array[(cir_cnt - 1) / 2].ctname);
                    printf("|%-*d", csize[6],
                           cour_array[(cir_cnt - 1) / 2].ctype);
                    printf("|%-*s|\n", csize[7],
                           cour_array[(cir_cnt - 1) / 2].cdepart);
                } else {
                    printf("|%-*d", csize[0], (cir_cnt - 1) / 2);
                    printf("|%-*d", csize[1],
                           cour_array[(cir_cnt - 1) / 2].cid);
                    printf("|%-*s", csize[2],
                           cour_array[(cir_cnt - 1) / 2].cname);
                    printf("|%-*d", csize[3],
                           cour_array[(cir_cnt - 1) / 2].cscore);
                    printf("|%-*d", csize[4],
                           cour_array[(cir_cnt - 1) / 2].cperiod);
                    printf("|%-*s", csize[5],
                           cour_array[(cir_cnt - 1) / 2].ctname);
                    printf("|%-*d", csize[6],
                           cour_array[(cir_cnt - 1) / 2].ctype);
                    printf("|%-*s|\n", csize[7],
                           cour_array[(cir_cnt - 1) / 2].cdepart);
                }
            }
        }

        free(cour_array);
    }
}

void showCourType() {
    char buffer[128] = {0};
    char csize[2] = {4, 0};
    int max_len = 0;
    csize[0] = max(strlen("No."), csize[0]);
    csize[1] = max(strlen("Course Type"), csize[1]);
    csize[1] = max(strlen("major"), csize[1]);
    csize[1] = max(strlen("optianal"), csize[1]);
    csize[1] = max(strlen("art"), csize[1]);
    csize[1] = max(strlen("humanities"), csize[1]);
    csize[1] = max(strlen("sencondary"), csize[1]) + 2;

    max_len += csize[0];
    max_len += csize[1];
    max_len += 3;

    memset(buffer, '-', 128);

    buffer[0] = buffer[csize[0] + 1] = buffer[csize[0] + csize[1] + 2] = '+';
    buffer[max_len - 1] = '+';
    buffer[max_len] = 0;

    printf("%s\n", buffer);
    printf("|%-*s", csize[0], "No.");
    printf("|%-*s|\n", csize[1], "Course Type");

    printf("%s\n", buffer);
    printf("|%-*d", csize[0], 1);
    printf("|%-*s|\n", csize[1], "major");

    printf("%s\n", buffer);
    printf("|%-*d", csize[0], 2);
    printf("|%-*s|\n", csize[1], "optianal");

    printf("%s\n", buffer);
    printf("|%-*d", csize[0], 3);
    printf("|%-*s|\n", csize[1], "art");

    printf("%s\n", buffer);
    printf("|%-*d", csize[0], 4);
    printf("|%-*s|\n", csize[1], "humanities");

    printf("%s\n", buffer);
    printf("|%-*d", csize[0], 5);
    printf("|%-*s|\n", csize[1], "sencondary");
    printf("%s\n", buffer);
}

bool removeCourse() {
    bool ret = true;
    int opt = 0, nmemb = 0, rest_obj = 0;
    int cir_cnt = 0;
    courptr cour_array = NULL;

    /** try to read the objects */
    nmemb = readCourse(&cour_array);

    if (nmemb == 0 && cour_array == NULL) {
        printf("No any courses, please create course first!\n");
        printf("Empty Set!\n");
        ret &= false;
    } else {
        /** show all the course objects */
        showCourse();
        printf("%s ", "Enter the course id:>>>");
        scanf("%d", &opt);
        while (getchar() != '\n')
            ;

        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (cour_array[cir_cnt].cid == opt) {
                memset(&cour_array[cir_cnt], 0, sizeof(course));
                cour_array[cir_cnt].cid = -1;
            } else {
                rest_obj++;
            }
        }

        if (rest_obj >= nmemb) {
            /** that means there has one object changed*/
            printf("No such course id!\n");
            printf("Please check the course id first!\n");
            ret &= false;
        }

        if (ret != false) {
            FILE *file = fopen("./course", "wb+");

            if (file == NULL) {
                printf("%s\n", "Open file Failed!");
                ret &= false;
            } else {
                for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
                    if (cour_array[cir_cnt].cid != -1) {
                        fwrite(&cour_array[cir_cnt], sizeof(course), 1, file);
                    }
                }
                fclose(file);
                ret |= true;
            }
        }

        /** important! */
        free(cour_array);
    }
    return ret;
}

bool resetCourse() {
    bool ret = true;
    int opt = 0, cir_cnt = 0, nmemb = 0, tindex = -1;
    courptr cour_array = NULL;

    /** try to get the objects */
    nmemb = readCourse(&cour_array);

    if (nmemb == 0 && cour_array == NULL) {
        printf("No any courses, please create course first!\n");
        printf("Empty Set!\n");
        ret &= false;
    } else {
        /** give the informations */
        showCourse();

        printf("%s ", "Please enter the course id:>>>");
        scanf("%d", &opt);
        while (getchar() != '\n')
            ;

        /** find right index */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (opt == cour_array[cir_cnt].cid) {
                tindex = cir_cnt;
                break;
            }
        }

        if (tindex < 0 || cir_cnt >= nmemb) {
            ret &= false;
            printf("%s\n", "No such course id!");
        }

        if (ret != false) {
            /** if every thing is llegal */
            char buffer[256] = {0};
            char csize[7] = {0};
            int max_len = 0;
            course cour = {cour_array[tindex].cid, 0, 0, 0, {0}, {0}, {0}};

            printf("%s ", "Please reenter the course name:>>>");
            scanf("%[^\n]s", buffer);
            memcpy(cour.cname, buffer,
                   strlen(buffer) >= 31 ? 31 : strlen(buffer));
            while (getchar() != '\n')
                ;

            printf("%s ", "Please reenter the teacher's name:>>>");
            scanf("%[^\n]s", buffer);
            memcpy(cour.ctname, buffer,
                   strlen(buffer) >= 15 ? 15 : strlen(buffer));
            while (getchar() != '\n')
                ;

            printf("%s ", "Please reenter the course's department:>>>");
            scanf("%[^\n]s", buffer);
            memcpy(cour.cdepart, buffer,
                   (strlen(buffer) >= 31 ? 31 : strlen(buffer)));
            while (getchar() != '\n')
                ;

            printf("%s ", "Please reenter the course's score:>>>");
            scanf("%d", &opt);
            while (getchar() != '\n')
                ;
            cour.cscore = (char)opt;

            printf("%s ", "Please reenter the course's hours:>>>");
            scanf("%d", &opt);
            while (getchar() != '\n')
                ;
            cour.cperiod = (char)opt;

            printf("%s\n", "Please enter the course's type.");
            printf("%s\n", "Enter 1 represents major course,");
            printf("%s\n", "Enter 2 represents optianal course,");
            printf("%s\n", "Enter 3 represents art course,");
            printf("%s\n", "Enter 4 represents humanities course,");
            printf("%s\n", "Enter 5 represents sencondary course.");

            opt = -1;
            while (opt < 1 || opt > 5) {
                printf("%s ", "Enter you choice:>>>");
                scanf("%d", &opt);
                cour.ctype = (char)opt;
            }
            while (getchar() != '\n')
                ;

            printf("%s\n", "Confrim the informations you enter:>>");
            /** get every columns max width */
            char *cols[] = {"Course ID", "Course Name", "Score",      "Hours",
                            "Type",      "Teacher",     "Department", NULL};

            for (cir_cnt = 0; cols[cir_cnt] != NULL && cir_cnt < 7; cir_cnt++) {
                csize[cir_cnt] = max(strlen(cols[cir_cnt]), csize[cir_cnt]);
            }

            sprintf(buffer, "%d", cour.cid);
            csize[0] = max(strlen(buffer), csize[0]);

            csize[1] = max(strlen(cour.cname), csize[1]) + 2;

            sprintf(buffer, "%d", cour.cscore);
            csize[2] = max(strlen(buffer), csize[2]);

            sprintf(buffer, "%d", cour.cperiod);
            csize[3] = max(strlen(buffer), csize[3]);

            sprintf(buffer, "%d", cour.ctype);
            csize[4] = max(strlen(buffer), csize[4]);

            csize[5] = max(strlen(cour.ctname), csize[5]) + 2;

            csize[6] = max(strlen(cour.cdepart), csize[6]) + 2;

            for (cir_cnt = 0, max_len = 0; cir_cnt < 7; cir_cnt++) {
                max_len += csize[cir_cnt];
            }

            max_len += 8;

            /** build table */
            memset(buffer, '-', max_len);
            for (cir_cnt = 0; cir_cnt < 7; cir_cnt++) {
                if (cir_cnt == 0) {
                    buffer[cir_cnt] = '+';
                    buffer[csize[cir_cnt] + cir_cnt + 1] = '+';
                } else {
                    int y = 0, total = 0;
                    for (y = 0; y < cir_cnt; y++) {
                        total += csize[y];
                    }
                    buffer[total + cir_cnt] = '+';
                }
            }
            buffer[max_len - 1] = '+';
            buffer[max_len] = 0;

            for (cir_cnt = 0; cir_cnt < 5; cir_cnt++) {
                if (cir_cnt % 2 == 0) {
                    /** printf title */
                    printf("%s\n", buffer);
                } else {
                    if (cir_cnt == 1) {
                        printf("|%-*s", csize[0], cols[0]);
                        printf("|%-*s", csize[1], cols[1]);
                        printf("|%-*s", csize[2], cols[2]);
                        printf("|%-*s", csize[3], cols[3]);
                        printf("|%-*s", csize[4], cols[4]);
                        printf("|%-*s", csize[5], cols[5]);
                        printf("|%-*s|\n", csize[6], cols[6]);
                    } else {
                        printf("|%-*d", csize[0], cour.cid);
                        printf("|%-*s", csize[1], cour.cname);
                        printf("|%-*d", csize[2], cour.cscore);
                        printf("|%-*d", csize[3], cour.cperiod);
                        printf("|%-*d", csize[4], cour.ctype);
                        printf("|%-*s", csize[5], cour.ctname);
                        printf("|%-*s|\n", csize[6], cour.cdepart);
                    }
                }
            }

            buffer[255] = 'k';
            while (buffer[255] != 'N' || buffer[255] != 'n' ||
                   buffer[255] != 'Y' || buffer[255] != 'y') {
                printf("%s ", "Enter your choice(Y/N):>>>");
                if ((buffer[255] = getchar()) == 'Y' || buffer[255] == 'y') {
                    cour_array[tindex] = cour;
                    FILE *file = fopen("./course", "wb+");
                    if (file == NULL) {
                        ret &= false;
                    } else {
                        fwrite(cour_array, sizeof(course), nmemb, file);
                        fclose(file);
                    }

                    break;
                } else if (buffer[255] == 'N' || buffer[255] == 'n') {
                    printf("%s\n", "User cancel the input!");
                    ret &= false;
                    break;
                }
                while (getchar() != '\n')
                    ;
            }
        }
        free(cour_array);
    }
    return ret;
}

#endif