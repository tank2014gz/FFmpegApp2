package com.dr.ffmpeg.app;

import android.view.View;
import android.widget.TextView;

import com.dr.ffmpeg.jni.FFmpegJNIManager1;
import com.dr.ffmpeg.jni.JNIManager;


public class Demo01Activity extends BaseActivity implements View.OnClickListener {
    
    
    @Override
    int getLayoutViewId() {
        return R.layout.activity_demo01;
    }
    
    @Override
    JNIManager getJNIManager() {
        return new FFmpegJNIManager1();
    }
    
    
    @Override
    void initView() {
        TextView infoView = findViewById(R.id.testInfo);
        infoView.setText(((FFmpegJNIManager1) manager).testString());
        findViewById(R.id.openFile).setOnClickListener(this);
        findViewById(R.id.openFile1).setOnClickListener(this);
    }
    
    
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.openFile: {
                ((FFmpegJNIManager1) manager).open("/sdcard/1080.mp4", this);
                break;
            }
            case R.id.openFile1: {
                ((FFmpegJNIManager1) manager).open("/sdcard/1085.mp4", this);
                break;
            }
        }
        
    }
}
