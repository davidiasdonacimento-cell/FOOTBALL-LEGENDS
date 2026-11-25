package com.footballlegends.mobile;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

final class GameRenderer implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("football_engine");
    }

    private static native void nativeSurfaceCreated();
    private static native void nativeSurfaceChanged(int width, int height);
    private static native void nativeDrawFrame();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame();
    }
}
