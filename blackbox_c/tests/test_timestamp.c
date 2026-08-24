#include <assert.h>
#include <stdio.h>

#include "timestamp.h"
#include "logger.h"

static void test_initialize(void)
{
    Timestamp timestamp = timestamp_initialize();

    assert(timestamp.hours == 0);
    assert(timestamp.minutes == 0);
    assert(timestamp.seconds == 0);
}

static void test_validate(void)
{
    assert(timestamp_validate(0, 0, 0));
    assert(timestamp_validate(1, 59, 59));

    assert(!timestamp_validate(-1, 0, 0));
    assert(!timestamp_validate(0, -1, 0));
    assert(!timestamp_validate(0, 0, -1));

    assert(!timestamp_validate(0, 60, 0));
    assert(!timestamp_validate(0, 0, 60));

    assert(!timestamp_validate(2, 0, 0));
}

static void test_create(void)
{
    Timestamp timestamp = timestamp_create(0, 25, 30);

    assert(timestamp.hours == 0);
    assert(timestamp.minutes == 25);
    assert(timestamp.seconds == 30);
}

static void test_create_invalid(void)
{
    Timestamp timestamp = timestamp_create(-1, -1, -1);

    assert(timestamp.hours == -1);
    assert(timestamp.minutes == -1);
    assert(timestamp.seconds == -1);
}

static void test_advance_seconds(void)
{
    Timestamp timestamp = timestamp_create(0, 0, 0);

    timestamp_advance(&timestamp);

    assert(timestamp.hours == 0);
    assert(timestamp.minutes == 0);
    assert(timestamp.seconds == 1);
}

static void test_advance_minute(void)
{
    Timestamp timestamp = timestamp_create(0, 0, 59);

    timestamp_advance(&timestamp);

    assert(timestamp.hours == 0);
    assert(timestamp.minutes == 1);
    assert(timestamp.seconds == 0);
}

static void test_advance_hour(void)
{
    Timestamp timestamp = timestamp_create(0, 59, 59);

    timestamp_advance(&timestamp);

    assert(timestamp.hours == 1);
    assert(timestamp.minutes == 0);
    assert(timestamp.seconds == 0);
}

static void test_compare(void)
{
    Timestamp first = timestamp_create(0, 10, 0);
    Timestamp second = timestamp_create(0, 20, 0);
    Timestamp third = timestamp_create(0, 10, 0);

    assert(timestamp_compare(&first, &second) == -1);
    assert(timestamp_compare(&second, &first) == 1);
    assert(timestamp_compare(&first, &third) == 0);
}

static void test_compare_hours(void)
{
    Timestamp first = timestamp_create(0, 59, 59);
    Timestamp second = timestamp_create(1, 0, 0);

    assert(timestamp_compare(&first, &second) == -1);
    assert(timestamp_compare(&second, &first) == 1);
}

int main(void)
{
    Logger logger = logger_create("blackbox_c/recordings/tests/test_timestamp.txt");

    if (logger.file == NULL)
    {
        printf("Could not create test_timestamp.txt\n");
        return 1;
    }

    printf("=== TIMESTAMP TESTS ===\n");
    logger_section(&logger, "TIMESTAMP TESTS");

    test_initialize();
    logger_test(&logger, "initialization", true);
    printf("[PASS] initialization\n");

    test_validate();
    logger_test(&logger, "validation", true);
    printf("[PASS] validation\n");

    test_create();
    logger_test(&logger, "creation", true);
    printf("[PASS] creation\n");

    test_create_invalid();
    logger_test(&logger, "invalid creation", true);
    printf("[PASS] invalid creation\n");

    test_advance_seconds();
    logger_test(&logger, "advance seconds", true);
    printf("[PASS] advance seconds\n");

    test_advance_minute();
    logger_test(&logger, "advance minute", true);
    printf("[PASS] advance minute\n");

    test_advance_hour();
    logger_test(&logger, "advance hour", true);
    printf("[PASS] advance hour\n");

    test_compare();
    logger_test(&logger, "comparison", true);
    printf("[PASS] comparison\n");

    test_compare_hours();
    logger_test(&logger, "hour comparison", true);
    printf("[PASS] hour comparison\n");

    logger_section(&logger, "SUMMARY");
    fprintf(logger.file, "Passed: %d\n", logger.passed);
    fprintf(logger.file, "Failed: %d\n", logger.failed);

    printf("\n=== ALL TIMESTAMP TESTS PASSED ===\n");
    printf("Results written to blackbox_c/recordings/tests/test_timestamp.txt\n");

    logger_destroy(&logger);
    return 0;
}