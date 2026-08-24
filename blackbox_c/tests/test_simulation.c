#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../include/simulation.h"
#include "../include/timestamp.h"

static int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    fclose(file);
    return 1;
}

static int file_has_content(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return 0;
    }

    int character = fgetc(file);

    fclose(file);

    return character != EOF;
}

static void test_basic_simulation(void)
{
    const char *filename = "test_simulation_basic.txt";

    Timestamp duration = timestamp_create(0, 0, 3);

    simulation_run(filename, 2, 1, 1, duration);

    assert(file_exists(filename));
    assert(file_has_content(filename));

    remove(filename);
}

static void test_multiple_simulations(void)
{
    const char *file1 = "test_simulation_1.txt";
    const char *file2 = "test_simulation_2.txt";

    Timestamp duration = timestamp_create(0, 0, 2);

    simulation_run(file1, 1, 1, 0, duration);

    simulation_run(file2, 2, 0, 2, duration);

    assert(file_exists(file1));
    assert(file_exists(file2));

    assert(file_has_content(file1));
    assert(file_has_content(file2));

    remove(file1);
    remove(file2);
}

static void test_zero_entities(void)
{
    const char *filename = "test_simulation_empty.txt";

    Timestamp duration = timestamp_create(0, 0, 2);

    simulation_run(filename, 0, 0, 0, duration);

    assert(file_exists(filename));

    remove(filename);
}

static void test_maximum_valid_entities(void)
{
    const char *filename = "test_simulation_max.txt";

    Timestamp duration = timestamp_create(0, 0, 1);

    simulation_run(filename, MAX_TEMPERATURE_SENSORS, 0, MAX_DOORS, duration);

    assert(file_exists(filename));

    remove(filename);
}

static void test_invalid_sensor_count(void)
{
    const char *filename = "test_simulation_invalid_sensor.txt";

    Timestamp duration = timestamp_create(0, 0, 1);

    simulation_run(filename, MAX_TEMPERATURE_SENSORS + 1, 0, 0, duration);

    assert(!file_exists(filename));
}

static void test_invalid_door_count(void)
{
    const char *filename = "test_simulation_invalid_doors.txt";

    Timestamp duration = timestamp_create(0, 0, 1);

    simulation_run(filename, 0, 3, 3, duration);

    assert(!file_exists(filename));
}

static void test_invalid_duration(void)
{
    const char *filename = "test_simulation_invalid_duration.txt";

    Timestamp duration = timestamp_create(2, -1, 0);

    simulation_run(filename, 1, 1, 0, duration);

    assert(!file_exists(filename));
}

static void test_simulation_file_overwrite(void)
{
    const char *filename = "test_simulation_overwrite.txt";

    Timestamp duration = timestamp_create(0, 0, 1);

    simulation_run(filename, 1, 1, 0, duration);

    assert(file_exists(filename));

    simulation_run(filename, 2, 0, 1, duration);

    assert(file_exists(filename));
    assert(file_has_content(filename));

    remove(filename);
}

int main(void)
{
    printf("=== SIMULATION TESTS ===\n");

    test_basic_simulation();
    printf("[PASS] basic simulation\n");

    test_multiple_simulations();
    printf("[PASS] multiple independent simulations\n");

    test_zero_entities();
    printf("[PASS] zero entities\n");

    test_maximum_valid_entities();
    printf("[PASS] maximum valid entities\n");

    test_invalid_sensor_count();
    printf("[PASS] invalid sensor count\n");

    test_invalid_door_count();
    printf("[PASS] invalid door count\n");

    test_invalid_duration();
    printf("[PASS] invalid duration\n");

    test_simulation_file_overwrite();
    printf("[PASS] simulation file handling\n");

    printf("=== ALL SIMULATION TESTS PASSED ===\n");

    return 0;
}