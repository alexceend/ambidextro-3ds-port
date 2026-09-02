#include <stdio.h>
#include <time.h>

void log_message(const char* msg) {
    FILE* f = fopen("sdmc:/log.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}