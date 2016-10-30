#include "util.h"

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    if (stop->tv_nsec < start->tv_nsec) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec + 1000000000 - start->tv_nsec;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return;
}
