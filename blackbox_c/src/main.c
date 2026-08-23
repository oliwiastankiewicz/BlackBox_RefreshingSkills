#include <stdio.h>

#include "entity.h"
#include "event.h"
#include "queue.h"
#include "timestamp.h"

int main(void)
{
    printf("BlackBox - Version 1\n\n");

    Timestamp clock = timestamp_initialize();

    /*
     * ---------------------------------------------------------
     * Create entities
     * ---------------------------------------------------------
     */

    TemperatureSensor *sensor_1 = temperature_sensor_create();
    TemperatureSensor *sensor_2 = temperature_sensor_create();

    Door *door_1 = door_create();
    Door *door_2 = door_create();

    Alarm *alarm_1 = alarm_create(door_1->entity.id);

    printf("Created entities:\n");

    printf("Temperature Sensor 1: id=%d\n",
           sensor_1->entity.id);

    printf("Temperature Sensor 2: id=%d\n",
           sensor_2->entity.id);

    printf("Door 1: id=%d\n",
           door_1->entity.id);

    printf("Door 2: id=%d\n",
           door_2->entity.id);

    printf("Alarm 1: id=%d, attached to door=%d\n\n",
           alarm_1->entity.id,
           alarm_1->door_id);

    /*
     * ---------------------------------------------------------
     * Temperature
     * ---------------------------------------------------------
     */

    printf("Temperature test:\n");

    Event event;

    temperature_update_all(25.5f);

    timestamp_advance(&clock);

    event = generate_temperature_reading(&clock, sensor_1);
    event_print(&event);

    temperature_update(sensor_1, 20.0f);

    timestamp_advance(&clock);

    event = generate_temperature_reading(&clock, sensor_1);
    event_print(&event);

    printf("Sensor 1 current: %.1f\n", sensor_1->current_temperature);
    printf("Sensor 1 minimum: %.1f\n", sensor_1->min_temperature);
    printf("Sensor 1 maximum: %.1f\n\n", sensor_1->max_temperature);

    /*
     * ---------------------------------------------------------
     * Door
     * ---------------------------------------------------------
     */

    printf("Door test:\n");

    timestamp_advance(&clock);

    event = door_open(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = door_open(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = door_close(door_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = door_close(door_1, &clock);
    event_print(&event);

    printf("\n");

    /*
     * ---------------------------------------------------------
     * Alarm
     * ---------------------------------------------------------
     */

    printf("Alarm test:\n");

    timestamp_advance(&clock);

    event = alarm_trigger(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = alarm_trigger(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    timestamp_advance(&clock);

    event = alarm_clear(alarm_1, &clock);
    event_print(&event);

    printf("\n");

    /*
     * ---------------------------------------------------------
     * Queue
     * ---------------------------------------------------------
     */

    printf("Queue test:\n");

    Queue queue = queue_create();

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

    queue_print(&queue);

    printf("\n");

    /*
     * ---------------------------------------------------------
     * Pop queue
     * ---------------------------------------------------------
     */

    printf("Popping queue:\n");

    Event popped_event;

    while (queue_pop(&queue, &popped_event))
    {
        printf("Popped: ");
        event_print(&popped_event);
    }

    printf("\n");

    /*
     * ---------------------------------------------------------
     * Invalid entity creation
     * ---------------------------------------------------------
     */

    printf("Invalid entity test:\n");

    TemperatureSensor *sensors[MAX_TEMPERATURE_SENSORS + 1];

    for (int i = 0; i < MAX_TEMPERATURE_SENSORS + 1; i++)
    {
        sensors[i] = temperature_sensor_create();

        if (sensors[i] == NULL)
        {
            printf("Temperature sensor creation failed at index %d\n", i);
        }
    }

    printf("\n");

    /*
     * ---------------------------------------------------------
     * Invalid alarm creation
     * ---------------------------------------------------------
     */

    Alarm *invalid_alarm = alarm_create(999999);

    if (invalid_alarm == NULL)
    {
        printf("Invalid alarm creation correctly rejected.\n");
    }

    printf("\nBlackBox tests complete.\n");

    return 0;
}