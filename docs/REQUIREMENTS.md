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
- Acesso à partida manual e ao modo árbitro IA x IA.
- Animações de entrada, seleção e transição.
- Retorno ao lobby sem reiniciar o aplicativo.

## 3. Personagem e recursos visuais
- Modelo humano 3D com rig esquelético compatível com Unity.
- Materiais e texturas aplicados corretamente.
- Animações de apresentação integradas ao Animator.
- Uniformes e elementos visuais originais ou licenciados.
- Recursos temporários devem ser identificados como temporários; não podem ser apresentados como assets finais.

## 4. Partida e arbitragem
- IA movimenta os atletas continuamente por funções e formação.
- Um goleiro por equipe, com posicionamento e tentativa de defesa.
- Bola com física básica, chute, passe e condução.
- Gols detectados ao cruzar a linha dentro da área válida e placar atualizado.
- Avaliação de impedimento no instante do passe, com linha virtual.
- Modo árbitro permite selecionar qualquer atleta, parar/retomar a partida, dar amarelo/vermelho sem motivo e decidir impedimento.
- Dois cartões amarelos geram expulsão.
- Atleta punido reclama com animação corporal, movimento de boca e fala visual na interface.

## 5. Interação
- Cada botão visível deve executar uma ação real.
- Estados de carregamento, indisponibilidade e erro devem ser comunicados.
- Toques não podem ficar bloqueados por painéis invisíveis.
- Navegação deve permitir voltar ao lobby sem reinicialização.

## 6. Estabilidade e dados
- Inicialização sem crash.
- Salvamento local versionado.
- Falha de leitura do save não pode impedir a abertura do lobby.
- Carregamento e descarregamento controlados de recursos.
- Logs úteis para diagnosticar falhas.

## 7. Critérios de aceitação
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

## 8. Pendências de produção
- IA tática avançada com estratégias, marcação coordenada e tomada de decisão contextual.
- Modelos humanos finais com malhas, UVs, texturas e rig profissional.
- Animações avançadas de corrida, passe, chute, defesa, queda e comemoração.
- Regras completas de arbitragem, cronômetro, substituições, faltas, laterais, escanteios e pênaltis.
- Testes de estabilidade e desempenho em aparelho Android real.
- Multiplayer online.
- Multiplayer online.
- Mercado, compras ou monetização.
- Licenciamento de clubes, ligas e atletas reais.

Esses recursos serão desenvolvidos e validados em etapas posteriores.
