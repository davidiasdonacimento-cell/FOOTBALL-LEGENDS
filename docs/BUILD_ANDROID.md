# Compilar FOOTBALL LEGENDS MOBILE

## Ambiente
Android nativo com Java 17, Android SDK, Android NDK, CMake e Gradle. Não é necessário Unity nem Godot.

## Compilação local
Com SDK/NDK e Gradle instalados:
```bash
gradle :app:assembleDebug
```
APK esperado: `app/build/outputs/apk/debug/app-debug.apk`.

## GitHub Actions
O workflow `.github/workflows/android-apk.yml` compila o APK em cada push para `native-android-opengles` e por execução manual.
1. Abra Actions.
2. Selecione **Android APK — FOOTBALL LEGENDS MOBILE**.
3. Aguarde o job.
4. Baixe `FOOTBALL-LEGENDS-MOBILE-debug-apk` se o job terminar com sucesso.

## Limites
CI verifica compilação e empacotamento; não testa jogabilidade em aparelho físico. O APK precisa ser instalado e testado em Android ARM64. Não são necessárias credenciais Unity. Nunca coloque keystore, senha, token ou chave privada no Git.
