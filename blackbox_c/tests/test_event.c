#include <stdio.h>
#include <stdbool.h>

#include "../include/event.h"
#include "../include/logger.h"

static bool test_event_create(void)
{
    Timestamp timestamp = timestamp_create(0, 1, 2);

    Entity entity;
    entity.id = 5;
    entity.type = DOOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, DOOR_OPENED, entity);

    return event.timestamp.hours == 0 &&
           event.timestamp.minutes == 1 &&
           event.timestamp.seconds == 2 &&
           event.type == DOOR_OPENED &&
           event.entity.id == 5 &&
           event.entity.type == DOOR &&
           event.entity.id_in_entity_registry_type == 0 &&
           event.temperature == 0.0f;
}

static bool test_invalid_event(void)
{
    Event event = event_create_invalid();

    return event.type == INVALID &&
           event.entity.id == -1 &&
           event.entity.type == INVALID_ENTITY &&
           event.entity.id_in_entity_registry_type == -1 &&
           event.temperature == 0.0f &&
           event.timestamp.hours == 0 &&
           event.timestamp.minutes == 0 &&
           event.timestamp.seconds == 0;
}

static bool test_temperature_event(void)
{
    Timestamp timestamp = timestamp_create(0, 0, 10);

    Entity entity;
    entity.id = 0;
    entity.type = TEMPERATURE_SENSOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, TEMPERATURE_READING, entity);

    event.temperature = 25.5f;

    return event.type == TEMPERATURE_READING &&
           event.entity.id == 0 &&
           event.entity.type == TEMPERATURE_SENSOR &&
           event.temperature == 25.5f;
}

static bool test_event_write(Logger *logger)
{
    Timestamp timestamp = timestamp_create(0, 0, 5);

    Entity entity;
    entity.id = 2;
    entity.type = DOOR;
    entity.id_in_entity_registry_type = 0;

    Event event = event_create(timestamp, DOOR_OPENED, entity);

    if (logger->file == NULL)
    {
        return false;
    }

    event_write_to_file(&event, logger->file);

    /*
     * Make sure the output is physically written to the file
     * before the test continues.
     */
    fflush(logger->file);

    return true;
}

int main(void)
{
    Logger logger =
        logger_create("blackbox_c/recordings/tests/test_event.txt");

    if (logger.file == NULL)
    {
        printf("Could not create test_event.txt\n");
        return 1;
    }

    printf("=== EVENT TESTS ===\n");

    logger_section(&logger, "EVENT TESTS");

    bool passed;

    passed = test_event_create();
    logger_test(&logger, "event_create", passed);

    passed = test_invalid_event();
    logger_test(&logger, "event_create_invalid", passed);

    passed = test_temperature_event();
    logger_test(&logger, "temperature event", passed);

    logger_section(&logger, "EVENT FILE OUTPUT");

    passed = test_event_write(&logger);
    logger_test(&logger, "event_write_to_file", passed);

    logger_section(&logger, "SUMMARY");

    fprintf(
        logger.file,
        "Passed: %d\n",
        logger.passed);

    fprintf(
        logger.file,
        "Failed: %d\n",
        logger.failed);

    int failed = logger.failed;

    logger_destroy(&logger);

    if (failed > 0)
    {
        printf("=== EVENT TESTS FAILED ===\n");
        return 1;
    }

    printf("=== ALL EVENT TESTS PASSED ===\n");

    return 0;
}