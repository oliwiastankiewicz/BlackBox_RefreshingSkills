#include "../include/scenario.h"
#include <stdlib.h>

#define DOOR_OPEN_PROBABILITY 80
#define DOOR_CLOSE_PROBABILITY 70
#define TEMPERATURE_CHANGE_PROBABILITY 50
#define TEMPERATURE_READING_INTERVAL 30
#define MIN_TEMPERATURE_CHANGE -2
#define MAX_TEMPERATURE_CHANGE 2

static bool probability_check(int probability)
{
    return (rand() % 100) < probability;
}

static bool temperature_reading(Timestamp *timestamp)
{
    return timestamp->seconds % TEMPERATURE_READING_INTERVAL == 0;
}

static Alarm *alarm_attached(int door_id)
{
    for (int i = 0; i < entity_registry.alarm_count; i++)
    {
        if (entity_registry.alarms[i].door_id == door_id)
        {
            return &entity_registry.alarms[i];
        }
    }

    return NULL;
}

static float generate_new_temperature(float current_temperature)
{
    float change = ((float)rand() / (float)RAND_MAX) * 4.0f - 2.0f;

    return current_temperature + change;
}

void scenario_generate(Queue *queue, Timestamp *timestamp)
{

    for (int i = 0; i < entity_registry.temperature_sensor_count; i++)
    {
        if (probability_check(TEMPERATURE_CHANGE_PROBABILITY))
        {
            float new_temperature = generate_new_temperature(entity_registry.temperature_sensors[i].current_temperature);

            temperature_update(&entity_registry.temperature_sensors[i], new_temperature);
        }
    }

    if (temperature_reading(timestamp))
    {
        for (int i = 0; i < entity_registry.temperature_sensor_count; i++)
        {
            Event event = generate_temperature_reading(timestamp, &entity_registry.temperature_sensors[i]);

            if (event.type != INVALID)
            {
                queue_push(queue, event);
            }
        }
    }

    for (int i = 0; i < entity_registry.door_count; i++)
    {
        if (entity_registry.doors[i].state == CLOSED)
        {
            if (probability_check(DOOR_OPEN_PROBABILITY))
            {
                Event event = door_open(&entity_registry.doors[i], timestamp);

                if (event.type != INVALID)
                {
                    queue_push(queue, event);

                    Alarm *alarm = alarm_attached(entity_registry.doors[i].entity.id);

                    if (alarm != NULL)
                    {
                        Event trigger = alarm_trigger(alarm, timestamp);

                        if (trigger.type != INVALID)
                        {
                            queue_push(queue, trigger);
                        }
                    }
                }
            }
        }
        else
        {
            if (probability_check(DOOR_CLOSE_PROBABILITY))
            {
                Event event = door_close(&entity_registry.doors[i], timestamp);

                if (event.type != INVALID)
                {
                    queue_push(queue, event);

                    Alarm *alarm = alarm_attached(entity_registry.doors[i].entity.id);

                    if (alarm != NULL)
                    {
                        Event trigger = alarm_clear(alarm, timestamp);

                        if (trigger.type != INVALID)
                        {
                            queue_push(queue, trigger);
                        }
                    }
                }
            }
        }
    }
}
