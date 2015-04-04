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

#define MAX_AUDIO_FRAME_SIZE 192000

static AVFormatContext *fmt_ctx;
static AVCodecContext  *dec_ctx;
static int             audio_stream_index = -1;
static int             quit = 0;
static SDL_AudioSpec   wanted, have;


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
    av_register_all();
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)){
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
        exit(0);
    }
}

int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size) {
    static AVPacket pkt;
    static uint8_t *audio_pkt_data = NULL;
    static int audio_pkt_size = 0;
    static AVFrame frame;
    int len1, data_size = 0;
    for(;;) {
        while(audio_pkt_size > 0) {
            int got_frame = 0;
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
            if(len1 < 0) {
                /* if error, skip frame */
                audio_pkt_size = 0;
                break;
            }
            audio_pkt_data += len1;
            audio_pkt_size -= len1;
            if (got_frame)
            {
                data_size =
                av_samples_get_buffer_size
                (
                NULL,
                aCodecCtx->channels,
                frame.nb_samples,
                aCodecCtx->sample_fmt,
                1
                );
                memcpy(audio_buf, frame.data[0], data_size);
            }
            if(data_size <= 0) {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if(pkt.data)
            av_free_packet(&pkt);
            if(quit) {
                return -1;
            }
            audio_pkt_data = pkt.data;
            audio_pkt_size = pkt.size;
    }
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
    AVCodecContext *aCodecCtx = (AVCodecContext *)userdata;
    int len1, audio_size;
    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;
    while(len > 0) {
        if(audio_buf_index >= audio_buf_size) {
            /* We have already sent all our data; get more */
            audio_size = audio_decode_frame(aCodecCtx, audio_buf, audio_buf_size);
            if(audio_size < 0) {
                /* If error, output silence */
                audio_buf_size = 1024; // arbitrary?
                memset(audio_buf, 0, audio_buf_size);
            } else {
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if(len1 > len)
            len1 = len;
            memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
            len -= len1;
            stream += len1;
            audio_buf_index += len1;
    }
}

int playFile(const char *fname){
    int      ret;
    int      got_frame;
    AVPacket packet0, packet;
    AVFrame  *frame = av_frame_alloc();

    if(!frame){
        perror("Could not allocate frame!");
        exit(1);
    }
    
    ret = open_input_file(fname);
    if(ret < 0)
        goto end; // We fucked up opening the file

    wanted.freq     = dec_ctx->sample_rate;
    wanted.format   = AUDIO_U16SYS;
    wanted.channels = dec_ctx->channels;
    wanted.silence  = 0;
    wanted.samples  = dec_ctx->sample_rate / 10;
    wanted.callback = audio_callback;
    wanted.userdata = dec_ctx;

    /* read packets */
  /*  packet0.data = NULL;
    packet.data  = NULL;
    for(;;){
        if(!packet0.data){
            ret = av_read_frame(fmt_ctx, &packet);
            if(ret < 0){
                break;
            }
            packet0 = packet;
        }

        if(packet.stream_index == audio_stream_index){
            got_frame = 0;
            ret = avcodec_decode_audio4(dec_ctx, frame, &got_frame, &packet);
            if(ret < 0){
                av_log(NULL, AV_LOG_ERROR, "Error decoding audio\n");
                continue;
            }

            packet.size -= ret;
            packet.data += ret;

            if(got_frame){
            }
        }
    }*/

end:
    avcodec_close(dec_ctx);
    av_frame_free(&frame);
    avformat_close_input(&fmt_ctx);

    if(ret < 0 && ret != AVERROR_EOF){
//        fprintf(stderr, "Error occured: %s\n", av_err2str(ret));
        exit(1);
    }
    return 0;
}

