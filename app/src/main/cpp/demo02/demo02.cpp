#include <jni.h>
#include <string>


#include "../include/my_log.h"
//定义日志最大长度是256
#define MAX_LOG_MESSAGE_LENGTH 256

//最大数据缓冲区大小
#define MAX_BUFFER_SIZE 80
extern "C" {
#include <libavformat/avformat.h>
}


//缓存日志的方法
static jmethodID methodID = NULL;

static void logInfo(JNIEnv *env, jobject _clazz, const char *format, ...) {

    if (NULL == methodID) {
        jclass clazz = env->GetObjectClass(_clazz);

        methodID = env->GetMethodID(clazz, "jniLogInfo", "(Ljava/lang/String;)V");

        env->DeleteLocalRef(clazz);
    }

    if (NULL != methodID) {
        //格式化日志消息
        char buffer[MAX_LOG_MESSAGE_LENGTH];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        //将缓冲区转换为Java字符窜
        jstring message = env->NewStringUTF(buffer);
        if (NULL != message) {
            //记录消息
            env->CallVoidMethod(_clazz, methodID, message);
            //释放消息引用
            env->DeleteLocalRef(message);
        }
    }

}

/**
 * 系统的 clock 函数在多线程环境下返回的时间有偏差
 * @return
 */
long long GetNowMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int sec = tv.tv_sec % 360000; //100个小时
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}

static double r2d(AVRational r) {
    return r.num == 0 | r.den == 0 ? 0.0 : ((double) r.num) / ((double) r.den);
}


const char *className = "com/dr/ffmpeg/jni/FFmpegJNIManager2";



/**
 *
 */
extern "C"
JNIEXPORT void JNICALL initFFmpeg(JNIEnv *env, jobject thiz, jstring _path) {
    //初始化解封装(解封装器)
    av_register_all();
    //初始化网络
    avformat_network_init();

    //初始化解码器
    avcodec_register_all();
    //打开文件
    AVFormatContext *ic = NULL;
    const char *path1 = env->GetStringUTFChars(_path, 0);
    LOGW("path1=%s", path1);
    AVInputFormat *avInputFormat = NULL;
    int res = avformat_open_input(&ic, path1, avInputFormat, 0);

    if (0 != res) {
        logInfo(env, thiz, "%s avformat_open_input faild %s", path1, av_err2str(res));
        LOGW("avformat_open_input faild %s", av_err2str(res));
        goto out;
    } else {
        LOGW("avfomat_open_input %s success", path1);
        logInfo(env, thiz, "avfomat_open_input %s success", path1);

        res = avformat_find_stream_info(ic, 0);

        if (res != 0) {
            LOGW("%savformat_find_stream_info fail", path1);
            logInfo(env, thiz, "%savformat_find_stream_info fail", path1);
            goto out;
        } else {
            LOGW("%s avformat_find_stream_info success", path1);
            logInfo(env, thiz, "%s avformat_find_stream_info success", path1);
        }
        LOGW("%s video file duration=%lld  nb_stream=%d", path1, ic->duration, ic->nb_streams);
        logInfo(env, thiz, "%s video file duration=%lld  nb_stream=%d", path1, ic->duration,
                ic->nb_streams);

        int fps = 0;
        int video_stream = 0;
        int audio_stream = 1;
        for (int i = 0; i < ic->nb_streams; ++i) {
            AVStream *as = ic->streams[i];
            //视频流
            if (as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                video_stream = i;
                fps = r2d(as->avg_frame_rate);
                LOGW("fps =%d,width=%d height=%d codecId=%d", fps,
                     as->codecpar->width,
                     as->codecpar->height,
                     as->codecpar->codec_id);
                logInfo(env, thiz, " 视频数据 stream[%d] fps =%d,width=%d height=%d codecId=%d", i, fps,
                        as->codecpar->width,
                        as->codecpar->height,
                        as->codecpar->codec_id);
            } else if (as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                audio_stream = i;
                LOGW("sample_rate =%d,channels=%d sample_format=%d codecId=%d",
                     as->codecpar->sample_rate,
                     as->codecpar->channels,
                     as->codecpar->format);
                logInfo(env, thiz,
                        "音频数据 stream[%d] sample_rate =%d,channels=%d sample_format=%d codecId=%d",
                        i,
                        as->codecpar->sample_rate,
                        as->codecpar->channels,
                        as->codecpar->format);
            }
        }

        //获取音频流信息
        audio_stream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
        LOGW("av_find_best_stream 获取音频流数据 audio_stream =%d", audio_stream);
        logInfo(env, thiz, "av_find_best_stream 获取音频流数据 audio_stream =%d", audio_stream);
//        video_stream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
//        LOGW("av_find_best_stream 获取视频流数据 video_stream =%d", video_stream);
//        logInfo(env, thiz, "av_find_best_stream 获取视频流数据 video_stream =%d", video_stream);

        //视频解码器
        //视频软解码
        AVCodec *codec = avcodec_find_decoder(ic->streams[video_stream]->codecpar->codec_id);
        //硬解码
//        codec = avcodec_find_decoder_by_name("h264_mediacodec");
        if (!codec) {
            LOGW("视频解码器查找失败！");
            logInfo(env, thiz, "视频解码器查找失败");
            goto out;
        }
        logInfo(env, thiz, "视频解码器查找成功");

        AVCodecContext *vc = avcodec_alloc_context3(codec);
        //复制stream中的参数复制到avcodecContext中
        avcodec_parameters_to_context(vc, ic->streams[video_stream]->codecpar);
        vc->thread_count = 8;
        logInfo(env, thiz, "视频解码器参数复制成功");

        //视频解码器初始化结束

        //打开解码器
        res = avcodec_open2(vc, 0, 0);
        if (0 != res) {
            //
            LOGW("avcodec_open2 打开视频解码器失败");
            logInfo(env, thiz, "avcodec_open2 打开视频解码器失败");
            goto out;
        }
        logInfo(env, thiz, "视频解码器打开成功");

        ////////////////////////////////////////////////////////
        //音频解码器初始化
        AVCodec *acodec = avcodec_find_decoder(ic->streams[audio_stream]->codecpar->codec_id);
        //硬解码
//         acodec=avcodec_find_decoder_by_name("");
        if (!acodec) {
            LOGW("音频解码器查找失败!");
            logInfo(env, thiz, "音频解码器查找失败");
            goto out;
        }
        logInfo(env, thiz, "音频解码器查找成功");
        //打开音频解码器a
        AVCodecContext *ac = avcodec_alloc_context3(acodec);
        //复制stream中的参数复制到avcodecContext中
        avcodec_parameters_to_context(ac, ic->streams[audio_stream]->codecpar);
        ac->thread_count = 8;
        logInfo(env, thiz, "音频解码器参数复制成功");
        //解码器初始化结束

        //打开视频解码器
        res = avcodec_open2(ac, 0, 0);
        if (0 != res) {
            //
            LOGW("avcodec_open2 打开音频解码器失败");
            logInfo(env, thiz, "avcodec_open2 打开音频解码器失败");
            goto out;
        }

        logInfo(env, thiz, "音频解码器打开成功");


        //读取帧数据
        //准备空间
        AVPacket *pkt = av_packet_alloc();
        AVFrame *frame = av_frame_alloc();

        long long start = GetNowMs();
        int frameCount = 0;
        for (;;) {
            //时间超过3秒
            if (GetNowMs() - start >= 3000) {
                LOGW("now decode fps is %d", frameCount / 3);
                logInfo(env, thiz, "now decode fps is %d", frameCount / 3);
                start = GetNowMs();
                frameCount = 0;

            }

            int re = av_read_frame(ic, pkt);


            if (re != 0) {
                LOGW("读取到结尾处！");
//                logInfo(env, thiz, "读取到结尾处!");
                av_seek_frame(ic, video_stream, ic->duration / 2,
                              AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
                continue;
            }

            //只测试视频的解码
            //判断视频
            if (pkt->stream_index != video_stream) {
                continue;
            }

            AVCodecContext *cc = vc;
            if (pkt->stream_index == audio_stream) {
                cc = ac;
                LOGW("current stream is audio");
            }


            int p = pkt->pts;
            re = avcodec_send_packet(cc, pkt);
            av_packet_unref(pkt);
            if (re != 0) {
                LOGW("avcodec_send_packet failed");
                continue;
            }


            for (;;) {
                re = avcodec_receive_frame(cc, frame);

                if (re != 0) {
                    LOGW("avcodec_receive_frame failed error=%s", av_err2str(re));
                    break;
                }
//                LOGW("avcodec_receive_frame %lld", frame->pts);
                //如果是视频帧
                if (cc == vc) {
                    frameCount++;

                }
            }

//            LOGW("stream =%d size=%d pts=%d,flag=%d",
//                 pkt->stream_index, pkt->size, pkt->pts, pkt->flags);
//            logInfo(env, thiz, "stream =%d size=%d pts=%d,flag=%d",
//                    pkt->stream_index, pkt->size, pkt->pts, pkt->flags);

//            av_packet_unref(pkt);
        }
    }


    out:

    env->ReleaseStringUTFChars(_path, path1);
    avformat_close_input(&ic);


}


static JNINativeMethod methods[] = {
        {"initFFmpeg", "(Ljava/lang/String;)V", (void *) initFFmpeg},
};

jint registerMethod(JNIEnv *env, jint version) {


    jclass clazz = env->FindClass(className);

    if (clazz == nullptr) {
        return JNI_ERR;
    }
    jint size = sizeof(methods) / sizeof(methods[0]);
    jint res = env->RegisterNatives(clazz, methods, size);
    if (res == 0) {
        return version;
    }
    return JNI_ERR;


}


void unRegisterMethod(JNIEnv *env, jint version) {
    jclass clazz = env->FindClass(className);
    if (className != nullptr) {
        env->UnregisterNatives(clazz);
    }
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = nullptr;

    jint res = vm->GetEnv((void **) &env, JNI_VERSION_1_4);
    if (res == 0) {
        return registerMethod(env, JNI_VERSION_1_4);
    } else if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) == 0) {
        return registerMethod(env, JNI_VERSION_1_6);
    }

    return JNI_ERR;


}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if ((vm->GetEnv((void **) &env, JNI_VERSION_1_4))) {
        unRegisterMethod(env, JNI_VERSION_1_4);
    } else if ((vm->GetEnv((void **) env, JNI_VERSION_1_6))) {
        unRegisterMethod(env, JNI_VERSION_1_6);
    }
}
