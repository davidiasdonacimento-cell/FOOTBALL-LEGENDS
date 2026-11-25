package com.footballlegends.mobile;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public final class MainActivity extends Activity {
    private GameSurface gameSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        gameSurface = new GameSurface(this);
        setContentView(gameSurface);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (gameSurface != null) gameSurface.onResume();
    }

    @Override
    protected void onPause() {
        if (gameSurface != null) gameSurface.onPause();
        super.onPause();
    }
}
