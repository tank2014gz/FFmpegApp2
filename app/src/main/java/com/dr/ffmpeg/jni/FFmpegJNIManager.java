package com.dr.ffmpeg.jni;

import android.util.SparseArray;

/**
 * 项目名称：FFmpegApp2
 * 类描述：
 * 创建人：yuliyan
 * 创建时间：2020/2/14 10:20 AM
 * 修改人：yuliyan
 * 修改时间：2020/2/14 10:20 AM
 * 修改备注：
 */
public class FFmpegJNIManager {
    private JNILogInfoCallBack jniLogInfoCallBack;
    
    static {
        System.loadLibrary("native-lib");
    }
    
    
    public void init() {
        ;
    }
    
    
    public void setLogCallBack(JNILogInfoCallBack callBack) {
        this.jniLogInfoCallBack = callBack;
        
    }
    
    
    private void jniLogInfo(String message) {
        if (this.jniLogInfoCallBack != null) {
            this.jniLogInfoCallBack.logInfo(message);
        }
    }
    
    public native String testString();
    
    
    public native void open(String url, Object handle);
    
    public native void initFFmpeg(String path);
    
    public interface JNILogInfoCallBack {
        void logInfo(String message);
    }
}
