#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    void *pool = NULL;
    int used = 0;
    srand(time(NULL));
    char buf[256];

    printf("Hello\n");
    free(pool);
    return 0;
}
