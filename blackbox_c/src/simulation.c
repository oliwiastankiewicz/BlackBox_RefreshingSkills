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

static bool entities_create(EntityRegistry *entity_registry, int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms)
{
    for (int i = 0; i < nr_temperature_sensors; i++)
    {
        if (temperature_sensor_create(entity_registry) == NULL)
        {
            return false;
        }
    }

    for (int i = 0; i < nr_doors_with_alarms; i++)
    {
        if (door_create(entity_registry) == NULL)
        {
            return false;
        }
        if (alarm_create(entity_registry, entity_registry->doors[i].entity.id) == NULL)
        {
            return false;
        }
    }

    for (int i = 0; i < nr_doors_without_alarms; i++)
    {
        if (door_create(entity_registry) == NULL)
        {
            return false;
        }
    }
    return true;
}

static void simulation_write_file_header(FILE *file, EntityRegistry *entity_registry)
{
    fprintf(file, "BlackBox - Version 1\n\n");

    fprintf(file, "--- ENTITIES ---\n");

    fprintf(file, "Temperature sensors: %d\n", entity_registry->temperature_sensor_count);
    fprintf(file, "Doors: %d\n", entity_registry->door_count);
    fprintf(file, "Alarms: %d\n\n", entity_registry->alarm_count);

    fprintf(file, "Sensors:\n");
    for (int i = 0; i < entity_registry->temperature_sensor_count; i++)
    {
        fprintf(file, "  Sensor %d: entity %d\n", i, entity_registry->temperature_sensors[i].entity.id);
    }

    fprintf(file, "\nDoors:\n");
    for (int i = 0; i < entity_registry->door_count; i++)
    {
        fprintf(file, "  Door %d: entity %d", i, entity_registry->doors[i].entity.id);

        Alarm *alarm = NULL;

        for (int j = 0; j < entity_registry->alarm_count; j++)
        {
            if (entity_registry->alarms[j].door_id == entity_registry->doors[i].entity.id)
            {
                alarm = &entity_registry->alarms[j];
                break;
            }
        }

        if (alarm != NULL)
        {
            fprintf(file, " -> Alarm %d: entity %d", i, alarm->entity.id);
        }
        else
        {
            fprintf(file, " -> no alarm");
        }

        fprintf(file, "\n");
    }

    fprintf(file, "\n--- EVENTS ---\n");
}

void simulation_run(const char *filename, int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms, Timestamp simulation_duration)
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

    EntityRegistry entity_registry = entity_registry_initialize();
    if (!entities_create(&entity_registry, nr_temperature_sensors, nr_doors_with_alarms, nr_doors_without_alarms))
    {
        printf("Failed to create entities!");
        return;
    }

    Timestamp clock = timestamp_initialize();
    Queue queue = queue_create();

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error: Could not open %s!\n", filename);
        return;
    }

    simulation_write_file_header(file, &entity_registry);

    while (timestamp_compare(&clock, &simulation_duration) == -1)
    {
        scenario_generate(&entity_registry, &queue, &clock);

        Event popped_event;
        while (queue_pop(&queue, &popped_event))
        {
            event_print(&popped_event);
            event_write_to_file(&popped_event, file);
        }

        timestamp_advance(&clock);
    }

    fclose(file);
    printf("Simulation complete.\n");
    printf("Output: %s\n", filename);
}