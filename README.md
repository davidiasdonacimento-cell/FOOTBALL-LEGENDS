# FOOTBALL LEGENDS MOBILE

Jogo de futebol 3D para Android, com modos **offline e online**, desenvolvido em C++ e OpenGL ES — sem Unity e sem Godot.

## Plataforma e tecnologia
- Android APK, orientação paisagem.
- Aplicação Android nativa em Java, com renderização em C++.
- C++17 + Android NDK + CMake.
- OpenGL ES 3.0 para renderização 3D.
- Gradle para o projeto Android.
- GitHub Actions para validação e geração automatizada do APK.

## Estado desta branch
Branch ativa: `native-android-opengles`. Esta branch inicia a migração para uma arquitetura Android nativa. A primeira etapa estabelece a aplicação Android, a superfície OpenGL ES e os módulos básicos. O jogo completo e o multiplayer ainda não estão prontos.

## Arquitetura
- `app/src/main/java`: ciclo de vida Android e superfície de renderização.
- `app/src/main/cpp`: ponte JNI e motor gráfico nativo.
- `native/include`: contratos dos módulos C++.
- `native/src`: lógica de jogo independente da plataforma.
- `docs`: arquitetura, requisitos e processo de compilação.
- `.github/workflows`: automação de validação e APK.

## Modos planejados
- **Offline:** partida local contra IA.
- **Online:** partidas com servidor autoritativo, sincronização de estado e reconexão.
- O modo online só será considerado implementado depois de backend, autenticação, sincronização e testes reais.

## Segurança e qualidade
- Não armazenar senhas, tokens ou chaves no repositório.
- Não confiar no cliente para validar resultados de partidas online.
- Separar simulação, renderização, interface, persistência e rede.
- Nenhum botão deve ser apresentado como funcional sem ação implementada.
- Builds automatizados não substituem testes em aparelhos Android reais.

## Compilar
Consulte [docs/BUILD_ANDROID.md](docs/BUILD_ANDROID.md).

## Documentação
- [Arquitetura](docs/ARCHITECTURE.md)
- [Requisitos](docs/REQUIREMENTS.md)
- [Compilação Android](docs/BUILD_ANDROID.md)

Identidade original FOOTBALL LEGENDS MOBILE. Escudos, uniformes, clubes, nomes e atletas devem ser originais ou devidamente licenciados.
