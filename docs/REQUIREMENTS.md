# Requisitos — FOOTBALL LEGENDS

## 1. Plataforma e apresentação
- Android APK.
- Unity 6.
- Orientação paisagem.
- Interface responsiva para diferentes resoluções e recortes de tela.
- Estilo de lobby esportivo cinematográfico, com identidade visual própria; não copiar logos, marcas ou telas protegidas de FIFA/eFootball.

## 2. Lobby
- Cenário de estádio 3D com gramado, arquibancadas e iluminação.
- Jogador 3D posicionado no lado esquerdo.
- Painéis de navegação no lado direito.
- Perfil e progresso acessíveis.
- Prévia do elenco acessível.
- Área de eventos e desafios acessível.
- Acesso rápido à experiência de partida, sem apresentar modo não implementado como funcional.
- Animações de entrada, seleção e transição.
- Retorno ao lobby sem reiniciar o aplicativo.

## 3. Personagem e recursos visuais
- Modelo humano 3D com rig esquelético compatível com Unity.
- Materiais e texturas aplicados corretamente.
- Animações de apresentação integradas ao Animator.
- Uniformes e elementos visuais originais ou licenciados.
- Recursos temporários devem ser identificados como temporários; não podem ser apresentados como assets finais.

## 4. Interação
- Cada botão visível deve executar uma ação real.
- Estados de carregamento, indisponibilidade e erro devem ser comunicados.
- Toques não podem ficar bloqueados por painéis invisíveis.
- Navegação deve permitir voltar ao lobby sem reinicialização.

## 5. Estabilidade e dados
- Inicialização sem crash.
- Salvamento local versionado.
- Falha de leitura do save não pode impedir a abertura do lobby.
- Carregamento e descarregamento controlados de recursos.
- Logs úteis para diagnosticar falhas.

## 6. Critérios de aceitação da primeira etapa
- [ ] Projeto Unity 6 abre sem erro.
- [ ] Build Android de teste é gerado pela automação.
- [ ] APK instala e abre em dispositivo Android compatível.
- [ ] Lobby aparece em paisagem.
- [ ] Estádio e personagem 3D são renderizados.
- [ ] Materiais e animações são exibidos corretamente.
- [ ] Botões de perfil, elenco, eventos e navegação respondem.
- [ ] Transições e retorno ao lobby funcionam.
- [ ] Não há crash durante o fluxo principal.
- [ ] Desempenho é medido em aparelho real, incluindo Redmi 12.

## 7. Fora do escopo da primeira etapa
- Partida completa de futebol.
- Física avançada da bola e dos jogadores.
- IA de adversários.
- Multiplayer online.
- Mercado, compras ou monetização.
- Licenciamento de clubes, ligas e atletas reais.

Esses recursos serão especificados separadamente antes de serem implementados.
