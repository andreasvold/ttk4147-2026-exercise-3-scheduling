#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

// Internal node structure for the linked list
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Internal queue structure tracking front, rear, and size
struct Queue {
    Node *front;
    Node *rear;
    int size;
};

// Initialize an empty queue
Queue* queue_create(void) {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "Memory allocation failed for Queue\n");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Push an item onto the back of the queue (FIFO)
void queue_enqueue(Queue *q, int value) {
    if (q == NULL) return;

    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed for Node\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = value;
    new_node->next = NULL;

    if (q->rear == NULL) {
        // Queue is empty, both front and rear point to the new node
        q->front = new_node;
        q->rear = new_node;
    } else {
        // Link the old rear node to the new node, then update rear
        q->rear->next = new_node;
        q->rear = new_node;
    }
    q->size++;
}

// Pop an item off the front of the queue
int queue_dequeue(Queue *q) {
    if (q == NULL || queue_is_empty(q)) {
        fprintf(stderr, "Queue Underflow: Cannot dequeue from an empty queue\n");
        exit(EXIT_FAILURE);
    }

    Node *temp = q->front;
    int value = temp->data;

    q->front = q->front->next;
    
    // If the queue becomes empty, set rear to NULL as well
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return value;
}

// View the front item without removing it
int queue_peek(const Queue *q) {
    if (q == NULL || queue_is_empty(q)) {
        fprintf(stderr, "Queue is empty: Nothing to peek\n");
        exit(EXIT_FAILURE);
    }
    return q->front->data;
}

// Check if empty
bool queue_is_empty(const Queue *q) {
    return (q == NULL || q->front == NULL);
}

// Get current item count
int queue_size(const Queue *q) {
    return q ? q->size : 0;
}

// Free all memory associated with the queue
void queue_destroy(Queue *q) {
    if (q == NULL) return;
    
    while (!queue_is_empty(q)) {
        queue_dequeue(q);
    }
    free(q);
}
