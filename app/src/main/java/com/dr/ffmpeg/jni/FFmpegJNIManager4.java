package com.dr.ffmpeg.jni;

/**
 * 项目名称：FFmpegApp2
 * 类描述：
 * 创建人：yuliyan
 * 创建时间：2020/2/16 8:50 AM
 * 修改人：yuliyan
 * 修改时间：2020/2/16 8:50 AM
 * 修改备注：
 */
public class FFmpegJNIManager4 implements JNIManager{
    
    
    private JNILogInfoCallBack jniLogInfoCallBack;
    
    static {
        System.loadLibrary("demo04-lib");
    }
    
    
    private void jniLogInfo(String message) {
        if (this.jniLogInfoCallBack != null) {
            this.jniLogInfoCallBack.logInfo(message);
        }
    }
    
    
    public native void initFFmpeg(String path);
    
    
    @Override
    public void setLogCallBack(JNILogInfoCallBack jniLogInfoCallBack) {
        this.jniLogInfoCallBack = jniLogInfoCallBack;
    }
}
