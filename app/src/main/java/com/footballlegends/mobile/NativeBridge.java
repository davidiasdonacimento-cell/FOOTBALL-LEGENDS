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
    public static native void nativeDestroy(long engineHandle);
}