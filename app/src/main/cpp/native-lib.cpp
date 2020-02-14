#include <jni.h>
#include <string>


#include "include/my_log.h"
//定义日志最大长度是256
#define MAX_LOG_MESSAGE_LENGTH 256

//最大数据缓冲区大小
#define MAX_BUFFER_SIZE 80
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
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


const char *className = "com/dr/ffmpeg/jni/FFmpegJNIManager";

extern "C" JNIEXPORT jstring JNICALL testString(
        JNIEnv *env,
        jobject thiz) {
    std::string hello = "Hello from C++";

//    const char *avcodec_info = avcodec_configuration();
//    hello += avcodec_info;
    return env->NewStringUTF(hello.c_str());
}




extern "C"
JNIEXPORT void JNICALL open(JNIEnv *env, jobject thiz, jstring _url, jobject jHandle) {

//    if (_url == nullptr || env == nullptr) {
//        logInfo(env, thiz, "url =null file  open fail ");
//        return;
//    }
//
//    const char *url = env->GetStringUTFChars(_url, nullptr);
//
//    FILE *fp = fopen(url, "rb");
//    if (!fp) {
//        logInfo(env, thiz, "%s file  open fail ", url);
//        LOGW("%s file  open fail ", url);
//    } else {
//        LOGI("%s file  open success ", url);
//        logInfo(env, thiz, "%s file  open success ", url);
//        fclose(fp);
//    }
//    env->ReleaseStringUTFChars(_url, url);

}

/**
 *
 */
extern "C"
JNIEXPORT void JNICALL initFFmpeg(JNIEnv *env, jobject thiz, jstring _path) {
    //初始化解封装(解封装器)
    av_register_all();
    //初始化网络
    avformat_network_init();

    //打开文件
    AVFormatContext *ic = NULL;
    const char *path1 = env->GetStringUTFChars(_path, 0);
    LOGW("path1=%s", path1);
    AVInputFormat *avInputFormat = NULL;
    try {
        int res = avformat_open_input(&ic, path1, avInputFormat, 0);

        if (0 == res) {
            logInfo(env, thiz, "avfomat_open_input %s success", path1);
            LOGW("avfomat_open_input %s success", path1);
        } else {
            logInfo(env, thiz, "%s avformat_open_input faild %s", path1, av_err2str(res));
            LOGW("avformat_open_input faild %s", av_err2str(res));
        }

    } catch (char *str) {
        logInfo(env, thiz, "avfomat_open_input %s fail", str);
    }
    env->ReleaseStringUTFChars(_path, path1);


}


static JNINativeMethod methods[] = {
        {"testString", "()Ljava/lang/String;",                    (void *) testString},
        {"initFFmpeg", "(Ljava/lang/String;)V",                   (void *) initFFmpeg},
        {"open",       "(Ljava/lang/String;Ljava/lang/Object;)V", (void *) open},
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
