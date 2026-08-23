#include <stdio.h>
#include "queue.h"

Queue queue_create()
{
    Queue queue;

    queue.front = 0;
    queue.rear = 0;
    queue.size = 0;

    return queue;
}

int queue_get_size(const Queue *queue)
{
    return queue->size;
}

bool queue_is_empty(const Queue *queue)
{
    return (queue->size == 0);
}

bool queue_is_full(const Queue *queue)
{
    return (queue->size == MAX_SIZE);
}

void queue_push(Queue *queue, Event event)
{
    if (queue_is_full(queue))
    {
        printf("Queue is full!\n");
        return;
    }

    queue->events[queue->rear] = event;
    queue->rear = (queue->rear + 1) % MAX_SIZE;
    queue->size++;
}

bool queue_pop(Queue *queue, Event *event)
{
    if (queue_is_empty(queue))
    {
        return false;
    }

    queue_peek(queue, event);

    queue->front = (queue->front + 1) % MAX_SIZE;
    queue->size--;

    return true;
}

bool queue_peek(const Queue *queue, Event *event)
{
    if (queue_is_empty(queue))
    {
        return false;
    }

    *event = queue->events[queue->front];

    return true;
}

void queue_print(const Queue *queue)
{
    if (queue_is_empty(queue))
    {
        printf("Queue is empty!\n");
        return;
    }

    printf("Queue:\n");

    int i = queue->front;
    int j = 0;

    while (j < queue_get_size(queue))
    {
        event_print(&queue->events[i]);

        i = (i + 1) % MAX_SIZE;
        j++;
    }
}
