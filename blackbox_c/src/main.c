// AI generated tests

#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "event.h"
#include "queue.h"
#include "scenario.h"
#include "simulation.h"
#include "timestamp.h"

static void separator(const char *title)
{
    printf("\n--- %s ---\n", title);
}

int main(void)
{
    srand(23);

    printf("BlackBox - Version 1\n");

    /*
     * ---------------------------------------------------------
     * Registry 1
     * ---------------------------------------------------------
     */

    separator("REGISTRY 1");

    EntityRegistry registry_1 = entity_registry_initialize();

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry_1);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry_1);

    Door *door_1 =
        door_create(&registry_1);

    Alarm *alarm_1 =
        alarm_create(&registry_1, door_1->entity.id);

    printf("Sensors: %d, %d\n",
           sensor_1->entity.id,
           sensor_2->entity.id);

    printf("Door: %d\n",
           door_1->entity.id);

    printf("Alarm: %d -> Door %d\n",
           alarm_1->entity.id,
           alarm_1->door_id);

    printf("Counts: S=%d D=%d A=%d\n",
           registry_1.temperature_sensor_count,
           registry_1.door_count,
           registry_1.alarm_count);

    /*
     * ---------------------------------------------------------
     * Entity behaviour
     * ---------------------------------------------------------
     */

    separator("ENTITY EVENTS");

    Timestamp clock = timestamp_initialize();

    Event event;

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
     * Invalid state transitions.
     */

    event = door_close(door_1, &clock);
    event_print(&event);

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    /*
     * ---------------------------------------------------------
     * Temperature
     * ---------------------------------------------------------
     */

    separator("TEMPERATURE");

    temperature_update(&registry_1.temperature_sensors[0], 20.0f);
    temperature_update(&registry_1.temperature_sensors[0], 30.0f);

    printf("Sensor 0: current=%.1f min=%.1f max=%.1f\n",
           registry_1.temperature_sensors[0].current_temperature,
           registry_1.temperature_sensors[0].min_temperature,
           registry_1.temperature_sensors[0].max_temperature);

    temperature_update_all(&registry_1, 25.0f);

    printf("After update_all: %.1f, %.1f\n",
           registry_1.temperature_sensors[0].current_temperature,
           registry_1.temperature_sensors[1].current_temperature);

    /*
     * ---------------------------------------------------------
     * Queue
     * ---------------------------------------------------------
     */

    separator("QUEUE");

    Queue queue = queue_create();

    timestamp_advance(&clock);

    queue_push(
        &queue,
        generate_temperature_reading(
            &clock,
            &registry_1.temperature_sensors[0]));

    timestamp_advance(&clock);

    queue_push(
        &queue,
        door_open(
            door_1,
            &clock));

    printf("Size: %d\n", queue_get_size(&queue));

    Event queue_event;

    if (queue_peek(&queue, &queue_event))
    {
        printf("Peek: ");
        event_print(&queue_event);
    }

    while (queue_pop(&queue, &queue_event))
    {
        printf("Pop:  ");
        event_print(&queue_event);
    }

    printf("Empty: %s\n",
           queue_is_empty(&queue) ? "yes" : "no");

    /*
     * ---------------------------------------------------------
     * Independent registry
     * ---------------------------------------------------------
     */

    separator("REGISTRY INDEPENDENCE");

    EntityRegistry registry_2 = entity_registry_initialize();

    TemperatureSensor *sensor_3 =
        temperature_sensor_create(&registry_2);

    Door *door_2 =
        door_create(&registry_2);

    Alarm *alarm_2 =
        alarm_create(&registry_2, door_2->entity.id);

    printf("Registry 1 next ID: %d\n",
           registry_1.next_entity_id);

    printf("Registry 2 next ID: %d\n",
           registry_2.next_entity_id);

    printf("Registry 2 first sensor ID: %d\n",
           sensor_3->entity.id);

    printf("Registry 2 door ID: %d\n",
           door_2->entity.id);

    printf("Registry 2 alarm ID: %d\n",
           alarm_2->entity.id);

    /*
     * This should prove that registry_2 started from ID 0
     * even though registry_1 already contains entities.
     */

    /*
     * ---------------------------------------------------------
     * Registry reset
     * ---------------------------------------------------------
     */

    separator("REGISTRY RESET");

    entity_registry_reset(&registry_2);

    printf("After reset: S=%d D=%d A=%d next=%d\n",
           registry_2.temperature_sensor_count,
           registry_2.door_count,
           registry_2.alarm_count,
           registry_2.next_entity_id);

    sensor_3 = temperature_sensor_create(&registry_2);

    printf("New sensor after reset: id=%d\n",
           sensor_3->entity.id);

    /*
     * ---------------------------------------------------------
     * Scenario
     * ---------------------------------------------------------
     */

    separator("SCENARIO");

    Queue scenario_queue = queue_create();
    Timestamp scenario_clock = timestamp_initialize();

    for (int i = 0; i < 5; i++)
    {
        scenario_generate(
            &registry_1,
            &scenario_queue,
            &scenario_clock);

        while (queue_pop(&scenario_queue, &queue_event))
        {
            event_print(&queue_event);
        }

        timestamp_advance(&scenario_clock);
    }

    /*
     * ---------------------------------------------------------
     * Simulation
     * ---------------------------------------------------------
     */

    separator("SIMULATION");

    Timestamp duration = timestamp_create(0, 0, 5);

    printf("Running 5-second simulation...\n");

    simulation_run(
        2,
        1,
        1,
        duration);

    printf("Simulation complete.\n");
    printf("Output: SIMULATION_BlackBox.txt\n");

    /*
     * ---------------------------------------------------------
     * Invalid input
     * ---------------------------------------------------------
     */

    separator("INVALID INPUT");

    printf("Too many sensors: ");
    simulation_run(
        MAX_TEMPERATURE_SENSORS + 1,
        0,
        0,
        duration);

    printf("\nToo many doors: ");
    simulation_run(
        0,
        3,
        3,
        duration);

    printf("\nInvalid duration: ");
    simulation_run(
        1,
        1,
        0,
        timestamp_create(2, 0, 0));

    /*
     * ---------------------------------------------------------
     * Invalid entity creation
     * ---------------------------------------------------------
     */

    separator("INVALID ENTITY");

    EntityRegistry test_registry =
        entity_registry_initialize();

    /*
     * Invalid alarm: no such door exists.
     */

    Alarm *invalid_alarm =
        alarm_create(&test_registry, 9999);

    printf("Invalid alarm: %s\n",
           invalid_alarm == NULL ? "rejected" : "ERROR");

    /*
     * Fill sensors to capacity.
     */

    for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++)
    {
        temperature_sensor_create(&test_registry);
    }

    TemperatureSensor *extra_sensor =
        temperature_sensor_create(&test_registry);

    printf("Extra sensor: %s\n",
           extra_sensor == NULL ? "rejected" : "ERROR");

    /*
     * ---------------------------------------------------------
     * Complete
     * ---------------------------------------------------------
     */

    printf("\nTests complete.\n");

    return 0;
}