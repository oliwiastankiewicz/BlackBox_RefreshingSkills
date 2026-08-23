#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include <stdbool.h>

#define MAX_TEMPERATURE_SENSORS 10
#define MAX_DOORS 5
#define MAX_ALARMS 10

typedef enum
{
    INVALID_ENTITY,
    TEMPERATURE_SENSOR,
    DOOR,
    ALARM
} EntityType;

typedef struct
{
    int id;
    EntityType type;
    int id_in_entity_registry_type;
} Entity;

typedef struct
{
    Entity entity;
    float current_temperature;
    float min_temperature;
    float max_temperature;
} TemperatureSensor;

typedef enum
{
    OPEN,
    CLOSED
} DoorState;

typedef struct
{
    Entity entity;
    DoorState state;

} Door;

typedef enum
{
    ACTIVE,
    INACTIVE
} AlarmState;

typedef struct
{
    Entity entity;
    int door_id;
    AlarmState state;

} Alarm;

typedef struct
{
    TemperatureSensor temperature_sensors[MAX_TEMPERATURE_SENSORS];
    int temperature_sensor_count;

    Door doors[MAX_DOORS];
    int door_count;

    Alarm alarms[MAX_ALARMS];
    int alarm_count;

} EntityRegistry;

extern EntityRegistry entity_registry;

#endif