package com.dr.ffmpeg.app;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        
        ListView listView = findViewById(R.id.listView);
        
        List<String> titles = new ArrayList<>();
        titles.add("1 初始化FFMPEG avcodec.so加载");
        titles.add("2 FFMPEG 视频软解码 解封装");
        titles.add("3 FFMPEG 视频硬解码 解封装");
        titles.add("4 FFMPEG 视频软解码 解封装");
        titles.add("5 FFMPEG 视频硬解码 视频格式转换");
        
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
                    case 2: {
                        startActivity(new Intent(MainActivity.this, Demo03Activity.class));
                        break;
                    }
                    case 3: {
                        startActivity(new Intent(MainActivity.this, Demo04Activity.class));
                        break;
                    }
                    case 4: {
                        startActivity(new Intent(MainActivity.this, Demo05Activity.class));
                        break;
                    }
                }
            }
        });
    }
    
}
