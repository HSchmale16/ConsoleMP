/**\file   playback.c
 * \author Henry J Schmale
 * \date   April 6, 2015
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "playback.h"

static uint8_t  *audio_chunk;
static uint8_t  *audio_pos;
static uint32_t audio_len;
static bool     playbackInitd = false;
SDL_AudioSpec   wantedSpec;

// Audio Callback
void fill_audio(void *data, uint8_t *stream, int len){
    if(audio_len == 0)
        return;
    
    len = ( len > audio_len ? audio_len : len);
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

int initPlaybackSys(){
    if(!playbackInitd){
        if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)){
            fprintf(stderr, "Failed to init audio system\n");
            exit(0);
        }
        wantedSpec.freq     = 44100;
        wantedSpec.format   = AUDIO_S16SYS;
        wantedSpec.channels = 2;
        wantedSpec.samples  = 1024;
        wantedSpec.callback = fill_audio;
        wantedSpec.userdata = NULL;
        
        if(SDL_OpenAudio(&wantedSpec, NULL) < 0){
            fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
            return -1;
        }

        playbackInitd = true;
    }
}




