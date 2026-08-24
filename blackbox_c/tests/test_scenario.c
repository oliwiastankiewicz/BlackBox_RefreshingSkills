#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "scenario.h"
#include "entity.h"
#include "queue.h"
#include "timestamp.h"

static void test_temperature_readings(void)
{
    EntityRegistry registry = entity_registry_initialize();

    temperature_sensor_create(&registry);
    temperature_sensor_create(&registry);

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_initialize();

    /*
     * At 00:00:00 a temperature reading should be generated
     * for every temperature sensor.
     */
    srand(1);

    scenario_generate(&registry, &queue, &timestamp);

    assert(queue_get_size(&queue) >= 2);

    int temperature_events = 0;
    Event event;

    while (queue_pop(&queue, &event))
    {
        if (event.type == TEMPERATURE_READING)
        {
            temperature_events++;

            assert(event.entity.type == TEMPERATURE_SENSOR);
            assert(event.temperature >= -1000.0f);
            assert(event.temperature <= 1000.0f);
        }
    }

    assert(temperature_events == 2);
}

static void test_door_alarm_relationship(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);
    Alarm *alarm = alarm_create(&registry, door->entity.id);

    assert(door != NULL);
    assert(alarm != NULL);
    assert(alarm->door_id == door->entity.id);

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_initialize();

    /*
     * Run the scenario several times.
     * Whenever the door opens, its alarm should trigger.
     * Whenever the door closes, its alarm should clear.
     */
    srand(2);

    for (int i = 0; i < 20; i++)
    {
        scenario_generate(&registry, &queue, &timestamp);

        Event event;

        while (queue_pop(&queue, &event))
        {
            if (event.type == DOOR_OPENED)
            {
                assert(event.entity.id == door->entity.id);
                assert(door->state == OPEN);
            }

            if (event.type == ALARM_TRIGGERED)
            {
                assert(event.entity.id == alarm->entity.id);
                assert(alarm->state == ACTIVE);
                assert(alarm->door_id == door->entity.id);
            }

            if (event.type == DOOR_CLOSED)
            {
                assert(event.entity.id == door->entity.id);
                assert(door->state == CLOSED);
            }

            if (event.type == ALARM_CLEARED)
            {
                assert(event.entity.id == alarm->entity.id);
                assert(alarm->state == INACTIVE);
            }
        }

        timestamp_advance(&timestamp);
    }
}

static void test_door_without_alarm(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    assert(door != NULL);

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_initialize();

    srand(3);

    for (int i = 0; i < 20; i++)
    {
        scenario_generate(&registry, &queue, &timestamp);

        Event event;

        while (queue_pop(&queue, &event))
        {
            /*
             * This door has no alarm, so no alarm event
             * should ever be generated.
             */
            assert(event.type != ALARM_TRIGGERED);
            assert(event.type != ALARM_CLEARED);

            if (event.type == DOOR_OPENED ||
                event.type == DOOR_CLOSED)
            {
                assert(event.entity.id == door->entity.id);
            }
        }

        timestamp_advance(&timestamp);
    }
}

static void test_temperature_changes(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    assert(sensor != NULL);

    float initial_temperature = sensor->current_temperature;

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_initialize();

    srand(4);

    for (int i = 0; i < 20; i++)
    {
        scenario_generate(&registry, &queue, &timestamp);

        /*
         * The scenario changes temperature by a random amount
         * between -2 and +2.
         */
        assert(sensor->current_temperature >=
               initial_temperature - 40.0f);

        assert(sensor->current_temperature <=
               initial_temperature + 40.0f);

        while (queue_pop(&queue, &(Event){0}))
        {
        }

        timestamp_advance(&timestamp);
    }

    /*
     * Min/max should remain consistent with the current value.
     */
    assert(sensor->min_temperature <=
               sensor->current_temperature ||
           sensor->max_temperature >=
               sensor->current_temperature);

    assert(sensor->min_temperature <=
           sensor->max_temperature);
}

static void test_queue_is_empty_after_processing(void)
{
    EntityRegistry registry = entity_registry_initialize();

    temperature_sensor_create(&registry);
    door_create(&registry);

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_initialize();

    srand(5);

    for (int i = 0; i < 10; i++)
    {
        scenario_generate(&registry, &queue, &timestamp);

        Event event;

        while (queue_pop(&queue, &event))
        {
        }

        assert(queue_is_empty(&queue));

        timestamp_advance(&timestamp);
    }
}

static void test_temperature_reading_interval(void)
{
    EntityRegistry registry = entity_registry_initialize();

    temperature_sensor_create(&registry);

    Queue queue = queue_create();
    Timestamp timestamp = timestamp_create(0, 0, 29);

    srand(6);

    scenario_generate(&registry, &queue, &timestamp);

    Event event;

    while (queue_pop(&queue, &event))
    {
        assert(event.type != TEMPERATURE_READING);
    }

    timestamp_advance(&timestamp);

    assert(timestamp.seconds == 30);

    scenario_generate(&registry, &queue, &timestamp);

    int readings = 0;

    while (queue_pop(&queue, &event))
    {
        if (event.type == TEMPERATURE_READING)
        {
            readings++;
        }
    }

    assert(readings == 1);
}

int main(void)
{
    printf("Running scenario tests...\n");

    test_temperature_readings();
    printf("  [PASS] temperature readings\n");

    test_door_alarm_relationship();
    printf("  [PASS] door/alarm relationship\n");

    test_door_without_alarm();
    printf("  [PASS] door without alarm\n");

    test_temperature_changes();
    printf("  [PASS] temperature changes\n");

    test_queue_is_empty_after_processing();
    printf("  [PASS] queue processing\n");

    test_temperature_reading_interval();
    printf("  [PASS] temperature reading interval\n");

    printf("All scenario tests passed.\n");

    return 0;
}