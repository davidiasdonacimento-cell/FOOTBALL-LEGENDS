# Arquitetura — FOOTBALL LEGENDS MOBILE

## Decisão técnica
A branch `native-android-opengles` usa aplicação Android nativa, C++17, Android NDK, CMake e OpenGL ES 3.0. Não usa Unity nem Godot.

## Camadas
- **Android:** Activity, ciclo de vida, janela, orientação e entrada de toque.
- **JNI:** fronteira pequena entre Java e C++.
- **Core:** estado da partida, relógio, fases, configuração, logs e persistência versionada.
- **Renderização:** contexto OpenGL ES 3.0, câmera 3D, projeção, materiais, malhas, iluminação e recursos GPU.
- **Gameplay:** simulação em passo fixo, atletas, bola, colisões, passe, chute, goleiros, regras, placar e IA tática.
- **Rede (futura):** cliente separado, protocolo versionado, autenticação, matchmaking, sincronização e reconexão.

## Organização
```text
app/
  build.gradle
  src/main/
    AndroidManifest.xml
    java/com/footballlegends/mobile/
      MainActivity.java
      GameSurface.java
      GameRenderer.java
    cpp/
      CMakeLists.txt
      native-lib.cpp
native/
  include/football/{core,gameplay,render}/
  src/{core,gameplay,render}/
docs/
.github/workflows/
```

## Inicialização e ciclo de vida
1. Android cria a Activity em paisagem.
2. A superfície OpenGL é criada.
3. JNI inicializa o motor nativo.
4. O motor configura estado e recursos gráficos.
5. O loop desenha os quadros.
6. Ao pausar ou perder contexto, recursos GPU são recriados com segurança.

## Simulação
A simulação usa passo fixo independente da taxa de quadros. A renderização interpola estados. Entrada de toque vira comandos de jogo e não altera diretamente estruturas internas da simulação.

## Modos
- **Offline:** partida local com IA.
- **Online:** servidor autoritativo valida relógio, bola, gols, faltas e resultados. O modo online permanece pendente até backend, protocolo, autenticação, matchmaking e testes estarem implementados.

## Desempenho
- Alvo inicial ARM64.
- Evitar alocações por quadro.
- Controlar resolução, sombras, materiais e quantidade de objetos.
- Carregar recursos sob demanda.
- Medir FPS, memória, temperatura e inicialização em aparelhos reais.
- Tratar perda e recriação do contexto OpenGL.

## Testes
Testes unitários da lógica C++, compilação Gradle/C++ no CI, ciclo de vida Android e testes físicos. Build verde não comprova jogabilidade nem estabilidade em todos os aparelhos.
