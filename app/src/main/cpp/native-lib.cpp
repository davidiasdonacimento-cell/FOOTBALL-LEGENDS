#include <jni.h>
#include <android/log.h>

#include <cstdint>

#include "football/core/GameEngine.hpp"

namespace {
constexpr char kLogTag[] = "FootballLegendsJNI";
football::GameEngine* fromHandle(jlong handle) {
    return reinterpret_cast<football::GameEngine*>(static_cast<std::uintptr_t>(handle));
}
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeCreate(JNIEnv*, jclass) {
    try {
        auto* engine = new football::GameEngine();
        __android_log_print(ANDROID_LOG_INFO,kLogTag,"Native game engine created.");
        return static_cast<jlong>(reinterpret_cast<std::uintptr_t>(engine));
    } catch (...) {
        __android_log_print(ANDROID_LOG_ERROR,kLogTag,"Failed to create native game engine.");
        return 0;
    }
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeSurfaceCreated(JNIEnv*,jclass,jlong handle) {
    if(auto* engine=fromHandle(handle)) engine->onSurfaceCreated();
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeSurfaceChanged(JNIEnv*,jclass,jlong handle,jint width,jint height) {
    if(auto* engine=fromHandle(handle)) engine->onSurfaceChanged(width,height);
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeDraw(JNIEnv*,jclass,jlong handle) {
    if(auto* engine=fromHandle(handle)) engine->renderFrame();
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeSetLobbyFormation(JNIEnv*,jclass,jlong handle,jint formation) {
    if(auto* engine=fromHandle(handle)) engine->setLobbyFormation(formation);
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeSetCinematicCamera(JNIEnv*,jclass,jlong handle,jboolean enabled) {
    if(auto* engine=fromHandle(handle)) engine->setCinematicCamera(enabled==JNI_TRUE);
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeDestroy(JNIEnv*,jclass,jlong handle) {
    delete fromHandle(handle);
    __android_log_print(ANDROID_LOG_INFO,kLogTag,"Native game engine destroyed.");
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeStartMatch(JNIEnv*, jclass, jlong handle) {
    if (auto* engine = fromHandle(handle)) engine->startMatch();
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativePauseMatch(JNIEnv*, jclass, jlong handle) {
    if (auto* engine = fromHandle(handle)) engine->pauseMatch();
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeResumeMatch(JNIEnv*, jclass, jlong handle) {
    if (auto* engine = fromHandle(handle)) engine->resumeMatch();
}
extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_NativeBridge_nativeResetMatch(JNIEnv*, jclass, jlong handle) {
    if (auto* engine = fromHandle(handle)) engine->resetMatch();
}
