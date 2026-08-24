#include <stdio.h>
#include "../include/event.h"

Event event_create(Timestamp timestamp, EventType type, Entity entity)
{
    Event event;

    event.timestamp = timestamp;
    event.type = type;
    event.entity = entity;
    event.temperature = 0.0f;

    return event;
}

Event event_create_invalid(void)
{
    Event event;

    event.timestamp = timestamp_initialize();
    event.type = INVALID;

    event.entity.id = -1;
    event.entity.type = INVALID_ENTITY;
    event.entity.id_in_entity_registry_type = -1;

    event.temperature = 0.0f;

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
        return "INVALID";
    }
}

void event_print(const Event *event)
{

    const char *event_type = event_type_to_string(event->type);
    const Timestamp time = event->timestamp;

    if (event->type == TEMPERATURE_READING)
    {
        printf("%02d:%02d:%02d %s %.1f°C entity = %d\n", time.hours, time.minutes, time.seconds, event_type, event->temperature, event->entity.id);
    }
    else
    {
        printf("%02d:%02d:%02d %s entity = %d\n", time.hours, time.minutes, time.seconds, event_type, event->entity.id);
    }
}

void event_write_to_file(const Event *event, FILE *file)
{

    const char *event_type = event_type_to_string(event->type);
    const Timestamp time = event->timestamp;

    if (event->type == TEMPERATURE_READING)
    {
        fprintf(file, "%02d:%02d:%02d %s %.1f°C entity = %d\n", time.hours, time.minutes, time.seconds, event_type, event->temperature, event->entity.id);
    }
    else
    {
        fprintf(file, "%02d:%02d:%02d %s entity = %d\n", time.hours, time.minutes, time.seconds, event_type, event->entity.id);
    }
}