#ifndef _CSELECT_H_
#define _CSELECT_H_ "cselect.h"
#undef LSCORE
#undef HSCORE
#define LSCORE 20
#define HSCORE 25

#include "course.h"
#include "student.h"

typedef struct sim_cs cselect;
typedef struct sim_cs* cselectptr;
struct sim_cs {
    student stu;
    course cours[12];
    char class[32];
    int tscore;
    int where;
};

extern cselect getCSelect();

extern bool selected(stuptr ref_login, cselectptr* out_csobj);

extern bool selecter(cselectptr ref_csobj);

extern bool saveInformation(cselectptr ref_csobj);

extern bool unselecter(cselectptr ref_csobj);

extern void showCSelect(cselectptr ref_csobj);

/** some unlinked functions **/
extern void show_menu_for_admin();
extern void show_menu_for_ouser();
double add(void* array, int len);
#include "cselect.c"
#endif