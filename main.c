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

int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,
                       int bufsz){
    static AVPacket pkt;
    static uint8_t  *audio_pkt_data = NULL;
    static int      audio_pkt_sz    = 0;
    static AVFrame  frame;

    int len1, data_sz = 0;

    for(;;){
        while(audio_pkt_sz > 0){
            int got_frame = 0;
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame,
                                         &pkt);
            if(len1 < 0){
                audio_pkt_sz = 0;
                break;
            }
        }
    }
}

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

    int             audioStream;
    AVFormatContext *pFormatCtx    = NULL;
    AVCodecContext  *aCodecCtx     = NULL;
    AVCodec         *aCodec        = NULL;
    AVDictionary    *audioOptDict  = NULL;
    SDL_AudioSpec   wanted;

    if(avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0)
        return -1;
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1;

    av_dump_format(pFormatCtx, 0, argv[1], 0);

    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
        return -1;

    // Find Streams
    audioStream = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
           audioStream < 0){
            audioStream = i;
        }
    }
    if(audioStream == -1)
        return -1;

    aCodecCtx       = pFormatCtx->streams[audioStream]->codec;
    wanted.freq     = aCodecCtx->sample_rate;
    wanted.format   = AUDIO_S16SYS;
    wanted.channels = aCodecCtx->channels;
    wanted.silence  = 0;
    wanted.samples  = 1024;
    wanted.callback = fill_audio;
    wanted.userdata = aCodecCtx;

    if(SDL_OpenAudio(&wanted, NULL) < 0){
        fprintf(stderr, "SDL_OpenAudio: %s\n", SDL_GetError());
        return -1;
    }

    aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
    if(!aCodec){
        fprintf(stderr, "Unsupported Codec!\n");
        return -1;
    }

    avcodec_open2(aCodecCtx, aCodec, &audioOptDict);
    SDL_PauseAudio(0);


}
