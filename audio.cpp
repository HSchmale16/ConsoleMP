/**\file   audio.cpp
 * \author Henry J Schmale
 * \date   April 4, 2015
 */

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <SDL/SDL.h>

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/opt.h>
}

static AVFormatContext *fmt_ctx;
static AVCodecContext  *dec_ctx;
static int             audio_stream_index = -1;

int open_input_file(const char *fname){
    int ret;
    AVCodec *dec;

    ret = avformat_open_input(&fmt_ctx, fname, NULL, NULL);
    if(ret < 0){
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    /* Find Audio Stream */
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, &dec, 0);
    if(ret < 0){
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }
    audio_stream_index = ret;
    dec_ctx = fmt_ctx->streams[audio_stream_index]->codec;
    av_opt_set_int(dec_ctx, "refcounted_frames", 1, 0);
    
    /* Init Decoder */
    ret = avcodec_open2(dec_ctx, dec, NULL);
    if(ret < 0){
        av_log(NULL, AV_LOG_ERROR, "Cannot open audio decoder\n");
        return ret;
    }
    return 0;
}

void initSoundSystem(){
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)){
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(0);
    }
}

void playFile(char *fname){
    int      ret;
    int      got_frame;
    AVPacket packet0, packet;
    AVFrame  *frame = av_frame_alloc();

    if(!frame){
        perror("Could not allocate frame!");
        exit(1);
    }

    av_register_all();

}

