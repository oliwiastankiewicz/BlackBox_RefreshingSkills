#include <stdio.h>
#include "../include/event.h"

Event event_create(Timestamp *timestamp, EventType type, int entity_id)
{
    Event event;

    event.timestamp = *timestamp;

    event.type = type;
    event.entity_id = entity_id;

    return event;
}

const char *event_type_to_string(EventType type)
{

    switch (type)
    {
    case TEMPERATURE_READING:
        return "TEMPERATURE_READING";
    case DOOR_OPENED:
        return "DOOR_OPENED";
    case DOOR_CLOSED:
        return "DOOR_CLOSED";
    case ALARM_TRIGGERED:
        return "ALARM_TRIGGERED";
    case ALARM_CLEARED:
        return "ALARM_CLEARED";
    default:
        return "UNKNOWN";
    }
}

void event_print(const Event *event)
{

    const char *event_type = event_type_to_string(event->type);
    const Timestamp time = event->timestamp;

    printf("%02d:%02d:%02d %s entity = %d\n", time.hours, time.minutes, time.seconds, event_type, event->entity_id);
}