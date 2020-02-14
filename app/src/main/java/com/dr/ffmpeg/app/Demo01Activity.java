package com.dr.ffmpeg.app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.dr.ffmpeg.jni.FFmpegJNIManager;

import org.w3c.dom.Text;

public class Demo01Activity extends BaseActivity implements View.OnClickListener {
    
    
    
    @Override
    int getLayoutViewId() {
        return R.layout.activity_demo01;
    }
    
    
    @Override
    void initView() {
        TextView infoView = findViewById(R.id.testInfo);
        infoView.setText(manager.testString());
        findViewById(R.id.openFile).setOnClickListener(this);
        findViewById(R.id.openFile1).setOnClickListener(this);
    }
    
    
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.openFile: {
                manager.open("999999", 0);
                break;
            }
            case R.id.openFile1: {
                manager.open("/sdcard/1080.mp4", this);
            }
        }
        
    }
}
