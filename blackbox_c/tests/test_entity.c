#include <stdio.h>
#include <assert.h>

#include "../include/entity.h"
#include "../include/timestamp.h"
#include "../include/logger.h"

static void test_registry_initialize(void)
{
    EntityRegistry registry = entity_registry_initialize();

    assert(registry.temperature_sensor_count == 0);
    assert(registry.door_count == 0);
    assert(registry.alarm_count == 0);
    assert(registry.next_entity_id == 0);
}

static void test_registry_reset(void)
{
    EntityRegistry registry = entity_registry_initialize();

    temperature_sensor_create(&registry);
    door_create(&registry);
    alarm_create(&registry, registry.doors[0].entity.id);

    entity_registry_reset(&registry);

    assert(registry.temperature_sensor_count == 0);
    assert(registry.door_count == 0);
    assert(registry.alarm_count == 0);
    assert(registry.next_entity_id == 0);
}

static void test_registry_independence(void)
{
    EntityRegistry registry_1 = entity_registry_initialize();
    EntityRegistry registry_2 = entity_registry_initialize();

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry_1);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry_2);

    assert(sensor_1->entity.id == 0);
    assert(sensor_2->entity.id == 0);

    assert(registry_1.next_entity_id == 1);
    assert(registry_2.next_entity_id == 1);
}

static void test_temperature_sensor_create(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    assert(sensor != NULL);
    assert(sensor->entity.id == 0);
    assert(sensor->entity.type == TEMPERATURE_SENSOR);
    assert(sensor->entity.id_in_entity_registry_type == 0);

    assert(sensor->current_temperature == 23.0f);
    assert(sensor->min_temperature == 23.0f);
    assert(sensor->max_temperature == 23.0f);

    assert(registry.temperature_sensor_count == 1);
    assert(registry.next_entity_id == 1);
}

static void test_temperature_sensor_ids(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry);

    assert(sensor_1->entity.id == 0);
    assert(sensor_2->entity.id == 1);

    assert(sensor_1->entity.id_in_entity_registry_type == 0);
    assert(sensor_2->entity.id_in_entity_registry_type == 1);
}

static void test_temperature_sensor_capacity(void)
{
    EntityRegistry registry = entity_registry_initialize();

    for (int i = 0; i < MAX_TEMPERATURE_SENSORS; i++)
    {
        assert(temperature_sensor_create(&registry) != NULL);
    }

    assert(registry.temperature_sensor_count ==
           MAX_TEMPERATURE_SENSORS);

    TemperatureSensor *extra =
        temperature_sensor_create(&registry);

    assert(extra == NULL);

    assert(registry.temperature_sensor_count ==
           MAX_TEMPERATURE_SENSORS);
}

static void test_temperature_update(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    temperature_update(sensor, 30.0f);

    assert(sensor->current_temperature == 30.0f);
    assert(sensor->min_temperature == 23.0f);
    assert(sensor->max_temperature == 30.0f);

    temperature_update(sensor, 15.0f);

    assert(sensor->current_temperature == 15.0f);
    assert(sensor->min_temperature == 15.0f);
    assert(sensor->max_temperature == 30.0f);

    temperature_update(sensor, 25.0f);

    assert(sensor->current_temperature == 25.0f);
    assert(sensor->min_temperature == 15.0f);
    assert(sensor->max_temperature == 30.0f);
}

static void test_temperature_update_all(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor_1 =
        temperature_sensor_create(&registry);

    TemperatureSensor *sensor_2 =
        temperature_sensor_create(&registry);

    temperature_update_all(&registry, 28.0f);

    assert(sensor_1->current_temperature == 28.0f);
    assert(sensor_2->current_temperature == 28.0f);

    assert(sensor_1->max_temperature == 28.0f);
    assert(sensor_2->max_temperature == 28.0f);
}

static void test_door_create(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    assert(door != NULL);
    assert(door->entity.id == 0);
    assert(door->entity.type == DOOR);
    assert(door->entity.id_in_entity_registry_type == 0);

    assert(door->state == CLOSED);

    assert(registry.door_count == 1);
    assert(registry.next_entity_id == 1);
}

static void test_door_ids(void)
{
    EntityRegistry registry = entity_registry_initialize();

    TemperatureSensor *sensor =
        temperature_sensor_create(&registry);

    Door *door_1 =
        door_create(&registry);

    Door *door_2 =
        door_create(&registry);

    assert(sensor->entity.id == 0);
    assert(door_1->entity.id == 1);
    assert(door_2->entity.id == 2);

    assert(door_1->entity.id_in_entity_registry_type == 0);
    assert(door_2->entity.id_in_entity_registry_type == 1);
}

static void test_door_capacity(void)
{
    EntityRegistry registry = entity_registry_initialize();

    for (int i = 0; i < MAX_DOORS; i++)
    {
        assert(door_create(&registry) != NULL);
    }

    assert(registry.door_count == MAX_DOORS);

    Door *extra = door_create(&registry);

    assert(extra == NULL);
    assert(registry.door_count == MAX_DOORS);
}

static void test_door_open_close(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    Timestamp timestamp = timestamp_initialize();

    Event event = door_open(door, &timestamp);

    assert(event.type == DOOR_OPENED);
    assert(event.entity.id == door->entity.id);
    assert(door->state == OPEN);

    timestamp_advance(&timestamp);

    event = door_close(door, &timestamp);

    assert(event.type == DOOR_CLOSED);
    assert(event.entity.id == door->entity.id);
    assert(door->state == CLOSED);
}

static void test_invalid_door_transitions(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    Timestamp timestamp = timestamp_initialize();

    Event event = door_close(door, &timestamp);

    assert(event.type == INVALID);
    assert(door->state == CLOSED);

    door_open(door, &timestamp);

    event = door_open(door, &timestamp);

    assert(event.type == INVALID);
    assert(door->state == OPEN);
}

static void test_alarm_create(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    assert(alarm != NULL);
    assert(alarm->entity.id == 1);
    assert(alarm->entity.type == ALARM);
    assert(alarm->entity.id_in_entity_registry_type == 0);

    assert(alarm->door_id == door->entity.id);
    assert(alarm->state == INACTIVE);

    assert(registry.alarm_count == 1);
    assert(registry.next_entity_id == 2);
}

static void test_alarm_invalid_door(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Alarm *alarm =
        alarm_create(&registry, 9999);

    assert(alarm == NULL);
    assert(registry.alarm_count == 0);
    assert(registry.next_entity_id == 0);
}

static void test_alarm_capacity(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    for (int i = 0; i < MAX_ALARMS; i++)
    {
        assert(alarm_create(&registry, door->entity.id) != NULL);
    }

    assert(registry.alarm_count == MAX_ALARMS);

    Alarm *extra =
        alarm_create(&registry, door->entity.id);

    assert(extra == NULL);
    assert(registry.alarm_count == MAX_ALARMS);
}

static void test_alarm_trigger_clear(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    Timestamp timestamp = timestamp_initialize();

    Event event = alarm_trigger(alarm, &timestamp);

    assert(event.type == ALARM_TRIGGERED);
    assert(event.entity.id == alarm->entity.id);
    assert(alarm->state == ACTIVE);

    timestamp_advance(&timestamp);

    event = alarm_clear(alarm, &timestamp);

    assert(event.type == ALARM_CLEARED);
    assert(event.entity.id == alarm->entity.id);
    assert(alarm->state == INACTIVE);
}

static void test_invalid_alarm_transitions(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    Alarm *alarm =
        alarm_create(&registry, door->entity.id);

    Timestamp timestamp = timestamp_initialize();

    Event event = alarm_clear(alarm, &timestamp);

    assert(event.type == INVALID);
    assert(alarm->state == INACTIVE);

    alarm_trigger(alarm, &timestamp);

    event = alarm_trigger(alarm, &timestamp);

    assert(event.type == INVALID);
    assert(alarm->state == ACTIVE);
}

static void test_door_exists(void)
{
    EntityRegistry registry = entity_registry_initialize();

    Door *door = door_create(&registry);

    assert(entity_registry_check_door_exists(
        &registry,
        door->entity.id));

    assert(!entity_registry_check_door_exists(
        &registry,
        9999));
}

int main(void)
{
    Logger logger =
        logger_create(
            "blackbox_c/recordings/tests/test_entity.txt");

    if (logger.file == NULL)
    {
        printf("Could not create test_entity.txt\n");
        return 1;
    }

    printf("=== ENTITY TESTS ===\n");

    logger_section(&logger, "ENTITY TESTS");

    test_registry_initialize();
    logger_test(
        &logger,
        "registry initialization",
        true);
    printf("[PASS] registry initialization\n");

    test_registry_reset();
    logger_test(
        &logger,
        "registry reset",
        true);
    printf("[PASS] registry reset\n");

    test_registry_independence();
    logger_test(
        &logger,
        "registry independence",
        true);
    printf("[PASS] registry independence\n");

    test_temperature_sensor_create();
    logger_test(
        &logger,
        "temperature sensor creation",
        true);
    printf("[PASS] temperature sensor creation\n");

    test_temperature_sensor_ids();
    logger_test(
        &logger,
        "temperature sensor IDs",
        true);
    printf("[PASS] temperature sensor IDs\n");

    test_temperature_sensor_capacity();
    logger_test(
        &logger,
        "temperature sensor capacity",
        true);
    printf("[PASS] temperature sensor capacity\n");

    test_temperature_update();
    logger_test(
        &logger,
        "temperature update",
        true);
    printf("[PASS] temperature update\n");

    test_temperature_update_all();
    logger_test(
        &logger,
        "temperature update all",
        true);
    printf("[PASS] temperature update all\n");

    test_door_create();
    logger_test(
        &logger,
        "door creation",
        true);
    printf("[PASS] door creation\n");

    test_door_ids();
    logger_test(
        &logger,
        "door IDs",
        true);
    printf("[PASS] door IDs\n");

    test_door_capacity();
    logger_test(
        &logger,
        "door capacity",
        true);
    printf("[PASS] door capacity\n");

    test_door_open_close();
    logger_test(
        &logger,
        "door open/close",
        true);
    printf("[PASS] door open/close\n");

    test_invalid_door_transitions();
    logger_test(
        &logger,
        "invalid door transitions",
        true);
    printf("[PASS] invalid door transitions\n");

    test_alarm_create();
    logger_test(
        &logger,
        "alarm creation",
        true);
    printf("[PASS] alarm creation\n");

    test_alarm_invalid_door();
    logger_test(
        &logger,
        "invalid alarm door",
        true);
    printf("[PASS] invalid alarm door\n");

    test_alarm_capacity();
    logger_test(
        &logger,
        "alarm capacity",
        true);
    printf("[PASS] alarm capacity\n");

    test_alarm_trigger_clear();
    logger_test(
        &logger,
        "alarm trigger/clear",
        true);
    printf("[PASS] alarm trigger/clear\n");

    test_invalid_alarm_transitions();
    logger_test(
        &logger,
        "invalid alarm transitions",
        true);
    printf("[PASS] invalid alarm transitions\n");

    test_door_exists();
    logger_test(
        &logger,
        "door existence check",
        true);
    printf("[PASS] door existence check\n");

    logger_section(&logger, "SUMMARY");

    fprintf(
        logger.file,
        "Passed: %d\n",
        logger.passed);

    fprintf(
        logger.file,
        "Failed: %d\n",
        logger.failed);

    printf("\n=== ALL ENTITY TESTS PASSED ===\n");
    printf("Results written to blackbox_c/recordings/tests/test_entity.txt\n");

    logger_destroy(&logger);

    return 0;
}