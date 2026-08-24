#include "../include/logger.h"

#include <stdio.h>

Logger logger_create(const char *filename)
{
    Logger logger;

    logger.file = fopen(filename, "w");

    logger.passed = 0;
    logger.failed = 0;

    return logger;
}

void logger_destroy(Logger *logger)
{
    if (logger->file != NULL)
    {
        fclose(logger->file);
        logger->file = NULL;
    }
}

void logger_test(Logger *logger, const char *test_name, bool passed)
{
    if (logger->file == NULL)
    {
        return;
    }

    if (passed)
    {
        fprintf(logger->file, "[PASS] %s\n", test_name);
        logger->passed++;
    }
    else
    {
        fprintf(logger->file, "[FAIL] %s\n", test_name);
        logger->failed++;
    }
}

void logger_section(Logger *logger, const char *section_name)
{
    if (logger->file == NULL)
    {
        return;
    }

    fprintf(logger->file, "\n--- %s ---\n", section_name);
}