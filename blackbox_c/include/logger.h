#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Logger
{
    FILE *file;
    int passed;
    int failed;
} Logger;

Logger logger_create(const char *filename);

void logger_destroy(Logger *logger);

void logger_test(Logger *logger, const char *test_name, bool passed);

void logger_section(Logger *logger, const char *section_name);

#endif