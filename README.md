# FOOTBALL LEGENDS

Jogo de futebol 3D para Android, desenvolvido com **Unity 6**.

## Estado atual

- Fase: lobby e primeira camada de gameplay/arbitragem.
- Plataforma-alvo: Android APK.
- Orientação: paisagem.
- Motor: Unity 6.6 (`6000.6.10f1`).
- Build: GitHub Actions com GameCI.
- Lobby: navegação funcional e seleção de partida manual ou modo árbitro IA x IA.
- Partida: IA tática inicial, goleiros procedurais, bola física, detecção de gols e placar.
- Arbitragem: seleção de qualquer atleta, pausa/retomada, cartões amarelos/vermelhos livres e decisão de impedimento.
- Animações procedurais básicas: corrida, movimento de cabeça e reclamação com boca/braços e fala na interface.

**Atenção:** a IA tática atual é inicial e baseada em regras; não representa ainda uma simulação profissional completa. Os atletas são modelos procedurais provisórios, não modelos 3D finais de produção. A compilação Unity e os testes no Android ainda estão pendentes; nenhum APK desta versão foi validado.

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
