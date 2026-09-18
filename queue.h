#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// Opaque structure declaration for the queue
typedef struct Queue Queue;

// Function declarations (Public API)
Queue* queue_create(void);
void queue_destroy(Queue *q);

void queue_enqueue(Queue *q, int value);
int queue_dequeue(Queue *q);
int queue_peek(Queue *q);

bool queue_is_empty(const Queue *q);
int queue_size(const Queue *q);

#endif // QUEUE_H
