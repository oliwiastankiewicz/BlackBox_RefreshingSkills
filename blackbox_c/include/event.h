// Initial: TEMPERATURE_READING, DOOR_OPENED, DOOR_CLOSED, ALARM_TRIGGERED, ALARM_CLEARED
// To add later: MACHINE_STARTED, MACHINE_STOPPED, JOB_CREATED, JOB_COMPLETED, DEVICE_FAILURE

#ifndef EVENT_H
#define EVENT_H

typedef enum {
    TEMPERATURE_READING,
    DOOR_OPENED,
    DOOR_CLOSED,
    ALARM_TRIGGERED,
    ALARM_CLEARED,
    INVALID
} EventType;

typedef struct {
    char timestamp[7];
    EventType type;
    int entity_id;
} Event;

Event event_create(char timestamp[7], EventType type, int entity_id);

void event_print(const Event *event);

#endif