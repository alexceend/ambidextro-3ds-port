#include <stdarg.h>
#include <stdio.h>

void log_message(const char *format, ...) {
  FILE *f = fopen("sdmc:/log.txt", "a");
  if (!f)
    return;

  va_list args;
  va_start(args, format);

  vfprintf(f, format, args);
  fprintf(f, "\n");

  va_end(args);

  fclose(f);
}
