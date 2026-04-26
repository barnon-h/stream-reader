#include "events.h"

// Tunable weights
#define W_SCENE_CHANGE 2
#define W_BLACK_SCREEN 3
#define W_NO_LOGO 1

#define COMMERCIAL_THRESHOLD 6


// returns state name given state
const char *get_state_name( Stream_state s )
{
    switch ( s )
    {
        case STATE_COMMERCIAL: return "COMMERCIAL";
        case STATE_CONTENT: return "CONTENT";

        default: return "UNKNOWN";
    }
}

// Calculate likelihood of being a commercial score
static int commercial_score( int scene_change, int black_screen, int no_logo)
{
    return ( scene_change * W_SCENE_CHANGE ) +
           ( black_screen * W_BLACK_SCREEN ) +
           ( no_logo * W_NO_LOGO );
}

// Calculate likelihood of being content score
static int content_score( int count, int scene_change, int no_logo, int audio_spike )
{
    // less scene changes and less scenes with no logo indicates content
    return ( count - scene_change ) + ( count - no_logo ) + audio_spike;
}


Stream_state classify( const Ring_buffer *rb )
{
    // return unknown state if ring is empty
    if( ring_empty( rb ))
    {
        return STATE_UNKNOWN;
    }

    int scene_change = 0;
    int black_screen = 0;
    int audio_spike = 0;
    int no_logo = 0;
    int silence = 0;

    int idx = rb->tail;
    int count = rb->count;

    // Slide window and count all different events in ring
    for( int i = 0; i < count; i ++ )
    {
        // Read the flag and increment events
        uint8_t f = rb->buf[ idx ].flag;

        if( f & EVT_SCENE_CHANGE ) scene_change++;
        if( f & EVT_BLACK_SCREEN ) black_screen++;

        if( f & EVT_AUDIO_SPIKE ) audio_spike++;
        if( f & EVT_NO_LOGO ) no_logo++;

        if( f & EVT_SILENCE ) silence++;

        idx = ( idx + 1 ) % RING_CAPACITY;
    }

    if( commercial_score( scene_change, black_screen, no_logo ) >= COMMERCIAL_THRESHOLD )
        return STATE_COMMERCIAL;

    if( content_score( count, scene_change, no_logo, audio_spike ) >= ( count * 2 - 2))
        return STATE_CONTENT;

    return STATE_UNKNOWN;
}