#include "student.h"
#ifndef _STUDENT_C_
#define _STUDENT_C_ "student.mainbody"

/**
 * function name: 	getStudent
 * params:			void
 * return value:	student (An initialized student object)
 */
student getStudent(void) {
    student s = {0, {0}, {0}};
    return s;
}

/**
 * function name: 	readStudent
 * params:			out_stuptr (A stuptr's address which read
 *					successfully point to)
 * return value:	int (the stored object counts)
 * descrition:		by the out_stuptr param, the function changes
 *					the pointer of the value.
 */
int readStudent(stuptr* out_stuptr) {
    /** open the default file */
    FILE* file = fopen("./user", "rb+");

    /** if the file does not exists */
    if (file == NULL) {
        /** avoid the wild pointer */
        *out_stuptr = NULL;

        /** read 0 object*/
        return 0;
    } else {
        /** student list object */
        stuptr sptr = NULL;
        student temp_stu = {0, {0}, {0}};
        int cnt = 0;

        /** if read object successfully cnt + 1 */
        while (fread(&temp_stu, sizeof(student), 1, file) == 1) {
            cnt++;
        }

        /** get the total cnt of the student object */
        if (cnt > 0) {
            rewind(file);
            sptr = (stuptr)calloc(cnt, sizeof(student));
            fread(sptr, sizeof(student), cnt, file);
        }

        /** close the file stream */
        fclose(file);

        *out_stuptr = sptr;
        return cnt;
    }
}

/**
 * function name:	setNextStuAccount
 * params:			ref_stu (A student object's address which
 *					need to be initialized)
 * return value:	void
 * descrition:		auto set the student's account
 */
void setNextStuAccount(stuptr ref_stu) {
    stuptr stu_array = NULL;
    int nmemb = readStudent(&stu_array);
    if (stu_array == NULL && nmemb == 0) {
        ref_stu->account = 1001;
    } else {
        ref_stu->account = stu_array[nmemb - 1].account + 1;
        /** important! */
        free(stu_array);
    }
}

/**
 * function name:	appendStudent
 * return value:	true: if successfully add the student object
 *					to the file end
 *					false: user cancel the input, file open failed
 * descrition:		append a student object to the file tail
 */
bool appendStudent() {
    /** character buffer, read data from console */
    char buffer[512] = {0};
    char size_rec[3] = {0};
    int max_len = 0, cir_cnt = 0;
    bool ret = true;

    student stu = {0, {0}, {0}};
    setNextStuAccount(&stu);

    /** give the tips */
    printf("%s\n", "name & password must less than 32 char!");
    printf("%s\n", "over the size will be cut!");

    /** input name tip */
    printf("%s ", "Enter the student's name:>>> ");
    scanf("%[^\n]s", buffer);
    memcpy(stu.name, buffer, (strlen(buffer) >= 31 ? 31 : strlen(buffer)));

    /** input password tip */
    printf("%s ", "Enter the student's password:>>> ");
    scanf("%s", buffer);
    memcpy(stu.password, buffer, (strlen(buffer) >= 31 ? 31 : strlen(buffer)));

    printf("%s\n", "Confirm the informations you enter:>");
    sprintf(buffer, "%d", stu.account);

    size_rec[0] = max(strlen("stu account"), strlen(buffer)) + 2;
    size_rec[1] = max(strlen("stu name"), strlen(stu.name)) + 3;
    size_rec[2] = max(strlen("stu password"), strlen(stu.password)) + 3;
    max_len = size_rec[0] + size_rec[1] + size_rec[2] + 4;

    memset(buffer, '-', max_len);
    buffer[0] = buffer[size_rec[0] + 1] = '+';
    buffer[size_rec[0] + size_rec[1] + 2] = '+';
    buffer[max_len - 1] = '+';
    buffer[max_len] = 0;

    for (cir_cnt = 0; cir_cnt < 5; cir_cnt++) {
        /** table border */
        if (cir_cnt % 2 == 0) {
            printf("%s\n", buffer);
        } else {
            if (cir_cnt == 1) {
                printf("|%-*s", size_rec[0], "stu account");
                printf("|%-*s", size_rec[1], "stu name");
                printf("|%-*s|\n", size_rec[2], "stu password");
            } else {
                printf("|%-*d", size_rec[0], stu.account);
                printf("|%-*s", size_rec[1], stu.name);
                printf("|%-*s|\n", size_rec[2], stu.password);
            }
        }
    }

    setbuf(stdin, NULL);

    buffer[511] = 'Y';
    while (buffer[511] != 'Y' || buffer[511] != 'y' || buffer[511] != 'N' ||
           buffer[511] != 'n') {
        printf("Enter your choice(Y/N):>>>");
        if ((buffer[511] = getchar()) == 'Y' || buffer[511] == 'y') {
            /** confirm ok */
            FILE* file = fopen("./user", "ab+");
            if (file != NULL) {
                /** write to the file */
                fwrite(&stu, sizeof(stu), 1, file);
                /** close the file stream */
                fclose(file);
            } else {
                /** file open failed */
                ret &= false;
            }
            break;
        } else if (buffer[511] == 'N' || buffer[511] == 'n') {
            printf("User cancel the operation!\n");
            ret &= false;
            break;
        } else {
            continue;
        }
    }

    return ret;
}

/**
 * function name:	stuLogin
 * return value:	true: if can successfully login
 *					false: no such user or password & account are
 *					not correct
 * descrition:		check  if the student object has the right to
 *					log in
 */
bool stuLogin(student* temp_stu) {
    bool ret = true;
    int cir_cnt = 0;
    stuptr stu_array = NULL;
    int nmemb = readStudent(&stu_array);

    /** do not have any users */
    if (nmemb == 0 && stu_array == NULL) {
        printf("No any users, Please create user first!\n");
        ret &= false;
    } else {
        /** have users, check the account and password */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (stu_array[cir_cnt].account == (*temp_stu).account &&
                strcmp(stu_array[cir_cnt].password, (*temp_stu).password) ==
                    0) {
                ret |= true;
                *temp_stu = stu_array[cir_cnt];
                return ret;
            }
        }
        ret &= false;
        /** free the array */
        free(stu_array);
    }
    return ret;
}

/**
 * function name:	showStudent
 * descrition:		use table to show the informations detailed
 *					about the student
 */
void showStudent() {
    char buffer[256] = {0};
    char size_rec[4] = {4, 0};
    stuptr stu_array = NULL;
    int max_len = 0, nmemb = 0, cir_cnt = 0;

    nmemb = readStudent(&stu_array);
#if 0
	printf("%d\n", nmemb);
#endif
    if (nmemb == 0 && stu_array == NULL) {
        printf("No any users, Please create user first!\n");
        printf("Empty set!\n");
    } else {
        /** get every objects' max_len */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            sprintf(buffer, "%d", stu_array[cir_cnt].account);
            size_rec[1] = max(size_rec[1], strlen(buffer));
            size_rec[2] = max(size_rec[2], strlen(stu_array[cir_cnt].name));
            size_rec[3] = max(size_rec[3], strlen(stu_array[cir_cnt].password));
        }

        size_rec[0] = max(size_rec[0], strlen("stu No."));
        size_rec[1] = max(size_rec[1], strlen("stu account"));
        size_rec[2] = max(size_rec[2], strlen("stu name"));
        size_rec[3] = max(size_rec[3], strlen("stu password"));

        /** make the suitable size */
        for (cir_cnt = 0; cir_cnt < 4; cir_cnt++) {
            size_rec[cir_cnt] += 3;
        }

        max_len = size_rec[0] + size_rec[1] + size_rec[2] + size_rec[3] + 5;

        /** table border */
        memset(buffer, '-', 255);
        buffer[0] = buffer[size_rec[0] + 1] = '+';
        buffer[size_rec[0] + size_rec[1] + 2] = '+';
        buffer[size_rec[0] + size_rec[1] + size_rec[2] + 3] = '+';
        buffer[max_len - 1] = '+';
        buffer[max_len] = 0;

        for (cir_cnt = 0; cir_cnt < nmemb * 2 + 1; cir_cnt++) {
            if (cir_cnt % 2 == 0) {
                printf("%s\n", buffer);
            } else {
                if (cir_cnt == 1) {
                    printf("|%-*s", size_rec[0], "stu No.");
                    printf("|%-*s", size_rec[1], "stu account");
                    printf("|%-*s", size_rec[2], "stu name");
                    printf("|%-*s|\n", size_rec[3], "stu password");
                    printf("%s\n", buffer);
                    printf("|%-*d", size_rec[0], (cir_cnt - 1) / 2);
                    printf("|%-*d", size_rec[1],
                           stu_array[(cir_cnt - 1) / 2].account);
                    printf("|%-*s", size_rec[2],
                           stu_array[(cir_cnt - 1) / 2].name);
                    printf("|%-*s|\n", size_rec[3],
                           stu_array[(cir_cnt - 1) / 2].password);
                } else {
                    printf("|%-*d", size_rec[0], (cir_cnt - 1) / 2);
                    printf("|%-*d", size_rec[1],
                           stu_array[(cir_cnt - 1) / 2].account);
                    printf("|%-*s", size_rec[2],
                           stu_array[(cir_cnt - 1) / 2].name);
                    printf("|%-*s|\n", size_rec[3],
                           stu_array[(cir_cnt - 1) / 2].password);
                }
            }
        }
    }

    free(stu_array);
}

/**
 *
 *
 */
bool removeStudent() {
    bool ret = true;
    /** find a better way to really remove the student object */

    stuptr stu_array = NULL;
    int opt = 0, cir_cnt = 0, rest_obj = 0;
    int nmemb = readStudent(&stu_array);

    if (nmemb == 0 && stu_array == NULL) {
        /** no users */
        printf("No any users, Please create user first!\n");
        printf("Empty set!\n");
        ret &= false;
    } else {
        /** give the all student informations */
        showStudent();
        /** give the enter tip */
        printf("%s ", "Enter the student's account:>>>");
        scanf("%d", &opt);

        /** check whether the opt is equal to the account */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (stu_array[cir_cnt].account == opt) {
                /** remove directly */
                memset(&stu_array[cir_cnt], 0, sizeof(student));
                stu_array[cir_cnt].account = -1;
                ret |= true;
            } else {
                rest_obj++;
            }
        }

        if (rest_obj == nmemb) {
            /** Nothing changed */
            printf("No such student account!\n");
            printf("Please check the account first!\n");
            ret &= false;
        }

        if (ret != false) {
            /** object list has been modified */
            FILE* file = fopen("./user", "wb+");
            if (file == NULL) {
                printf("Open file Failed!\n");
                ret &= false;
            } else {
                for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
                    if (stu_array[cir_cnt].account != -1) {
                        fwrite(&stu_array[cir_cnt], sizeof(student), 1, file);
                    }
                }
                fclose(file);
                ret |= true;
            }
        }

        free(stu_array);
    }

    return ret;
}

bool resetStudent() {
    bool ret = true;
    int opt = 0, cir_cnt = 0, tindex = -1;
    stuptr stu_array = NULL;
    int nmemb = readStudent(&stu_array);

    if (nmemb == 0 && stu_array == NULL) {
        printf("No any users, Please create user first!\n");
        printf("Empty set!\n");
        ret &= false;
    } else {
        showStudent();

        /** give tips: */
        printf("%s ", "Please enter the student's account:>>>");
        scanf("%d", &opt);
        setbuf(stdin, NULL);

        /** find the obj index */
        for (cir_cnt = 0; cir_cnt < nmemb; cir_cnt++) {
            if (opt == stu_array[cir_cnt].account) {
                tindex = cir_cnt;
                break;
            }
        }

        /** check if it is legal */
        if (tindex < 0 || cir_cnt >= nmemb) {
            ret &= false;
            printf("No such account!\n");
        }

        if (ret != false) {
            /** if it is llegal */
            char buffer[256] = {0};
            char csize[3] = {0};
            int max_len = 0;

            student stu = {stu_array[tindex].account, {0}, {0}};

            printf("%s ", "Reenter the student's name:>>>");
            scanf("%[^\n]s", buffer);
            memcpy(stu.name, buffer,
                   strlen(buffer) >= 31 ? 31 : strlen(buffer));

            printf("%s ", "Reenter the student's password:>>>");
            scanf("%s", buffer);
            memcpy(stu.password, buffer,
                   strlen(buffer) >= 31 ? 31 : strlen(buffer));

            printf("%s\n", "Confirm the informations you modified!:>>");
            sprintf(buffer, "%d", stu.account);

            csize[0] = max(strlen("stu account"), strlen(buffer)) + 2;
            csize[1] = max(strlen("stu name"), strlen(stu.name)) + 2;
            csize[2] = max(strlen("stu password"), strlen(stu.password)) + 2;
            max_len = csize[0] + csize[1] + csize[2] + 4;

            memset(buffer, '-', 256);

            buffer[0] = buffer[csize[0] + 1] = '+';
            buffer[csize[0] + csize[1] + 2] = '+';
            buffer[max_len - 1] = '+';
            buffer[max_len] = 0;

            for (opt = 0; opt < 5; opt++) {
                if (opt % 2 == 0) {
                    printf("%s\n", buffer);
                } else {
                    if (opt == 1) {
                        printf("|%-*s", csize[0], "stu account");
                        printf("|%-*s", csize[1], "stu name");
                        printf("|%-*s|\n", csize[2], "stu password");
#if 0
						printf("%s\n", buffer);
						printf("|%-*d", csize[0], stu.account);
						printf("|%-*s", csize[1], stu.name);
						printf("|%-*s|\n", csize[2], stu.password);
#endif
                    } else {
                        printf("|%-*d", csize[0], stu.account);
                        printf("|%-*s", csize[1], stu.name);
                        printf("|%-*s|\n", csize[2], stu.password);
                    }
                }
            }

            buffer[255] = 'Y';
            while (buffer[255] != 'Y' || buffer[255] != 'y' ||
                   buffer[255] != 'N' || buffer[255] != 'n') {
                setbuf(stdin, NULL);
                printf("Confirm or not?(Y/N):>>> ");
                if ((buffer[255] = getchar()) == 'Y' || buffer[255] == 'y') {
                    stu_array[tindex] = stu;

                    FILE* file = fopen("./user", "wb+");
                    fwrite(stu_array, sizeof(student), nmemb, file);
                    fclose(file);
                    ret |= true;
                    break;
                } else if (buffer[255] == 'N' || buffer[255] == 'n') {
                    ret &= false;
                    break;
                } else {
                    continue;
                }
            }
        }

        free(stu_array);
    }

    return ret;
}

/** all functions finished */
double sum(void* val_array, int len, double (*add)(void*, int)) {
    return (*add)(val_array, len);
}
#endif