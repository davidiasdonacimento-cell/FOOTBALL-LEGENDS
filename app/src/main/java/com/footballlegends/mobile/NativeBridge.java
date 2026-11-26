package com.footballlegends.mobile;

/** Stable Java-to-native boundary. Native calls are issued on the GL thread. */
public final class NativeBridge {
    static {
        System.loadLibrary("football_legends");
    }

    private NativeBridge() { }

    public static native long nativeCreate();
    public static native void nativeSurfaceCreated(long engineHandle);
    public static native void nativeSurfaceChanged(long engineHandle, int width, int height);
    public static native void nativeDraw(long engineHandle);
    public static native void nativeSetLobbyFormation(long engineHandle, int formation);
    public static native void nativeSetCinematicCamera(long engineHandle, boolean enabled);
    public static native void nativeStartMatch(long engineHandle);\n    public static native void nativePauseMatch(long engineHandle);\n    public static native void nativeResumeMatch(long engineHandle);\n    public static native void nativeResetMatch(long engineHandle);\n    public static native void nativeSetControlRole(long engineHandle, int role);
    public static native boolean nativeSelectPlayer(long engineHandle, int playerId);
    public static native boolean nativeMovePlayer(long engineHandle, float x, float y);
    public static native boolean nativePlayerAction(long engineHandle, int action, int targetPlayerId, float x, float y);
    public static native boolean nativeRefereeSendOff(long engineHandle, int playerId);
    public static native void nativeDestroy(long engineHandle);
}