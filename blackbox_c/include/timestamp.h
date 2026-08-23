#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdbool.h>

typedef struct
{
    int seconds;
    int minutes;
    int hours;

} Timestamp;

Timestamp timestamp_initialize();

Timestamp timestamp_create(int hours, int minutes, int seconds);

void timestamp_advance(Timestamp *timestamp);

int timestamp_compare(const Timestamp *timestamp, const Timestamp *timestamp_other);

bool timestamp_validate(int hours, int minutes, int seconds);

#endif