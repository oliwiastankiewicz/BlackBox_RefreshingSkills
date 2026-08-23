// AI generated tests

#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "event.h"
#include "queue.h"
#include "scenario.h"
#include "timestamp.h"
#include "simulation.h"

static void separator(const char *title)
{
    printf("\n--- %s ---\n", title);
}

int main(void)
{
    srand(23);

    printf("BlackBox - Version 1\n");

    Timestamp clock = timestamp_initialize();

    /*
     * =========================================================
     * ENTITY CREATION
     * =========================================================
     */

    separator("ENTITY CREATION");

    TemperatureSensor *sensor_1 = temperature_sensor_create();
    TemperatureSensor *sensor_2 = temperature_sensor_create();

    Door *door_1 = door_create();
    Door *door_2 = door_create();

    Alarm *alarm_1 = alarm_create(door_1->entity.id);

    if (sensor_1 == NULL ||
        sensor_2 == NULL ||
        door_1 == NULL ||
        door_2 == NULL ||
        alarm_1 == NULL)
    {
        printf("FAIL: entity creation\n");
        return 1;
    }

    printf("Sensors: %d, %d\n",
           sensor_1->entity.id,
           sensor_2->entity.id);

    printf("Doors: %d, %d\n",
           door_1->entity.id,
           door_2->entity.id);

    printf("Alarm: %d -> Door %d\n",
           alarm_1->entity.id,
           alarm_1->door_id);

    /*
     * =========================================================
     * TEMPERATURE
     * =========================================================
     */

    separator("TEMPERATURE");

    temperature_update_all(25.5f);
    temperature_update(sensor_1, 20.0f);

    printf("Sensor 1: current=%.1f min=%.1f max=%.1f\n",
           sensor_1->current_temperature,
           sensor_1->min_temperature,
           sensor_1->max_temperature);

    timestamp_advance(&clock);

    Event event = generate_temperature_reading(&clock, sensor_1);
    event_print(&event);

    /*
     * =========================================================
     * DOOR + ALARM
     * =========================================================
     */

    separator("DOOR + ALARM");

    timestamp_advance(&clock);

    event = door_open(door_1, &clock);
    event_print(&event);

    event = alarm_trigger(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = door_close(door_1, &clock);
    event_print(&event);

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    /*
     * Test invalid repeated operations.
     */

    timestamp_advance(&clock);

    event = door_close(door_1, &clock);
    event_print(&event);

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    /*
     * =========================================================
     * QUEUE
     * =========================================================
     */

    separator("QUEUE");

    Queue queue = queue_create();

    timestamp_advance(&clock);
    queue_push(&queue,
               generate_temperature_reading(&clock, sensor_1));

    timestamp_advance(&clock);
    queue_push(&queue,
               door_open(door_2, &clock));

    printf("Size after push: %d\n",
           queue_get_size(&queue));

    Event peeked_event;

    if (queue_peek(&queue, &peeked_event))
    {
        printf("Peek: ");
        event_print(&peeked_event);
    }

    Event popped_event;

    while (queue_pop(&queue, &popped_event))
    {
        printf("Pop:  ");
        event_print(&popped_event);
    }

    printf("Empty: %s\n",
           queue_is_empty(&queue) ? "yes" : "no");

    /*
     * =========================================================
     * SCENARIO
     * =========================================================
     */

    separator("SCENARIO");

    Queue scenario_queue = queue_create();
    Timestamp scenario_clock = timestamp_initialize();

    int scenario_events = 0;

    for (int second = 0; second < 60; second++)
    {
        scenario_generate(&scenario_queue, &scenario_clock);

        while (queue_pop(&scenario_queue, &popped_event))
        {
            scenario_events++;
            event_print(&popped_event);
        }

        timestamp_advance(&scenario_clock);
    }

    printf("Events generated: %d\n", scenario_events);

    /*
     * =========================================================
     * SIMULATION
     * =========================================================
     */

    separator("SIMULATION");

    printf("Running 30-second simulation...\n");

    Timestamp duration = timestamp_create(0, 0, 30);

    simulation_run(
        2,
        1,
        1,
        duration);

    printf("Output: SIMULATION_BlackBox.txt\n");

    /*
     * =========================================================
     * INVALID INPUTS
     * =========================================================
     */

    separator("INVALID INPUT");

    printf("Too many sensors:\n");

    simulation_run(
        MAX_TEMPERATURE_SENSORS + 1,
        0,
        0,
        timestamp_create(0, 0, 10));

    printf("Too many doors:\n");

    simulation_run(
        0,
        3,
        3,
        timestamp_create(0, 0, 10));

    printf("Invalid duration:\n");

    simulation_run(
        1,
        0,
        0,
        timestamp_create(2, 0, 0));

    /*
     * =========================================================
     * INVALID ENTITY CREATION
     * =========================================================
     */

    separator("INVALID ENTITY");

    TemperatureSensor *invalid_sensor = NULL;

    /*
     * Fill the remaining sensor slots.
     */
    while (entity_registry.temperature_sensor_count <
           MAX_TEMPERATURE_SENSORS)
    {
        temperature_sensor_create();
    }

    invalid_sensor = temperature_sensor_create();

    printf("Extra sensor: %s\n",
           invalid_sensor == NULL ? "rejected" : "ERROR");

    Alarm *invalid_alarm = alarm_create(999999);

    printf("Invalid alarm: %s\n",
           invalid_alarm == NULL ? "rejected" : "ERROR");

    /*
     * =========================================================
     * FINAL
     * =========================================================
     */

    separator("FINAL STATE");

    printf("Sensors: %d/%d\n",
           entity_registry.temperature_sensor_count,
           MAX_TEMPERATURE_SENSORS);

    printf("Doors:   %d/%d\n",
           entity_registry.door_count,
           MAX_DOORS);

    printf("Alarms:  %d/%d\n",
           entity_registry.alarm_count,
           MAX_ALARMS);

    printf("\nTests complete.\n");

    return 0;
}