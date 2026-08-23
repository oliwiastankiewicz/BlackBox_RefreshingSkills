#include "../include/simulation.h"
#include <stdio.h>

static bool simulation_input_validate(int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms)
{
    if (nr_temperature_sensors >= 0 &&
        nr_temperature_sensors <= MAX_TEMPERATURE_SENSORS &&
        nr_doors_with_alarms >= 0 &&
        nr_doors_with_alarms <= MAX_ALARMS &&
        nr_doors_without_alarms >= 0 &&
        nr_doors_with_alarms + nr_doors_without_alarms <= MAX_DOORS)
    {
        return true;
    }

    return false;
}

static bool entities_create(int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms)
{
    for (int i = 0; i < nr_temperature_sensors; i++)
    {
        if (temperature_sensor_create() == NULL)
        {
            return false;
        }
    }

    for (int i = 0; i < nr_doors_with_alarms; i++)
    {
        if (door_create() == NULL)
        {
            return false;
        }
        if (alarm_create(entity_registry.doors[i].entity.id) == NULL)
        {
            return false;
        }
    }

    for (int i = 0; i < nr_doors_without_alarms; i++)
    {
        if (door_create() == NULL)
        {
            return false;
        }
    }
    return true;
}

void simulation_run(int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms, Timestamp simulation_duration)
{
    if (!timestamp_validate(simulation_duration.hours, simulation_duration.minutes, simulation_duration.seconds))
    {
        printf("Incorrect simulation duration!");
        return;
    }
    if (!simulation_input_validate(nr_temperature_sensors, nr_doors_with_alarms, nr_doors_without_alarms))
    {
        printf("Incorrect input!");
        return;
    }

    if (!entities_create(nr_temperature_sensors, nr_doors_with_alarms, nr_doors_without_alarms))
    {
        printf("Failed to create entities!");
        return;
    }

    Timestamp clock = timestamp_initialize();
    Queue queue = queue_create();

    FILE *file;

    file = fopen("SIMULATION_BlackBox.txt", "w");

    if (file == NULL)
    {
        printf("Error: File returned null!");
        return;
    }

    while (timestamp_compare(&clock, &simulation_duration) == -1)
    {
        scenario_generate(&queue, &clock);

        Event popped_event;
        while (queue_pop(&queue, &popped_event))
        {
            event_print(&popped_event);
            event_write_to_file(&popped_event, file);
        }

        timestamp_advance(&clock);
    }

    fclose(file);
}