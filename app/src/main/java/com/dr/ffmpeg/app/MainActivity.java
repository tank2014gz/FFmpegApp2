package com.dr.ffmpeg.app;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.dr.ffmpeg.jni.FFmpegJNIManager;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        
        ListView listView = findViewById(R.id.listView);
        
        List<String> titles = new ArrayList<>();
        titles.add("初始化FFMPEG avcodec.so加载");
        titles.add("FFMPEG 视频解封装'");
        
        listView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, titles));
        
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                switch (position) {
                    case 0: {
                        startActivity(new Intent(MainActivity.this, Demo01Activity.class));
                        break;
                    }
                    case 1: {
                        startActivity(new Intent(MainActivity.this, Demo02Activity.class));
                        break;
                    }
                }
            }
        });
    }
    
}
