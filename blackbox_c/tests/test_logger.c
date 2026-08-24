#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

#define TEST_LOG_FILE "blackbox_c/recordings/tests/test_logger_output.txt"
#define TEST_REPORT_FILE "blackbox_c/recordings/tests/test_logger.txt"

static void test_logger_create(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);
    assert(logger.passed == 0);
    assert(logger.failed == 0);

    logger_destroy(&logger);

    remove(TEST_LOG_FILE);
}

static void test_logger_test_pass(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);

    logger_test(&logger, "Example test", true);

    assert(logger.passed == 1);
    assert(logger.failed == 0);

    logger_destroy(&logger);

    FILE *file = fopen(TEST_LOG_FILE, "r");

    assert(file != NULL);

    char buffer[100];

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[PASS] Example test\n") == 0);

    fclose(file);
    remove(TEST_LOG_FILE);
}

static void test_logger_test_fail(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);

    logger_test(&logger, "Example test", false);

    assert(logger.passed == 0);
    assert(logger.failed == 1);

    logger_destroy(&logger);

    FILE *file = fopen(TEST_LOG_FILE, "r");

    assert(file != NULL);

    char buffer[100];

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[FAIL] Example test\n") == 0);

    fclose(file);
    remove(TEST_LOG_FILE);
}

static void test_logger_multiple_tests(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);

    logger_test(&logger, "Test 1", true);
    logger_test(&logger, "Test 2", true);
    logger_test(&logger, "Test 3", false);
    logger_test(&logger, "Test 4", true);

    assert(logger.passed == 3);
    assert(logger.failed == 1);

    logger_destroy(&logger);

    FILE *file = fopen(TEST_LOG_FILE, "r");

    assert(file != NULL);

    char buffer[100];

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[PASS] Test 1\n") == 0);

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[PASS] Test 2\n") == 0);

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[FAIL] Test 3\n") == 0);

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "[PASS] Test 4\n") == 0);

    fclose(file);
    remove(TEST_LOG_FILE);
}

static void test_logger_section(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);

    logger_section(&logger, "ENTITY TESTS");

    logger_destroy(&logger);

    FILE *file = fopen(TEST_LOG_FILE, "r");

    assert(file != NULL);

    char buffer[100];

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "\n") == 0);

    assert(fgets(buffer, sizeof(buffer), file) != NULL);
    assert(strcmp(buffer, "--- ENTITY TESTS ---\n") == 0);

    fclose(file);
    remove(TEST_LOG_FILE);
}

static void test_logger_destroy(void)
{
    Logger logger = logger_create(TEST_LOG_FILE);

    assert(logger.file != NULL);

    logger_destroy(&logger);

    assert(logger.file == NULL);

    remove(TEST_LOG_FILE);
}

static void test_logger_invalid_file(void)
{
    Logger logger = logger_create("/invalid/path/test.log");

    assert(logger.file == NULL);
    assert(logger.passed == 0);
    assert(logger.failed == 0);

    logger_test(&logger, "Should not crash", true);
    logger_section(&logger, "Should not crash");

    assert(logger.passed == 0);
    assert(logger.failed == 0);

    logger_destroy(&logger);
}

int main(void)
{
    Logger logger = logger_create(TEST_REPORT_FILE);

    if (logger.file == NULL)
    {
        printf("Could not create %s\n", TEST_REPORT_FILE);
        return 1;
    }

    printf("=== LOGGER TESTS ===\n");

    logger_section(&logger, "LOGGER TESTS");

    test_logger_create();
    logger_test(
        &logger,
        "logger create",
        true);
    printf("[PASS] logger create\n");

    test_logger_test_pass();
    logger_test(
        &logger,
        "logger pass",
        true);
    printf("[PASS] logger pass\n");

    test_logger_test_fail();
    logger_test(
        &logger,
        "logger fail",
        true);
    printf("[PASS] logger fail\n");

    test_logger_multiple_tests();
    logger_test(
        &logger,
        "multiple tests",
        true);
    printf("[PASS] multiple tests\n");

    test_logger_section();
    logger_test(
        &logger,
        "logger section",
        true);
    printf("[PASS] logger section\n");

    test_logger_destroy();
    logger_test(
        &logger,
        "logger destroy",
        true);
    printf("[PASS] logger destroy\n");

    test_logger_invalid_file();
    logger_test(
        &logger,
        "invalid file handling",
        true);
    printf("[PASS] invalid file handling\n");

    logger_section(&logger, "SUMMARY");

    fprintf(
        logger.file,
        "Passed: %d\n",
        logger.passed);

    fprintf(
        logger.file,
        "Failed: %d\n",
        logger.failed);

    printf("\n=== ALL LOGGER TESTS PASSED ===\n");
    printf(
        "Results written to %s\n",
        TEST_REPORT_FILE);

    logger_destroy(&logger);

    return 0;
}