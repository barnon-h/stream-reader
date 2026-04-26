#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <time.h>

// Event flags
#define EVT_NONE 0x00          // 0000 0000
#define EVT_SCENE_CHANGE 0x01  // 0000 0001
#define EVT_BLACK_SCREEN 0x02  // 0000 0010
#define EVT_AUDIO_SPIKE 0x04   // 0000 0100
#define EVT_NO_LOGO 0x08       // 0000 1000
#define EVT_SILENCE 0x10       // 0001 0000

// Ring buffer size
#define RING_CAPACITY 16

// 3 possible stream states
typedef enum
{
    STATE_UNKNOWN,
    STATE_CONTENT,
    STATE_COMMERCIAL,
}Stream_state;

// Event structure
typedef struct
{
    uint8_t flag;
    uint32_t time_stamp;
}Event;

// Ring buffer structure
typedef struct
{
    Event buf[ RING_CAPACITY ];
    int head;
    int tail;
    int count;
}Ring_buffer;

// Ring buffer api
void ring_init( Ring_buffer *rb );              // Constructor
int ring_push( Ring_buffer *rb, Event evt );    // Add to ring
int ring_pop( Ring_buffer *rb, Event *out );    // Remove from ring
int ring_full( const Ring_buffer *rb );         // Check if ring is full
int ring_empty( const Ring_buffer *rb );        // Check if Ring is empty

// Event parser
int parse_event( const char *line, Event *out );

// Classifier
Stream_state classify( const Ring_buffer *rb );
const char *get_state_name( Stream_state s );

#endif