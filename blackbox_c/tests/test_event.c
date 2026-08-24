#include <stdio.h>
#include <assert.h>

#include "../include/event.h"

static void test_event_create(void)
{
    Timestamp timestamp = timestamp_create(0, 1, 2);

    Entity entity;
    entity.id = 5;
    entity.type = DOOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, DOOR_OPENED, entity);

    assert(event.timestamp.hours == 0);
    assert(event.timestamp.minutes == 1);
    assert(event.timestamp.seconds == 2);

    assert(event.type == DOOR_OPENED);
    assert(event.entity.id == 5);
    assert(event.entity.type == DOOR);
    assert(event.entity.id_in_entity_registry_type == 0);
    assert(event.temperature == 0.0f);
}

static void test_invalid_event(void)
{
    Event event = event_create_invalid();

    assert(event.type == INVALID);
    assert(event.entity.id == -1);
    assert(event.entity.type == INVALID_ENTITY);
    assert(event.entity.id_in_entity_registry_type == -1);
    assert(event.temperature == 0.0f);

    assert(event.timestamp.hours == 0);
    assert(event.timestamp.minutes == 0);
    assert(event.timestamp.seconds == 0);
}

static void test_temperature_event(void)
{
    Timestamp timestamp = timestamp_create(0, 0, 10);

    Entity entity;
    entity.id = 0;
    entity.type = TEMPERATURE_SENSOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, TEMPERATURE_READING, entity);

    event.temperature = 25.5f;

    assert(event.type == TEMPERATURE_READING);
    assert(event.entity.id == 0);
    assert(event.temperature == 25.5f);
}

static void test_event_write(void)
{
    Timestamp timestamp = timestamp_create(0, 0, 5);

    Entity entity;
    entity.id = 2;
    entity.type = DOOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, DOOR_OPENED, entity);

    FILE *file = fopen("test_event_output.txt", "w");

    assert(file != NULL);

    event_write_to_file(&event, file);

    fclose(file);
}

int main(void)
{
    printf("=== EVENT TESTS ===\n");

    test_event_create();
    printf("[PASS] event_create\n");

    test_invalid_event();
    printf("[PASS] event_create_invalid\n");

    test_temperature_event();
    printf("[PASS] temperature event\n");

    test_event_write();
    printf("[PASS] event_write_to_file\n");

    printf("=== ALL EVENT TESTS PASSED ===\n");

    return 0;
}