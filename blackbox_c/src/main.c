// AI generated tests

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"
#include "event.h"
#include "queue.h"
#include "scenario.h"
#include "timestamp.h"

static void print_separator(const char *title)
{
    printf("\n");
    printf("============================================================\n");
    printf("%s\n", title);
    printf("============================================================\n");
}

int main(void)
{
    /*
     * Use a fixed seed while testing so that the scenario is
     * reproducible.
     *
     * Once everything works, replace 23 with:
     *
     * srand((unsigned int)time(NULL));
     */
    srand(23);

    printf("BlackBox - Version 1\n");

    Timestamp clock = timestamp_initialize();

    /*
     * =========================================================
     * ENTITY CREATION
     * =========================================================
     */

    print_separator("ENTITY CREATION");

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
        printf("Entity creation failed.\n");
        return 1;
    }

    printf("Temperature Sensor 1: id=%d\n",
           sensor_1->entity.id);

    printf("Temperature Sensor 2: id=%d\n",
           sensor_2->entity.id);

    printf("Door 1: id=%d\n",
           door_1->entity.id);

    printf("Door 2: id=%d\n",
           door_2->entity.id);

    printf("Alarm 1: id=%d, attached to door=%d\n",
           alarm_1->entity.id,
           alarm_1->door_id);

    /*
     * =========================================================
     * TEMPERATURE TEST
     * =========================================================
     */

    print_separator("TEMPERATURE TEST");

    Event event;

    printf("Initial temperature:\n");
    printf("Sensor 1: %.1f C\n",
           sensor_1->current_temperature);

    printf("\nUpdating all sensors to 25.5 C...\n");

    temperature_update_all(25.5f);

    printf("Sensor 1: %.1f C\n",
           sensor_1->current_temperature);

    printf("Sensor 2: %.1f C\n",
           sensor_2->current_temperature);

    printf("\nUpdating Sensor 1 to 20.0 C...\n");

    temperature_update(sensor_1, 20.0f);

    printf("Current: %.1f C\n",
           sensor_1->current_temperature);

    printf("Minimum: %.1f C\n",
           sensor_1->min_temperature);

    printf("Maximum: %.1f C\n",
           sensor_1->max_temperature);

    timestamp_advance(&clock);

    event = generate_temperature_reading(&clock, sensor_1);

    printf("\nGenerated temperature event:\n");
    event_print(&event);

    /*
     * =========================================================
     * DOOR TEST
     * =========================================================
     */

    print_separator("DOOR TEST");

    timestamp_advance(&clock);

    printf("Opening Door 1:\n");

    event = door_open(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Trying to open Door 1 again:\n");

    event = door_open(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Closing Door 1:\n");

    event = door_close(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Trying to close Door 1 again:\n");

    event = door_close(door_1, &clock);
    event_print(&event);

    /*
     * =========================================================
     * ALARM TEST
     * =========================================================
     */

    print_separator("ALARM TEST");

    timestamp_advance(&clock);

    printf("Triggering Alarm 1:\n");

    event = alarm_trigger(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Trying to trigger Alarm 1 again:\n");

    event = alarm_trigger(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Clearing Alarm 1:\n");

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    printf("Trying to clear Alarm 1 again:\n");

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    /*
     * =========================================================
     * QUEUE TEST
     * =========================================================
     */

    print_separator("QUEUE TEST");

    Queue queue = queue_create();

    printf("Queue initially empty: %s\n",
           queue_is_empty(&queue) ? "yes" : "no");

    printf("Queue initially full: %s\n",
           queue_is_full(&queue) ? "yes" : "no");

    timestamp_advance(&clock);

    queue_push(
        &queue,
        generate_temperature_reading(&clock, sensor_1));

    timestamp_advance(&clock);

    queue_push(
        &queue,
        door_open(door_2, &clock));

    timestamp_advance(&clock);

    queue_push(
        &queue,
        alarm_trigger(alarm_1, &clock));

    timestamp_advance(&clock);

    queue_push(
        &queue,
        door_close(door_2, &clock));

    timestamp_advance(&clock);

    queue_push(
        &queue,
        alarm_clear(alarm_1, &clock));

    printf("\nQueue size: %d\n",
           queue_get_size(&queue));

    printf("\nQueue contents:\n");
    queue_print(&queue);

    /*
     * =========================================================
     * QUEUE PEEK
     * =========================================================
     */

    printf("\nPeeking at queue:\n");

    Event peeked_event;

    if (queue_peek(&queue, &peeked_event))
    {
        printf("Front event: ");
        event_print(&peeked_event);
    }

    printf("Queue size after peek: %d\n",
           queue_get_size(&queue));

    /*
     * =========================================================
     * QUEUE POP
     * =========================================================
     */

    printf("\nPopping queue:\n");

    Event popped_event;

    while (queue_pop(&queue, &popped_event))
    {
        printf("Popped: ");
        event_print(&popped_event);
    }

    printf("Queue empty after popping: %s\n",
           queue_is_empty(&queue) ? "yes" : "no");

    /*
     * =========================================================
     * INVALID ENTITY CREATION
     * =========================================================
     */

    print_separator("INVALID ENTITY CREATION");

    TemperatureSensor *sensors[MAX_TEMPERATURE_SENSORS + 1];

    for (int i = 0; i < MAX_TEMPERATURE_SENSORS + 1; i++)
    {
        sensors[i] = temperature_sensor_create();

        if (sensors[i] == NULL)
        {
            printf("Temperature sensor creation rejected at index %d.\n", i);
        }
    }

    /*
     * =========================================================
     * INVALID ALARM CREATION
     * =========================================================
     */

    print_separator("INVALID ALARM CREATION");

    Alarm *invalid_alarm = alarm_create(999999);

    if (invalid_alarm == NULL)
    {
        printf("Invalid alarm creation correctly rejected.\n");
    }
    else
    {
        printf("ERROR: Invalid alarm was created.\n");
    }

    /*
     * =========================================================
     * SCENARIO TEST
     * =========================================================
     */

    print_separator("SCENARIO SIMULATION");

    /*
     * Use a separate queue for the scenario.
     */
    Queue scenario_queue = queue_create();

    /*
     * Start a fresh simulated clock.
     */
    Timestamp scenario_clock = timestamp_initialize();

    printf("Starting scenario simulation...\n");
    printf("Simulation length: 120 seconds\n\n");

    for (int second = 0; second < 120; second++)
    {
        scenario_generate(&scenario_queue, &scenario_clock);

        /*
         * Print and remove all events generated during
         * this simulated second.
         */
        while (queue_pop(&scenario_queue, &popped_event))
        {
            event_print(&popped_event);
        }

        timestamp_advance(&scenario_clock);
    }

    /*
     * =========================================================
     * FINAL STATE
     * =========================================================
     */

    print_separator("FINAL ENTITY STATE");

    printf("Temperature Sensor 1:\n");
    printf("  Current: %.2f C\n",
           sensor_1->current_temperature);
    printf("  Minimum: %.2f C\n",
           sensor_1->min_temperature);
    printf("  Maximum: %.2f C\n",
           sensor_1->max_temperature);

    printf("\nTemperature Sensor 2:\n");
    printf("  Current: %.2f C\n",
           sensor_2->current_temperature);
    printf("  Minimum: %.2f C\n",
           sensor_2->min_temperature);
    printf("  Maximum: %.2f C\n",
           sensor_2->max_temperature);

    printf("\nDoor 1:\n");
    printf("  State: %s\n",
           door_1->state == OPEN ? "OPEN" : "CLOSED");

    printf("\nDoor 2:\n");
    printf("  State: %s\n",
           door_2->state == OPEN ? "OPEN" : "CLOSED");

    printf("\nAlarm 1:\n");
    printf("  State: %s\n",
           alarm_1->state == ACTIVE ? "ACTIVE" : "INACTIVE");

    /*
     * =========================================================
     * COMPLETE
     * =========================================================
     */

    printf("\n");
    printf("============================================================\n");
    printf("BlackBox tests complete.\n");
    printf("============================================================\n");

    return 0;
}