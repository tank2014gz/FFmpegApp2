package com.dr.ffmpeg.jni;

/**
 * 项目名称：FFmpegApp2
 * 类描述：
 * 创建人：yuliyan
 * 创建时间：2020/2/14 10:06 PM
 * 修改人：yuliyan
 * 修改时间：2020/2/14 10:06 PM
 * 修改备注：
 */
public class FFmpegJNIManager1 implements JNIManager{
    
    static {
        System.loadLibrary("demo01-lib");
    }
    
    private JNILogInfoCallBack jniLogInfoCallBack;
    
    static {
        System.loadLibrary("demo01-lib");
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
    
    
}
