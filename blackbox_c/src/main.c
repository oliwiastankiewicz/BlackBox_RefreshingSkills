#include <stdio.h>
#include "event.h"
#include "queue.h"
#include "timestamp.h"

int main()
{
    printf("BlackBox - Version 1\n\n");

    Timestamp clock = timestamp_initialize();

    timestamp_advance(&clock);

    Event event_test_1 = event_create(&clock, TEMPERATURE_READING, 0);

    timestamp_advance(&clock);
    Event event_test_2 = event_create(&clock, DOOR_OPENED, 1);

    timestamp_advance(&clock);
    Event event_test_3 = event_create(&clock, ALARM_TRIGGERED, 2);

    timestamp_advance(&clock);
    Event event_test_4 = event_create(&clock, DOOR_CLOSED, 3);

    timestamp_advance(&clock);
    Event event_test_5 = event_create(&clock, TEMPERATURE_READING, 4);

    timestamp_advance(&clock);
    Event event_test_6 = event_create(&clock, ALARM_CLEARED, 5);

    timestamp_advance(&clock);
    Event event_test_7 = event_create(&clock, TEMPERATURE_READING, 6);

    event_print(&event_test_1);
    printf("\n");

    Queue queue = queue_create();

    queue_push(&queue, event_test_1);
    queue_push(&queue, event_test_2);
    queue_push(&queue, event_test_3);
    queue_push(&queue, event_test_4);

    queue_print(&queue);
    printf("\n");

    Event popped_event;

    for (int i = 0; i < 2; i++)
    {
        if (queue_pop(&queue, &popped_event))
        {
            printf("Popped: ");
            event_print(&popped_event);
        }
    }
    printf("\n");

    queue_push(&queue, event_test_5);
    queue_push(&queue, event_test_6);
    queue_push(&queue, event_test_7);

    queue_print(&queue);
    printf("\n");

    queue_push(&queue, event_test_1);

    return 0;
}