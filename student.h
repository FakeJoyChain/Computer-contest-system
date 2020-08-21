#ifndef _STUDENT_H_
#define _STUDENT_H_ "student.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _LINUX
#include <unistd.h>
#endif
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sim_stu student;
typedef struct sim_stu* stuptr;
struct sim_stu {
    int account;
    char name[32];
    char password[32];
};

/** get a initialized student object */
extern student getStudent(void);

/**
 *  through the default file, read objects
 *  from the file
 */
extern int readStudent(stuptr* out_stuptr);

/** auto set student's account */
extern void setNextStuAccount(stuptr ref_stu);

/** append a student object to the file tail*/
extern bool appendStudent();

/** check if the student object has the right to log in */
extern bool stuLogin(student* temp_stu);

/** show all the student information */
extern void showStudent();

/** remove one Student by some condition */
extern bool removeStudent();

extern bool resetStudent();

double sum(void* val_array, int len, double (*add)(void*, int));

#include "student.c"
#endif