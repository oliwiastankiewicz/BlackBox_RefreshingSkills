#include <stdio.h>
#include <assert.h>

#include "../include/timestamp.h"

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
    Timestamp timestamp = timestamp_create(0, 60, 0);

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
    printf("=== TIMESTAMP TESTS ===\n");

    test_initialize();
    printf("[PASS] initialization\n");

    test_validate();
    printf("[PASS] validation\n");

    test_create();
    printf("[PASS] creation\n");

    test_create_invalid();
    printf("[PASS] invalid creation\n");

    test_advance_seconds();
    printf("[PASS] advance seconds\n");

    test_advance_minute();
    printf("[PASS] advance minute\n");

    test_advance_hour();
    printf("[PASS] advance hour\n");

    test_compare();
    printf("[PASS] comparison\n");

    test_compare_hours();
    printf("[PASS] hour comparison\n");

    printf("=== ALL TIMESTAMP TESTS PASSED ===\n");

    return 0;
}