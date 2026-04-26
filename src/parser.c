#include "events.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int parse_event( const char *line, Event *out )
{
    // Guard against empty input
    if( !line || !out ) return -1;

    // Skip comments, newlines & empty lines
    if( line[ 0 ] == '#' || line[ 0 ] == '\n' || line[ 0 ] == '\0' ) return -1;

    // read lines into time stamps and flags
    uint32_t time_stamp = 0;
    char flags[ 128 ] = { 0 };

    if(  sscanf( line, "%u %127s", &time_stamp, flags) != 2 )
        return -1;

    out->time_stamp = time_stamp;
    out->flag = EVT_NONE;

    // Seperate flags
    char *token = strtok( flags, "|");
    while( token )
    {
        if( strcmp( token, "SCENE_CHANGE" ) == 0 )
            out->flag |= EVT_SCENE_CHANGE;

        else if( strcmp( token, "BLACK_FRAME" ) == 0 )
            out->flag |= EVT_BLACK_SCREEN;

        else if( strcmp( token, "AUDIO_SPIKE" ) == 0 )
            out->flag |= EVT_AUDIO_SPIKE;

        else if( strcmp( token, "LOGO_ABSENT" ) == 0 )
            out->flag |= EVT_NO_LOGO;

        else if( strcmp( token, "SILENCE") == 0 )
            out->flag |= EVT_SILENCE;

        else
        {
            fprintf( stderr, "Unknown flag : %s\n", token );
            return -1;
        }

        token = strtok( NULL, "|" );
    }

    return 0;
}