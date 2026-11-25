package com.footballlegends.mobile;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;

/**
 * Android entry point. The game surface owns rendering; gameplay systems
 * will be introduced in later phases without putting game logic in this class.
 */
public final class MainActivity extends Activity {
    private GameSurface gameSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        gameSurface = new GameSurface(this);
        setContentView(gameSurface);
    }

    @Override
    protected void onPause() {
        if (gameSurface != null) {
            gameSurface.onPause();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (gameSurface != null) {
            gameSurface.onResume();
        }
    }

    @Override
    protected void onDestroy() {
        if (gameSurface != null) {
            gameSurface.release();
        }
        super.onDestroy();
    }
}
