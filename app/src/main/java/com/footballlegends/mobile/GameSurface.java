package com.footballlegends.mobile;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/**
 * Owns the OpenGL ES 3.0 surface and forwards lifecycle callbacks to C++.
 */
public final class GameSurface extends GLSurfaceView {
    private final GameRenderer gameRenderer;

    public GameSurface(Context context) {
        this(context, null);
    }

    public GameSurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        setPreserveEGLContextOnPause(false);
        gameRenderer = new GameRenderer();
        setRenderer(gameRenderer);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    public void release() {
        queueEvent(gameRenderer::releaseNativeEngine);
    }
}
