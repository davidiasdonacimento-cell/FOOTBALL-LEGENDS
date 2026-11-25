package com.footballlegends.mobile;

/**
 * Stable Java-to-native boundary. All native methods are implemented in C++.
 */
public final class NativeBridge {
    static {
        System.loadLibrary("football_legends");
    }

    private NativeBridge() {
    }

    public static native long nativeCreate();
    public static native void nativeSurfaceCreated(long engineHandle);
    public static native void nativeSurfaceChanged(long engineHandle, int width, int height);
    public static native void nativeDraw(long engineHandle);
    public static native void nativeDestroy(long engineHandle);
}
