#ifndef __M_UTIL_H
#define __M_UTIL_H

#include <time.h>

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result);

#endif //__M_UTIL_H
