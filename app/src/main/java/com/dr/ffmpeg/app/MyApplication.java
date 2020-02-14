package com.dr.ffmpeg.app;

import android.app.Application;

import com.dr.ffmpeg.jni.FFmpegJNIManager;

/**
 * 项目名称：FFmpegApp2
 * 类描述：
 * 创建人：yuliyan
 * 创建时间：2020/2/14 10:38 AM
 * 修改人：yuliyan
 * 修改时间：2020/2/14 10:38 AM
 * 修改备注：
 */
public class MyApplication extends Application {
    
    FFmpegJNIManager fFmpegJNIManager;
    
    @Override
    public void onCreate() {
        super.onCreate();
        fFmpegJNIManager = new FFmpegJNIManager();
        fFmpegJNIManager.init();
    }
    
    
    public FFmpegJNIManager getFFmpegJNIManager() {
        return fFmpegJNIManager;
    }
}
