# FOOTBALL LEGENDS

Jogo de futebol 3D para Android, desenvolvido com **Unity 6**.

## Estado atual

- Fase: fundação técnica e lobby inicial.
- Plataforma-alvo: Android APK.
- Orientação: paisagem.
- Motor: Unity 6.6 (`6000.6.10f1`).
- Build: GitHub Actions com GameCI.
- Lobby inicial: cenário de estádio e personagem em blockout, navegação funcional entre painéis.

**Atenção:** o estádio e o personagem atuais são formas temporárias de blockout, não modelos 3D finais. Ainda não existe partida de futebol jogável. O APK ainda não foi compilado nem testado em aparelho.

## Documentação

- [Arquitetura](docs/ARCHITECTURE.md)
- [Requisitos e critérios de aceitação](docs/REQUIREMENTS.md)
- [Como configurar e gerar o APK](docs/BUILD_ANDROID.md)

## Estrutura

```text
Assets/
  Editor/
  Scenes/Lobby/
  Scripts/Lobby/
  Models/Players/
  Models/Stadium/
  Prefabs/
  Materials/
  Animations/
  Audio/
Packages/
ProjectSettings/
.github/workflows/
docs/
```

## Princípios técnicos

- Sistemas separados por responsabilidade.
- Interface adaptável a resoluções Android.
- Carregamento controlado de cenas e recursos.
- Nenhum botão decorativo: toda ação visível deve ter comportamento definido.
- Identidade visual, clubes e atletas originais ou devidamente licenciados.
- Credenciais e arquivos de assinatura não entram no Git.
