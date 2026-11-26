package com.footballlegends.mobile;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/** OpenGL callbacks and native scene controls run on the dedicated GL thread. */
public final class GameRenderer implements GLSurfaceView.Renderer {
    private static final String TAG = "FootballLegends";
    private long engineHandle;
    private int formation;
    private boolean cinematicCamera = true;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        if (engineHandle == 0L) {
            engineHandle = NativeBridge.nativeCreate();
            if (engineHandle == 0L) {
                Log.e(TAG, "Native game engine initialization failed.");
                return;
            }
        }
        NativeBridge.nativeSurfaceCreated(engineHandle);
        NativeBridge.nativeSetLobbyFormation(engineHandle, formation);
        NativeBridge.nativeSetCinematicCamera(engineHandle, cinematicCamera);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        if (engineHandle != 0L) NativeBridge.nativeSurfaceChanged(engineHandle, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        if (engineHandle != 0L) NativeBridge.nativeDraw(engineHandle);
    }

    void setFormation(int value) {
        formation = Math.max(0, Math.min(value, 3));
        if (engineHandle != 0L) NativeBridge.nativeSetLobbyFormation(engineHandle, formation);
    }

    void setCinematicCamera(boolean enabled) {
        cinematicCamera = enabled;
        if (engineHandle != 0L) NativeBridge.nativeSetCinematicCamera(engineHandle, enabled);
    }

    void releaseNativeEngine() {
        if (engineHandle != 0L) {
            NativeBridge.nativeDestroy(engineHandle);
            engineHandle = 0L;
        }
    }
}