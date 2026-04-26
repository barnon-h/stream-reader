/*
Implements The ring buffer api
*/

#include "events.h"
#include <string.h>

// Constructor
void ring_init( Ring_buffer *rb )
{
    // Set all entries to 0
    memset( rb, 0, sizeof( *rb ));
}

// Add item to ring
int ring_push( Ring_buffer *rb, Event evt )
{
    // If ring is full advance tail by one
    if ( ring_full( rb ))
    {
        rb->tail = ( rb->tail + 1 ) % RING_CAPACITY;
        rb->count--;
    }

    // Add item to top of the ring and make head point to it
    rb->buf[ rb->head ] = evt;
    rb->head = ( rb->head + 1 ) % RING_CAPACITY;

    rb->count++;
    return 0;
}

// Remove item from ring
int ring_pop( Ring_buffer *rb, Event *out )
{
    // return -1 if ring is empty
    if( ring_empty( rb ))
    {
        return -1;
    }

    // return last item in ring
    *out = rb->buf[ rb->tail ];

    // Remove item at tail
    rb->tail = ( rb->tail + 1 ) % RING_CAPACITY;
    rb->count--;

    return 0;
}

// Check if ring is full
int ring_full( const Ring_buffer *rb )
{
    // return true if ring count == ring_capacity
    return rb->count == RING_CAPACITY;
}

// Check if ring is empty
int ring_empty( const Ring_buffer *rb )
{
    // return true is ring count is 0
    return rb->count == 0;
}