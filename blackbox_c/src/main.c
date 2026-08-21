#include <stdio.h>
#include "event.h"
#include "queue.h"

int main(){
    printf("BlackBox - Version 1\n\n");

    Event event_test_1 = event_create("000001", TEMPERATURE_READING, 0);
    Event event_test_2 = event_create("000020", DOOR_OPENED, 1);
    Event event_test_3 = event_create("000034", ALARM_TRIGGERED, 2);
    Event event_test_4 = event_create("000155", DOOR_CLOSED, 3);
    Event event_test_5 = event_create("000201", TEMPERATURE_READING, 4);
    Event event_test_6 = event_create("000210", ALARM_CLEARED, 5);
    Event event_test_7 = event_create("000301", TEMPERATURE_READING, 6);
    
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

    for (int i=0; i<2; i++){
        if(queue_pop(&queue, &popped_event)){
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