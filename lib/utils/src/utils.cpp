#include "utils.h"

String stringFormat(const char *format, ...) {
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    va_end(arg);
    return temp;
}