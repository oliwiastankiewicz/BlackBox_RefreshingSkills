#include "../include/timestamp.h"

Timestamp timestamp_initialize()
{
    Timestamp current_time;

    current_time.seconds = 0;
    current_time.minutes = 0;
    current_time.hours = 0;

    return current_time;
}

void timestamp_advance(Timestamp *timestamp)
{
    timestamp->seconds++;

    if (timestamp->seconds >= 60)
    {
        timestamp->seconds = 0;
        timestamp->minutes++;
    }

    if (timestamp->minutes >= 60)
    {
        timestamp->minutes = 0;
        timestamp->hours++;
    }
}

int timestamp_compare(const Timestamp *timestamp, const Timestamp *timestamp_other)
{
    unsigned long int total_seconds = timestamp->hours * 3600 + timestamp->minutes * 60 + timestamp->seconds;
    unsigned long int total_seconds_other = timestamp_other->hours * 3600 + timestamp_other->minutes * 60 + timestamp_other->seconds;

    if (total_seconds > total_seconds_other)
    {
        return 1;
    }
    else if (total_seconds == total_seconds_other)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}