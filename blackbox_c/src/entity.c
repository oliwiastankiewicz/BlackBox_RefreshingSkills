#include "../include/entity.h"
#include <stddef.h>

#define INITIAL_TEMPERATURE 23

int next_entity_id = 0;

EntityRegistry entity_registry = {.temperature_sensor_count = 0, .door_count = 0, .alarm_count = 0};

TemperatureSensor *temperature_sensor_create()
{
    if (entity_registry.temperature_sensor_count < MAX_TEMPERATURE_SENSORS)
    {
        Entity entity;
        entity.id = next_entity_id;
        entity.type = TEMPERATURE_SENSOR;
        entity.id_in_entity_registry_type = entity_registry.temperature_sensor_count;

        TemperatureSensor temperature_sensor;
        temperature_sensor.entity = entity;

        temperature_sensor.current_temperature = INITIAL_TEMPERATURE;
        temperature_sensor.min_temperature = INITIAL_TEMPERATURE;
        temperature_sensor.max_temperature = INITIAL_TEMPERATURE;

        entity_registry.temperature_sensors[entity_registry.temperature_sensor_count] = temperature_sensor;
        entity_registry.temperature_sensor_count++;
        next_entity_id++;

        return &entity_registry.temperature_sensors[entity_registry.temperature_sensor_count - 1];
    }
    else
        return NULL;
}

Event generate_temperature_reading(Timestamp *timestamp, TemperatureSensor *sensor)
{
    Event event = event_create(*timestamp, TEMPERATURE_READING, sensor->entity);

    event.temperature = sensor->current_temperature;

    return event;
}

void temperature_update(TemperatureSensor *temperature_sensor, float temperature)
{
    temperature_sensor->current_temperature = temperature;

    if (temperature > temperature_sensor->max_temperature)
    {
        temperature_sensor->max_temperature = temperature;
    }
    else if (temperature < temperature_sensor->min_temperature)
    {
        temperature_sensor->min_temperature = temperature;
    }
}

void temperature_update_all(float temperature)
{
    for (int i = 0; i < entity_registry.temperature_sensor_count; i++)
    {
        temperature_update(&entity_registry.temperature_sensors[i], temperature);
    }
}

Door *door_create()
{
    if (entity_registry.door_count < MAX_DOORS)
    {
        Entity entity;
        entity.id = next_entity_id;
        entity.id_in_entity_registry_type = entity_registry.door_count;
        entity.type = DOOR;

        Door door;

        door.entity = entity;
        door.state = CLOSED;

        entity_registry.doors[entity_registry.door_count] = door;
        entity_registry.door_count++;
        next_entity_id++;

        return &entity_registry.doors[entity_registry.door_count - 1];
    }
    else
        return NULL;
}

Event door_open(Door *door, Timestamp *timestamp)
{
    if (door->state == OPEN)
        return event_create_invalid();

    door->state = OPEN;

    return event_create(*timestamp, DOOR_OPENED, door->entity);
}

Event door_close(Door *door, Timestamp *timestamp)
{
    if (door->state == CLOSED)
        return event_create_invalid();

    door->state = CLOSED;

    return event_create(*timestamp, DOOR_CLOSED, door->entity);
}

Alarm *alarm_create(int door_id)
{
    if (entity_registry.alarm_count >= MAX_ALARMS)
        return NULL;

    bool door_exists = false;

    for (int i = 0; i < entity_registry.door_count; i++)
    {
        if (entity_registry.doors[i].entity.id == door_id)
        {
            door_exists = true;
            break;
        }
    }

    if (!door_exists)
        return NULL;

    Entity entity;
    entity.id = next_entity_id;
    entity.type = ALARM;
    entity.id_in_entity_registry_type = entity_registry.alarm_count;

    Alarm alarm;
    alarm.entity = entity;
    alarm.door_id = door_id;
    alarm.state = INACTIVE;

    entity_registry.alarms[entity_registry.alarm_count] = alarm;
    entity_registry.alarm_count++;
    next_entity_id++;

    return &entity_registry.alarms[entity_registry.alarm_count - 1];
}

Event alarm_trigger(Alarm *alarm, Timestamp *timestamp)
{
    if (alarm->state == ACTIVE)
        return event_create_invalid();

    alarm->state = ACTIVE;

    return event_create(*timestamp, ALARM_TRIGGERED, alarm->entity);
}

Event alarm_clear(Alarm *alarm, Timestamp *timestamp)
{
    if (alarm->state == INACTIVE)
        return event_create_invalid();

    alarm->state = INACTIVE;

    return event_create(*timestamp, ALARM_CLEARED, alarm->entity);
}