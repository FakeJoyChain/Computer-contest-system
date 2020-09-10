#define AUTO_APPEND

#include "competitor.h"

int main(int argc, char* argv[]) {
    //competitors competitor = get_competitor();
    competitors* cptr;
    int n = read_competitors(&cptr);
    printf("%d, %p\n", n, cptr);
    append_competitor();
    return 0;
}