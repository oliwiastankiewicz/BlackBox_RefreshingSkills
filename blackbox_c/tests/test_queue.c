#include "../include/queue.h"
#include "../include/logger.h"
#include "../include/event.h"

#include <stdio.h>

static void test_queue_create(Logger *logger)
{
    Queue queue = queue_create();

    logger_test(logger, "Queue initializes empty", queue_is_empty(&queue) && queue_get_size(&queue) == 0);

    logger_test(logger, "New queue is not full", !queue_is_full(&queue));
}

static void test_queue_push(Logger *logger)
{
    Queue queue = queue_create();

    Event event = event_create_invalid();

    queue_push(&queue, event);

    logger_test(logger, "Queue size increases after push", queue_get_size(&queue) == 1);

    logger_test(logger, "Queue is no longer empty after push", !queue_is_empty(&queue));
}

static void test_queue_peek(Logger *logger)
{
    Queue queue = queue_create();

    Event event = event_create_invalid();

    queue_push(&queue, event);

    Event result;

    bool success = queue_peek(&queue, &result);

    logger_test(logger, "Queue peek succeeds", success);

    logger_test(logger, "Peek does not remove event", queue_get_size(&queue) == 1);
}

static void test_queue_pop(Logger *logger)
{
    Queue queue = queue_create();

    Event event = event_create_invalid();

    queue_push(&queue, event);

    Event result;

    bool success = queue_pop(&queue, &result);

    logger_test(logger, "Queue pop succeeds", success);

    logger_test(logger, "Pop removes event", queue_is_empty(&queue));

    logger_test(logger, "Pop from empty queue fails", !queue_pop(&queue, &result));
}

static void test_queue_fifo(Logger *logger)
{
    Queue queue = queue_create();

    Event first = event_create(timestamp_initialize(), TEMPERATURE_READING, (Entity){1, TEMPERATURE_SENSOR, 0});

    Event second = event_create(timestamp_initialize(), DOOR_OPENED, (Entity){2, DOOR, 0});

    queue_push(&queue, first);
    queue_push(&queue, second);

    Event result;

    queue_pop(&queue, &result);

    bool first_correct = result.type == TEMPERATURE_READING && result.entity.id == 1;

    queue_pop(&queue, &result);

    bool second_correct = result.type == DOOR_OPENED && result.entity.id == 2;

    logger_test(logger, "Queue preserves FIFO order", first_correct && second_correct);
}

static void test_queue_capacity(Logger *logger)
{
    Queue queue = queue_create();

    Event event = event_create_invalid();

    for (int i = 0; i < MAX_SIZE; i++)
    {
        queue_push(&queue, event);
    }

    logger_test(logger, "Queue reaches maximum capacity", queue_is_full(&queue) && queue_get_size(&queue) == MAX_SIZE);

    queue_push(&queue, event);

    logger_test(logger, "Queue rejects push when full", queue_get_size(&queue) == MAX_SIZE);
}

int main(void)
{
    Logger logger = logger_create("blackbox_c/recordings/tests/test_queue.txt");

    if (logger.file == NULL)
    {
        printf("Could not create test_queue.txt\n");
        return 1;
    }

    logger_section(&logger, "QUEUE TESTS");

    test_queue_create(&logger);
    test_queue_push(&logger);
    test_queue_peek(&logger);
    test_queue_pop(&logger);
    test_queue_fifo(&logger);
    test_queue_capacity(&logger);

    logger_section(&logger, "RESULT");

    fprintf(logger.file, "Passed: %d\n", logger.passed);

    fprintf(logger.file, "Failed: %d\n", logger.failed);

    logger_destroy(&logger);

    return logger.failed == 0 ? 0 : 1;
}