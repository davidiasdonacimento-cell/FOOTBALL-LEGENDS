#include <jni.h>
#include "football/render/Renderer.hpp"

namespace {
football::render::Renderer renderer;
}

extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_GameRenderer_nativeSurfaceCreated(JNIEnv*, jclass) {
    renderer.onSurfaceCreated();
}

extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_GameRenderer_nativeSurfaceChanged(
    JNIEnv*, jclass, jint width, jint height) {
    renderer.onSurfaceChanged(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_footballlegends_mobile_GameRenderer_nativeDrawFrame(JNIEnv*, jclass) {
    renderer.drawFrame();
}
