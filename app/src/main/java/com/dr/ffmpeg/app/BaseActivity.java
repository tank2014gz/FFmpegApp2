package com.dr.ffmpeg.app;

import android.os.Bundle;
import android.widget.TextView;

import com.dr.ffmpeg.jni.JNILogInfoCallBack;
import com.dr.ffmpeg.jni.JNIManager;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

/**
 * 项目名称：FFmpegApp2
 * 类描述：
 * 创建人：yuliyan
 * 创建时间：2020/2/14 3:37 PM
 * 修改人：yuliyan
 * 修改时间：2020/2/14 3:37 PM
 * 修改备注：
 */
public abstract class BaseActivity extends AppCompatActivity implements JNILogInfoCallBack {
    
    abstract int getLayoutViewId();
    
    private TextView logView;
    
    JNIManager manager;
    
    
    abstract JNIManager getJNIManager();
    
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getLayoutViewId());
        
        manager = getJNIManager();
        manager.setLogCallBack(this);
        logView = findViewById(R.id.logView);
        initView();
        
    }
    
    
    abstract void initView();
    
    @Override
    public void logInfo(final String message) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (logView != null) {
                    logView.append(message);
                    logView.append("\n\n");
                }
            }
        });
        
    }
}
