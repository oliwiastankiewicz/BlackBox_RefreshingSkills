#include "../include/timestamp.h"

#define MAX_HOURS 1

Timestamp timestamp_initialize()
{
    Timestamp current_time;

    current_time.seconds = 0;
    current_time.minutes = 0;
    current_time.hours = 0;

    return current_time;
}

bool timestamp_validate(int hours, int minutes, int seconds)
{
    if (hours >= 0 &&
        hours <= MAX_HOURS &&
        minutes >= 0 &&
        minutes <= 59 &&
        seconds >= 0 &&
        seconds <= 59)
    {
        return true;
    }
    return false;
}

Timestamp timestamp_create(int hours, int minutes, int seconds)
{
    if (!timestamp_validate(hours, minutes, seconds))
    {
        Timestamp invalid;
        invalid.seconds = -1;
        invalid.minutes = -1;
        invalid.hours = -1;
        return invalid;
    }

    Timestamp current_time;

    current_time.seconds = seconds;
    current_time.minutes = minutes;
    current_time.hours = hours;

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