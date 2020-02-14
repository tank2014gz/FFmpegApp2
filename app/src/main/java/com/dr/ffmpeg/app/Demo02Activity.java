package com.dr.ffmpeg.app;

import android.view.View;

import com.dr.ffmpeg.jni.FFmpegJNIManager;

public class Demo02Activity extends BaseActivity implements View.OnClickListener, FFmpegJNIManager.JNILogInfoCallBack {
    
    
    
    @Override
    int getLayoutViewId() {
        return R.layout.activity_demo02;
    }
    
    
    @Override
    void initView() {
        findViewById(R.id.initFFmpeg).setOnClickListener(this);
    }
    
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.initFFmpeg: {
                manager.initFFmpeg("/sdcard/1080.mp4");
            }
        }
    }
    
    
}
