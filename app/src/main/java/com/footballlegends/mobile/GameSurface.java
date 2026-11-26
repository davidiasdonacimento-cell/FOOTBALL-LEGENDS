package com.footballlegends.mobile;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/** Owns the ES 3 surface and serializes native scene changes on the GL thread. */
public final class GameSurface extends GLSurfaceView {
    private final GameRenderer gameRenderer;

    public GameSurface(Context context) { this(context, null); }

    public GameSurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setPreserveEGLContextOnPause(false);
        gameRenderer = new GameRenderer();
        setRenderer(gameRenderer);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    public void setFormation(int formation) {
        queueEvent(() -> gameRenderer.setFormation(formation));
    }

    public void setCinematicCamera(boolean enabled) {
        queueEvent(() -> gameRenderer.setCinematicCamera(enabled));
    }

    public void startMatch() { queueEvent(gameRenderer::startMatch); }
    public void pauseMatch() { queueEvent(gameRenderer::pauseMatch); }
    public void resumeMatch() { queueEvent(gameRenderer::resumeMatch); }
    public void resetMatch() { queueEvent(gameRenderer::resetMatch); }

    public void release() { queueEvent(gameRenderer::releaseNativeEngine); }
}