package com.footballlegends.mobile;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

final class GameSurface extends GLSurfaceView {
    GameSurface(Context context) {
        super(context);
        setEGLContextClientVersion(3);
        setPreserveEGLContextOnPause(false);
        setRenderer(new GameRenderer());
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // The gameplay input module will map touch gestures to game commands.
        return true;
    }
}
