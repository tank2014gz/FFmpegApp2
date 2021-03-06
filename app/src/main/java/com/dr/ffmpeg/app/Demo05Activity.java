package com.dr.ffmpeg.app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.dr.ffmpeg.jni.FFmpegJNIManager3;
import com.dr.ffmpeg.jni.FFmpegJNIManager5;
import com.dr.ffmpeg.jni.JNIManager;

public class Demo05Activity extends BaseActivity implements View.OnClickListener {
    
    
    @Override
    int getLayoutViewId() {
        return R.layout.activity_demo05;
    }
    
    @Override
    JNIManager getJNIManager() {
        return new FFmpegJNIManager5();
    }
    
    @Override
    void initView() {
        findViewById(R.id.openFile1).setOnClickListener(this);
        findViewById(R.id.openFile2).setOnClickListener(this);
        findViewById(R.id.openFile3).setOnClickListener(this);
    }
    
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.openFile1: {
                new Thread() {
                    @Override
                    public void run() {
                        super.run();
                        ((FFmpegJNIManager5) manager).initFFmpeg("/sdcard/1080.mp4");
                    }
                }.start();
                
                break;
            }
            case R.id.openFile2: {
                new Thread() {
                    @Override
                    public void run() {
                        super.run();
                        ((FFmpegJNIManager5) manager).initFFmpeg("/sdcard/1085.mp4");
                    }
                }.start();
                break;
            }
            case R.id.openFile3: {
                new Thread() {
                    @Override
                    public void run() {
                        ((FFmpegJNIManager5) manager).initFFmpeg("/sdcard/1084.flv");
                    }
                }.start();
                break;
            }
        }
    }
}
