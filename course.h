#ifndef _COURSE_H_
#define _COURSE_H_ "course.h"

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

typedef enum sim_cour_type coutype;
enum sim_cour_type {
    MAJOR = 1,  /** major class */
    OPTIOANAL,  /** optional class */
    ART,        /** artical class */
    HUMANITIES, /** humanities class */
    SENCONDARY  /** sencondary class */
};

typedef struct sim_cour course;
typedef struct sim_cour* courptr;
struct sim_cour {
    int cid;          /** course id */
    char cscore;      /** course score */
    char ctype;       /** course type */
    short cperiod;    /** course times */
    char cname[32];   /** course name */
    char ctname[16];  /** teacher's name */
    char cdepart[32]; /** belongs to which department */
};

typedef struct courselist clist;
typedef struct courselist* clistptr;
struct courselist {
    course cour;             /** course object */
    struct courselist* next; /** next course address*/
};

extern course getCourse();

extern int readCourse(courptr* out_courptr);

extern clistptr convertToList(courptr cour_array, int nmemb,
                              bool shouldDestory);

extern void setNextCourID(courptr ref_cour);

extern bool appendCourse();

extern void showCourse();

extern void showCourType();

extern bool removeCourse();

extern bool resetCourse();

#include "course.c"
#endif