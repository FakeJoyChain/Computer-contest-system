#include "competitor.h"

int main(int argc, char* argv[]) {
    // competitors competitor = get_competitor();
    competitors* cptr;
    int n = read_competitors(&cptr);
    printf("%d, %p\n", n, cptr);
    return 0;
}