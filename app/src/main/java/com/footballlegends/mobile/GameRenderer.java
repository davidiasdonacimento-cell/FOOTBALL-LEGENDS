package com.footballlegends.mobile;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * OpenGL callbacks run on GLSurfaceView's dedicated rendering thread.
 */
public final class GameRenderer implements GLSurfaceView.Renderer {
    private static final String TAG = "FootballLegends";
    private long engineHandle;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        if (engineHandle == 0L) {
            engineHandle = NativeBridge.nativeCreate();
            if (engineHandle == 0L) {
                Log.e(TAG, "Native engine initialization failed.");
                return;
            }
        }
        NativeBridge.nativeSurfaceCreated(engineHandle);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        if (engineHandle != 0L) {
            NativeBridge.nativeSurfaceChanged(engineHandle, width, height);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        if (engineHandle != 0L) {
            NativeBridge.nativeDraw(engineHandle);
        }
    }

    void releaseNativeEngine() {
        if (engineHandle != 0L) {
            NativeBridge.nativeDestroy(engineHandle);
            engineHandle = 0L;
        }
    }
}
