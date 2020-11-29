#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct _QueueItem {
    /*
     * The data of this item.
     */
    int data;
    /*
     * The next item in the queue.
     * NULL if there is no next item.
     */
    struct _QueueItem *next;
} QueueItem;

typedef struct _Queue {
    /*
     * The first item of the queue.
     * NULL if the queue is empty.
     */
    QueueItem *head;
    /*
     * The last item of the queue.
     * undefined if the queue is empty.
     */
    QueueItem *tail;
} Queue;

typedef enum _ProcessState {
    STATE_UNUSED = 0, // This entry in the _processes array is unused.

    STATE_READY,      // The process is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The process is running and should not be on a ready queue
    STATE_WAITING     // The process is blocked and should not be on a ready queue
} ProcessState;

typedef struct _Process {
    int processId;
    ProcessState state;
    /*
     * Range: 0 ... HIGHEST_PRIORITY (including)
     * HIGHEST_PRIORITY is highest priority
     */
    int priority;
} Process;

Process _processes[MAX_PROCESSES] = {{0}};
Queue _queues[6];
int counter = 0;

/* TODO: Add global variables if needed. */

/*
 * Adds a new, waiting process.
 */
int startProcess(int processId, int priority)
{
    if (((processId < 0) || (processId >= MAX_PROCESSES) ||
        (_processes[processId].state != STATE_UNUSED)) ||
        (priority < 0) || (priority > HIGHEST_PRIORITY)) {

        return -1;
    }

    _processes[processId].processId = processId;
    _processes[processId].state    = STATE_WAITING;
    _processes[processId].priority = priority;
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, int data)
{
    QueueItem *p = malloc(sizeof(QueueItem));
    assert(p != NULL);

    p->data = data;
    p->next = NULL;
    if (queue->tail != NULL){
        queue->tail->next = p;
        queue->tail = p;
    } else {
        queue->head = p;
        queue->tail = queue->head;
    }
}

/*
 * Remove and get the head of the queue.
 * Return -1 if the queue is empty.
 */
int _dequeue(Queue *queue)
{
    if (queue->head == NULL) {
        return -1;
    }
    
    QueueItem *p = queue->head;
    int data = p->data;

    queue->head = p->next;
    if (queue->tail == p) {
        queue->tail = queue->head;
    }

    free(p);
    return data;
}

void initScheduler()
{
    for (int i = 0; i <= HIGHEST_PRIORITY; i++) {
        _queues[i].head = NULL;
    }
}

/*
 * Called whenever a waiting process gets ready to run.
 */
void onProcessReady(int processId)
{
    assert(_processes[processId].state == STATE_WAITING);
    _processes[processId].state = STATE_READY;
    _enqueue(&_queues[_processes[processId].priority], processId);
}

/*
 * Called whenever a running process is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onProcessPreempted(int processId)
{
    assert(_processes[processId].state == STATE_READY ||
        _processes[processId].state == STATE_RUNNING);
    
    _processes[processId].state = STATE_RUNNING;
    _dequeue(&_queues[_processes[processId].priority]);
}

/*
 * Called whenever a running process is blocked (e.g., after requesting an I/O
 * operation) and needs to wait.
 */
void onProcessBlocked(int processId)
{
    assert(_processes[processId].state == STATE_RUNNING);
    
    _processes[processId].state = STATE_WAITING;
    _dequeue(&_queues[_processes[processId].priority]);
}

/*
 * Gets the id of the next process to run and sets its state to running.
 */
int scheduleNextProcess()
{
    
    return -1;
}
