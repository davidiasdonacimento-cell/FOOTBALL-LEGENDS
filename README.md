# FOOTBALL LEGENDS

Jogo de futebol 3D para Android, desenvolvido com **Unity 6**.

## Estado do projeto

- Fase atual: arquitetura e especificação.
- Plataforma-alvo: Android (APK).
- Orientação: paisagem.
- Interface: lobby original com apresentação cinematográfica, inspirado na organização visual de jogos modernos de futebol.
- Motor: Unity 6.
- Build planejado: GitHub Actions.

> O projeto ainda não contém uma cena jogável nem código de gameplay. A implementação começa após a revisão da arquitetura e dos requisitos documentados.

## Documentação

- [Arquitetura](docs/ARCHITECTURE.md)
- [Requisitos e critérios de aceitação](docs/REQUIREMENTS.md)

## Estrutura planejada

```
Assets/
  Animations/
  Audio/
  Materials/
  Models/
    Players/
    Stadium/
  Prefabs/
    Lobby/
    UI/
  Scenes/
    Lobby/
  Scripts/
    Core/
    Lobby/
    Players/
    UI/
  Settings/
Packages/
ProjectSettings/
.github/
  workflows/
docs/
```

## Princípios técnicos

- Sistemas separados por responsabilidade.
- UI adaptável a diferentes resoluções Android.
- Carregamento controlado de recursos e cenas.
- Salvamento local versionado e tolerante a falhas.
- Nenhum botão decorativo: toda ação visível deve ter comportamento definido.
- Identidade visual, clubes e atletas originais ou devidamente licenciados.
