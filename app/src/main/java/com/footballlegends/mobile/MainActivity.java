package com.footballlegends.mobile;

import android.app.Activity;
import android.os.Bundle;
import android.view.WindowManager;
import android.widget.FrameLayout;

/** Android entry point for the native 3D club lobby. */
public final class MainActivity extends Activity {
    private GameSurface gameSurface;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        FrameLayout root = new FrameLayout(this);
        gameSurface = new GameSurface(this);
        root.addView(gameSurface, new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT));
        LobbyUi lobbyUi = new LobbyUi(this, gameSurface);
        root.addView(lobbyUi, new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT));
        setContentView(root);

        int formation = getSharedPreferences("football_legends_squad", MODE_PRIVATE)
                .getInt("formation", 0);
        boolean cinematic = getSharedPreferences("football_legends_settings", MODE_PRIVATE)
                .getBoolean("cinematic_camera", true);
        gameSurface.setFormation(formation);
        gameSurface.setCinematicCamera(cinematic);
    }

    @Override
    protected void onPause() {
        if (gameSurface != null) gameSurface.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (gameSurface != null) gameSurface.onResume();
    }

    @Override
    protected void onDestroy() {
        if (gameSurface != null) gameSurface.release();
        super.onDestroy();
    }
}