/**\file   main.c
 * \brief  Console MP Entry Point
 * \author Henry J Schmale
 * \date   April 3, 2015
 *
 * A console based media player that randomly selects songs from a data
 * base to play. This is designed to be even lighter then MOCP, because
 * it lacks a complex UI, and runs very simplely
 */

#include <stdio.h>
#include <stdint.h>
#include <sqlite3.h>  // SQLite
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <SDL/SDL.h>

static uint8_t  *audio_chunk;
static uint32_t audio_len;
static uint8_t  *audio_pos;

void fill_audio(void *udata, uint8_t* stream, int len){
    if(audio_len == 0){
        return;
    }

    len = (len > audio_len ? audio_len : len);
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

int main(int argc, char* argv[]){
    av_register_all();
    
    int             audioStreams;
    AVFormatContext *pFormatCtx    = NULL;
    AVCodecContext  *aCodecContext = NULL;
    AVCodec         *aCodec        = NULL;
    SDL_AudioSpec   wanted;

    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
        return -1;
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1;

    av_dump_format(pFormatCtx, 0, argv[1], 0);

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
        return -1;
 
    // Find Streams
    audioStreams = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO 
           && audioStreams < 0){
            audioStreams = i;
        }
    }
    if(audioStreams == -1)
        return -1;

    
}
