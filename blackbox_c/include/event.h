// Initial: TEMPERATURE_READING, DOOR_OPENED, DOOR_CLOSED, ALARM_TRIGGERED, ALARM_CLEARED
// To add later: MACHINE_STARTED, MACHINE_STOPPED, JOB_CREATED, JOB_COMPLETED, DEVICE_FAILURE
#ifndef EVENT_H
#define EVENT_H

#include "timestamp.h"
#include "entity_base.h"
#include <stdio.h>

typedef enum
{
    TEMPERATURE_READING,
    DOOR_OPENED,
    DOOR_CLOSED,
    ALARM_TRIGGERED,
    ALARM_CLEARED,
    INVALID
} EventType;

typedef struct
{
    Timestamp timestamp;
    EventType type;
    Entity entity;

    float temperature;
} Event;

Event event_create(Timestamp timestamp, EventType type, Entity entity);
Event event_create_invalid();

void event_print(const Event *event);
void event_write_to_file(const Event *event, FILE *file);

#endif