// AI generated tests

#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "event.h"
#include "queue.h"
#include "scenario.h"
#include "simulation.h"
#include "timestamp.h"

static int tests_passed = 0;
static int tests_failed = 0;

static void separator(const char *title)
{
    printf("\n--- %s ---\n", title);
}

static void test_result(FILE *report, const char *test, int passed)
{
    if (passed)
    {
        printf("[PASS] %s\n", test);
        fprintf(report, "[PASS] %s\n", test);
        tests_passed++;
    }
    else
    {
        printf("[FAIL] %s\n", test);
        fprintf(report, "[FAIL] %s\n", test);
        tests_failed++;
    }
}

static void write_event(FILE *file, const Event *event)
{
    event_write_to_file(event, file);
}

static void test_timestamp(FILE *report)
{
    separator("TIMESTAMP");

    Timestamp time = timestamp_initialize();

    test_result(
        report,
        "Timestamp initializes to 00:00:00",
        time.hours == 0 &&
            time.minutes == 0 &&
            time.seconds == 0);

    time = timestamp_create(0, 0, 59);
    timestamp_advance(&time);

    test_result(
        report,
        "Timestamp advances seconds correctly",
        time.hours == 0 &&
            time.minutes == 1 &&
            time.seconds == 0);

    time = timestamp_create(0, 59, 59);
    timestamp_advance(&time);

    test_result(
        report,
        "Timestamp advances minutes correctly",
        time.hours == 1 &&
            time.minutes == 0 &&
            time.seconds == 0);

    test_result(
        report,
        "Valid timestamp accepted",
        timestamp_validate(1, 30, 45));

    test_result(
        report,
        "Invalid hours rejected",
        !timestamp_validate(2, 0, 0));

    test_result(
        report,
        "Invalid minutes rejected",
        !timestamp_validate(0, 60, 0));

    test_result(
        report,
        "Invalid seconds rejected",
        !timestamp_validate(0, 0, 60));

    Timestamp a = timestamp_create(0, 0, 10);
    Timestamp b = timestamp_create(0, 0, 20);

    test_result(
        report,
        "Timestamp comparison works",
        timestamp_compare(&a, &b) == -1 &&
            timestamp_compare(&b, &a) == 1 &&
            timestamp_compare(&a, &a) == 0);
}

static void test_registry(FILE *report)
{
    separator("REGISTRY");

    EntityRegistry registry = entity_registry_initialize();

    test_result(
        report,
        "Registry initializes empty",
        registry.temperature_sensor_count == 0 &&
            registry.door_count == 0 &&
            registry.alarm_count == 0 &&
            registry.next_entity_id == 0);

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry);

    Door *door =
        door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    test_result(
        report,
        "Temperature sensors are created",
        sensor_1 != NULL &&
            sensor_2 != NULL &&
            registry.temperature_sensor_count == 2);

    test_result(
        report,
        "Door is created",
        door != NULL &&
            registry.door_count == 1);

    test_result(
        report,
        "Alarm is created for existing door",
        alarm != NULL &&
            registry.alarm_count == 1);

    test_result(
        report,
        "Entity IDs are sequential",
        sensor_1->entity.id == 0 &&
            sensor_2->entity.id == 1 &&
            door->entity.id == 2 &&
            alarm->entity.id == 3);

    test_result(
        report,
        "Door lookup finds existing door",
        entity_registry_check_door_exists(
            &registry,
            door->entity.id));

    test_result(
        report,
        "Door lookup rejects nonexistent door",
        !entity_registry_check_door_exists(
            &registry,
            9999));

    /*
     * Independent registry.
     */

    EntityRegistry registry_2 =
        entity_registry_initialize();

    TemperatureSensor *sensor_3 =
        temperature_sensor_create(&registry_2);

    Door *door_2 =
        door_create(&registry_2);

    Alarm *alarm_2 =
        alarm_create(&registry_2, door_2->entity.id);

    test_result(
        report,
        "Registries are independent",
        sensor_3->entity.id == 0 &&
            door_2->entity.id == 1 &&
            alarm_2->entity.id == 2 &&
            registry.next_entity_id == 4 &&
            registry_2.next_entity_id == 3);

    /*
     * Registry reset.
     */

    entity_registry_reset(&registry_2);

    test_result(
        report,
        "Registry reset clears counts and IDs",
        registry_2.temperature_sensor_count == 0 &&
            registry_2.door_count == 0 &&
            registry_2.alarm_count == 0 &&
            registry_2.next_entity_id == 0);

    TemperatureSensor *reset_sensor =
        temperature_sensor_create(&registry_2);

    test_result(
        report,
        "Entity IDs restart after reset",
        reset_sensor != NULL &&
            reset_sensor->entity.id == 0);
}

static void test_temperature(FILE *report)
{
    separator("TEMPERATURE");

    EntityRegistry registry =
        entity_registry_initialize();

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry);

    test_result(
        report,
        "Temperature sensors created for temperature tests",
        sensor_1 != NULL &&
            sensor_2 != NULL);

    test_result(
        report,
        "Initial temperature is correct",
        sensor_1->current_temperature == 23.0f &&
            sensor_1->min_temperature == 23.0f &&
            sensor_1->max_temperature == 23.0f);

    temperature_update(sensor_1, 20.0f);
    temperature_update(sensor_1, 30.0f);
    temperature_update(sensor_1, 25.0f);

    test_result(
        report,
        "Temperature current value updates",
        sensor_1->current_temperature == 25.0f);

    test_result(
        report,
        "Temperature minimum is tracked",
        sensor_1->min_temperature == 20.0f);

    test_result(
        report,
        "Temperature maximum is tracked",
        sensor_1->max_temperature == 30.0f);

    temperature_update_all(&registry, 15.0f);

    test_result(
        report,
        "temperature_update_all updates every sensor",
        sensor_1->current_temperature == 15.0f &&
            sensor_2->current_temperature == 15.0f);

    test_result(
        report,
        "temperature_update_all updates minimum",
        sensor_1->min_temperature == 15.0f &&
            sensor_2->min_temperature == 15.0f);

    Event reading =
        generate_temperature_reading(
            &(Timestamp){0, 0, 0},
            sensor_1);

    test_result(
        report,
        "Temperature reading event is generated",
        reading.type == TEMPERATURE_READING &&
            reading.entity.id == sensor_1->entity.id &&
            reading.temperature == 15.0f);
}

static void test_doors_and_alarms(FILE *report)
{
    separator("DOORS + ALARMS");

    EntityRegistry registry =
        entity_registry_initialize();

    Door *door =
        door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    Timestamp time =
        timestamp_initialize();

    Event event;

    event = door_open(door, &time);

    test_result(
        report,
        "Closed door can be opened",
        event.type == DOOR_OPENED &&
            door->state == OPEN);

    event = door_open(door, &time);

    test_result(
        report,
        "Opening an already-open door is rejected",
        event.type == INVALID &&
            door->state == OPEN);

    event = alarm_trigger(alarm, &time);

    test_result(
        report,
        "Inactive alarm can be triggered",
        event.type == ALARM_TRIGGERED &&
            alarm->state == ACTIVE);

    event = alarm_trigger(alarm, &time);

    test_result(
        report,
        "Triggering active alarm is rejected",
        event.type == INVALID &&
            alarm->state == ACTIVE);

    timestamp_advance(&time);

    event = door_close(door, &time);

    test_result(
        report,
        "Open door can be closed",
        event.type == DOOR_CLOSED &&
            door->state == CLOSED);

    event = alarm_clear(alarm, &time);

    test_result(
        report,
        "Active alarm can be cleared",
        event.type == ALARM_CLEARED &&
            alarm->state == INACTIVE);

    event = door_close(door, &time);

    test_result(
        report,
        "Closing an already-closed door is rejected",
        event.type == INVALID);

    event = alarm_clear(alarm, &time);

    test_result(
        report,
        "Clearing inactive alarm is rejected",
        event.type == INVALID);

    Alarm *invalid_alarm =
        alarm_create(&registry, 9999);

    test_result(
        report,
        "Alarm for nonexistent door is rejected",
        invalid_alarm == NULL);
}

static void test_queue(FILE *report)
{
    separator("QUEUE");

    Queue queue = queue_create();

    test_result(
        report,
        "New queue is empty",
        queue_is_empty(&queue) &&
            queue_get_size(&queue) == 0);

    EntityRegistry registry =
        entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    Door *door =
        door_create(&registry);

    Timestamp time =
        timestamp_initialize();

    Event event_1 =
        generate_temperature_reading(&time, sensor);

    timestamp_advance(&time);

    Event event_2 =
        door_open(door, &time);

    queue_push(&queue, event_1);
    queue_push(&queue, event_2);

    test_result(
        report,
        "Queue size increases after push",
        queue_get_size(&queue) == 2);

    Event peeked;

    test_result(
        report,
        "Queue peek returns first event",
        queue_peek(&queue, &peeked) &&
            peeked.type == TEMPERATURE_READING);

    Event popped;

    bool pop_1 = queue_pop(&queue, &popped);

    test_result(
        report,
        "Queue pop returns first event",
        pop_1 &&
            popped.type == TEMPERATURE_READING);

    bool pop_2 = queue_pop(&queue, &popped);

    test_result(
        report,
        "Queue preserves FIFO order",
        pop_2 &&
            popped.type == DOOR_OPENED);

    test_result(
        report,
        "Queue is empty after popping all events",
        queue_is_empty(&queue) &&
            queue_get_size(&queue) == 0);

    test_result(
        report,
        "Peek on empty queue fails",
        !queue_peek(&queue, &peeked));

    test_result(
        report,
        "Pop on empty queue fails",
        !queue_pop(&queue, &popped));

    /*
     * Fill queue to capacity.
     */

    Event filler =
        event_create_invalid();

    for (int i = 0; i < MAX_SIZE; i++)
    {
        queue_push(&queue, filler);
    }

    test_result(
        report,
        "Queue reaches maximum capacity",
        queue_is_full(&queue) &&
            queue_get_size(&queue) == MAX_SIZE);

    /*
     * queue_push() returns void, so the only thing we can
     * verify after an extra push is that the size stays 100.
     */

    queue_push(&queue, filler);

    test_result(
        report,
        "Queue rejects pushes when full",
        queue_get_size(&queue) == MAX_SIZE);
}

static void test_scenario(FILE *report)
{
    separator("SCENARIO");

    EntityRegistry registry =
        entity_registry_initialize();

    temperature_sensor_create(&registry);
    temperature_sensor_create(&registry);

    Door *alarm_door =
        door_create(&registry);

    door_create(&registry);

    alarm_create(&registry, alarm_door->entity.id);

    Queue queue =
        queue_create();

    Timestamp clock =
        timestamp_initialize();

    /*
     * Run several seconds. Because the scenario is random,
     * we check structural invariants rather than expecting
     * one exact sequence of events.
     */

    int total_events = 0;
    int invalid_events = 0;

    for (int i = 0; i < 10; i++)
    {
        scenario_generate(
            &registry,
            &queue,
            &clock);

        Event event;

        while (queue_pop(&queue, &event))
        {
            total_events++;

            if (event.type == INVALID)
            {
                invalid_events++;
            }
        }

        timestamp_advance(&clock);
    }

    test_result(
        report,
        "Scenario generates events",
        total_events > 0);

    test_result(
        report,
        "Scenario does not enqueue invalid events",
        invalid_events == 0);

    test_result(
        report,
        "Scenario leaves queue empty after processing",
        queue_is_empty(&queue));
}

static void test_capacity(FILE *report)
{
    separator("ENTITY CAPACITY");

    EntityRegistry registry =
        entity_registry_initialize();

    for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++)
    {
        temperature_sensor_create(&registry);
    }

    TemperatureSensor *extra_sensor =
        temperature_sensor_create(&registry);

    test_result(
        report,
        "Temperature sensor limit is enforced",
        registry.temperature_sensor_count ==
                MAX_TEMPERATURE_SENSORS &&
            extra_sensor == NULL);

    /*
     * New registry for door capacity.
     */

    EntityRegistry door_registry =
        entity_registry_initialize();

    for (int i = 0; i < MAX_DOORS; i++)
    {
        door_create(&door_registry);
    }

    Door *extra_door =
        door_create(&door_registry);

    test_result(
        report,
        "Door limit is enforced",
        door_registry.door_count == MAX_DOORS &&
            extra_door == NULL);

    /*
     * Alarm capacity.
     */

    EntityRegistry alarm_registry =
        entity_registry_initialize();

    Door *alarm_test_door =
        door_create(&alarm_registry);

    int successful_alarms = 0;

    for (int i = 0; i < MAX_ALARMS; i++)
    {
        if (alarm_create(
                &alarm_registry,
                alarm_test_door->entity.id) != NULL)
        {
            successful_alarms++;
        }
    }

    Alarm *extra_alarm =
        alarm_create(
            &alarm_registry,
            alarm_test_door->entity.id);

    test_result(
        report,
        "Alarm limit is enforced",
        successful_alarms == MAX_ALARMS &&
            alarm_registry.alarm_count == MAX_ALARMS &&
            extra_alarm == NULL);
}

static void test_events(FILE *report)
{
    separator("EVENTS");

    EntityRegistry registry =
        entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    Door *door =
        door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    Timestamp time =
        timestamp_initialize();

    sensor->current_temperature = 25.5f;

    Event temperature =
        generate_temperature_reading(
            &time,
            sensor);

    Event opened =
        door_open(
            door,
            &time);

    Event triggered =
        alarm_trigger(
            alarm,
            &time);

    Event invalid =
        event_create_invalid();

    test_result(
        report,
        "Temperature event has correct type",
        temperature.type == TEMPERATURE_READING);

    test_result(
        report,
        "Temperature event contains temperature",
        temperature.temperature == 25.5f);

    test_result(
        report,
        "Door event has correct type",
        opened.type == DOOR_OPENED);

    test_result(
        report,
        "Alarm event has correct type",
        triggered.type == ALARM_TRIGGERED);

    test_result(
        report,
        "Invalid event has invalid entity",
        invalid.type == INVALID &&
            invalid.entity.id == -1 &&
            invalid.entity.type == INVALID_ENTITY &&
            invalid.entity.id_in_entity_registry_type == -1);

    /*
     * Also put representative events in the report.
     */

    fprintf(
        report,
        "\nRepresentative events:\n");

    write_event(report, &temperature);
    write_event(report, &opened);
    write_event(report, &triggered);
    write_event(report, &invalid);
}

static void run_simulations(FILE *report)
{
    separator("SIMULATIONS");

    Timestamp short_duration =
        timestamp_create(0, 0, 5);

    Timestamp second_duration =
        timestamp_create(0, 0, 8);

    printf("Running simulation 1...\n");

    simulation_run(
        "SIMULATION_1.txt",
        2,
        1,
        1,
        short_duration);

    printf("Running simulation 2...\n");

    simulation_run(
        "SIMULATION_2.txt",
        3,
        2,
        0,
        second_duration);

    fprintf(
        report,
        "\nSimulation files created:\n");

    fprintf(
        report,
        "  SIMULATION_1.txt\n");

    fprintf(
        report,
        "  SIMULATION_2.txt\n");
}

static void test_invalid_simulation_input(FILE *report)
{
    separator("INVALID SIMULATION INPUT");

    Timestamp valid_duration =
        timestamp_create(0, 0, 2);

    printf("Too many sensors:\n");

    simulation_run(
        "INVALID_SENSORS.txt",
        MAX_TEMPERATURE_SENSORS + 1,
        0,
        0,
        valid_duration);

    printf("\nToo many doors:\n");

    simulation_run(
        "INVALID_DOORS.txt",
        0,
        3,
        3,
        valid_duration);

    printf("\nInvalid duration:\n");

    simulation_run(
        "INVALID_DURATION.txt",
        1,
        1,
        0,
        timestamp_create(2, 0, 0));

    fprintf(
        report,
        "Invalid simulation inputs were tested.\n");

    /*
     * These should not create simulation files because
     * validation occurs before fopen().
     */
}

int main(void)
{
    /*
     * Fixed seed makes scenario tests reproducible.
     */
    srand(23);

    FILE *report =
        fopen("TEST_REPORT.txt", "w");

    if (report == NULL)
    {
        printf("ERROR: Could not create TEST_REPORT.txt\n");
        return 1;
    }

    printf("BlackBox - Version 1\n");

    fprintf(
        report,
        "BlackBox - Version 1\n");

    fprintf(
        report,
        "Automated test report\n\n");

    /*
     * Run individual component tests.
     */

    test_timestamp(report);
    test_registry(report);
    test_temperature(report);
    test_doors_and_alarms(report);
    test_queue(report);
    test_events(report);
    test_capacity(report);
    test_scenario(report);

    /*
     * Run actual simulations.
     */

    run_simulations(report);

    /*
     * Test invalid simulation input.
     */

    test_invalid_simulation_input(report);

    /*
     * Final result.
     */

    separator("FINAL RESULT");

    printf(
        "Tests passed: %d\n",
        tests_passed);

    printf(
        "Tests failed: %d\n",
        tests_failed);

    fprintf(
        report,
        "\n--- FINAL RESULT ---\n");

    fprintf(
        report,
        "Tests passed: %d\n",
        tests_passed);

    fprintf(
        report,
        "Tests failed: %d\n",
        tests_failed);

    if (tests_failed == 0)
    {
        printf("All tests passed.\n");
        fprintf(report, "Result: ALL TESTS PASSED\n");
    }
    else
    {
        printf("Some tests failed.\n");
        fprintf(report, "Result: SOME TESTS FAILED\n");
    }

    fclose(report);

    printf("\nTest report: TEST_REPORT.txt\n");
    printf("Simulation files:\n");
    printf("  SIMULATION_1.txt\n");
    printf("  SIMULATION_2.txt\n");

    return tests_failed == 0 ? 0 : 1;
}