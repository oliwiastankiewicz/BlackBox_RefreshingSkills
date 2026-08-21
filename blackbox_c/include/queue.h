#include <stdbool.h>
#include "event.h"


#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 5

typedef struct{
    Event events[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;

Queue queue_create();

int queue_get_size(const Queue *queue);

bool queue_is_empty(const Queue *queue);

bool queue_is_full(const Queue *queue);

void queue_push(Queue *queue, Event event);

bool queue_pop(Queue *queue, Event *event);

bool queue_peek(const Queue *queue, Event *event);

void queue_print(const Queue *queue);

#endif