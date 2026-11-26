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

    void startMatch() { if (engineHandle != 0L) NativeBridge.nativeStartMatch(engineHandle); }

    void pauseMatch() { if (engineHandle != 0L) NativeBridge.nativePauseMatch(engineHandle); }

    void resumeMatch() { if (engineHandle != 0L) NativeBridge.nativeResumeMatch(engineHandle); }

    void resetMatch() { if (engineHandle != 0L) NativeBridge.nativeResetMatch(engineHandle); }


    void setControlRole(int role) { if (engineHandle != 0L) NativeBridge.nativeSetControlRole(engineHandle, role); }
    boolean selectPlayer(int playerId) { return engineHandle != 0L && NativeBridge.nativeSelectPlayer(engineHandle, playerId); }
    boolean movePlayer(float x, float y) { return engineHandle != 0L && NativeBridge.nativeMovePlayer(engineHandle, x, y); }
    boolean playerAction(int action, int target, float x, float y) { return engineHandle != 0L && NativeBridge.nativePlayerAction(engineHandle, action, target, x, y); }
    boolean refereeSendOff(int playerId) { return engineHandle != 0L && NativeBridge.nativeRefereeSendOff(engineHandle, playerId); }

    boolean refereeIssueCard(int playerId, int cardType) { return engineHandle != 0L && NativeBridge.nativeRefereeIssueCard(engineHandle, playerId, cardType); }
    boolean refereeCallFoul(int playerId) { return engineHandle != 0L && NativeBridge.nativeRefereeCallFoul(engineHandle, playerId); }
    boolean refereePlayAdvantage(int playerId) { return engineHandle != 0L && NativeBridge.nativeRefereePlayAdvantage(engineHandle, playerId); }
    boolean refereeCallOffside(int playerId) { return engineHandle != 0L && NativeBridge.nativeRefereeCallOffside(engineHandle, playerId); }
    boolean refereeRequestVar() { return engineHandle != 0L && NativeBridge.nativeRefereeRequestVar(engineHandle); }
    boolean refereeResolveVar(boolean uphold) { return engineHandle != 0L && NativeBridge.nativeRefereeResolveVar(engineHandle, uphold); }
    boolean refereeRestartPlay() { return engineHandle != 0L && NativeBridge.nativeRefereeRestartPlay(engineHandle); }
    void setMatchTimeScale(float scale) { if (engineHandle != 0L) NativeBridge.nativeSetMatchTimeScale(engineHandle, scale); }
    void releaseNativeEngine() {
        if (engineHandle != 0L) {
            NativeBridge.nativeDestroy(engineHandle);
            engineHandle = 0L;
        }
    }
}