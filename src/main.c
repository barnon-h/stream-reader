#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "events.h"

#define MAX_LINE 256

static void process_stream( FILE *f )
{
    Ring_buffer rb;
    Stream_state last = STATE_UNKNOWN;
    char line[ MAX_LINE ];

    ring_init( &rb );

    while( fgets( line, sizeof( line ), f ))
    {
        line[ strcspn( line, "\n" )] = '\0';

        // Parse events from lines
        Event evt;
        if( parse_event( line, &evt ) != 0 ) continue;

        // push events into ring and classify
        ring_push( &rb, evt );
        Stream_state curr = classify( &rb );

        // log state change
        if( curr != last )
        {
            printf("[!] %s @ %6u ms\n", get_state_name( curr ), evt.time_stamp);
            last = curr;
        }
    }

    printf("[*] Final Classification : %s\n", get_state_name( last ));
}

int main( int argc, char *argv[] )
{
    // Exit at no argument
    if( argc < 2 )
    {
        fprintf( stderr, "[!] Usage : stream-reader <file>\n");
        return EXIT_FAILURE;
    }

    FILE *f = fopen( argv[ 1 ], "r" );

    // Exit at file not found
    if( !f )
    {
        printf("[!] stream-reader failed to open %s\n", argv[ 1 ]);
        return EXIT_FAILURE;
    }

    process_stream( f );

    fclose( f );
    return EXIT_SUCCESS;
}