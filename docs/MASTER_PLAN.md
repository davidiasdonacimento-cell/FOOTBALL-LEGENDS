# FOOTBALL LEGENDS MOBILE
## PLANO MESTRE DE DESENVOLVIMENTO — DOCUMENTO OFICIAL

**Projeto:** FOOTBALL LEGENDS MOBILE  
**Plataforma:** Android  
**Arquitetura:** Android nativo + C++17 + JNI + OpenGL ES 3.0  
**Build:** Gradle + Android NDK + CMake  
**Automação:** GitHub Actions  
**Modos:** Offline e Online

# 1. OBJETIVO

Desenvolver um jogo de futebol mobile 3D completo, com identidade própria, jogabilidade real, apresentação profissional, estabilidade e arquitetura modular.

A qualidade visual e de apresentação deverá ser inspirada nos principais jogos de futebol mobile, tendo o eFootball como referência de qualidade, sem copiar sua marca, interface ou conteúdo protegido.

A prioridade é entregar sistemas realmente funcionais, não apenas telas demonstrativas.

# 2. TECNOLOGIAS

- Java para a camada Android.
- C++17 para sistemas nativos.
- JNI para comunicação entre Java e C++.
- OpenGL ES 3.0 para renderização 3D.
- Android NDK.
- CMake.
- Gradle.
- JSON.
- SQLite ou armazenamento local.
- GitHub Actions.

# 3. ARQUITETURA

Organizar o projeto em módulos:

- Android e inicialização.
- Interface e navegação.
- Contas e perfis.
- Elencos e jogadores.
- Motor de jogo.
- Renderização 3D.
- Física.
- Gameplay.
- Futebol e arbitragem.
- Inteligência artificial.
- Animações.
- Câmeras.
- Áudio.
- Estádios.
- Competições.
- Carreira.
- Economia e progressão.
- Multiplayer.
- Salvamento.
- Testes e documentação.

Não concentrar todos os sistemas em um único arquivo.

# 4. INICIALIZAÇÃO

- Splash screen profissional.
- Logo oficial.
- Animação de entrada.
- Carregamento real dos recursos.
- Indicador de progresso.
- Tratamento de falhas.
- Transição para o lobby.
- Nunca iniciar uma partida automaticamente.

# 5. LOBBY 3D

- Ambiente 3D.
- Estádio como cenário.
- Câmera cinematográfica.
- Jogadores com modelos e animações.
- Iluminação.
- Interface moderna.
- Navegação funcional.
- Perfil.
- Elenco.
- Configurações.
- Acesso aos modos de jogo.

Todos os menus deverão executar ações reais.

# 6. CONTAS E PERFIS

- Perfil local.
- Identificação da conta.
- Configurações individuais.
- Proteção de credenciais.
- Persistência de dados.
- Sincronização online quando houver backend funcional.
- Isolamento dos dados por conta.
- Recuperação de dados quando possível.

O modo offline deverá funcionar sem conexão.

# 7. ELENCOS E TÁTICAS

- Cadastro e carregamento de jogadores.
- Atributos individuais.
- Posições.
- Formação.
- Titulares.
- Reservas.
- Substituições.
- Estratégias.
- Táticas ofensivas e defensivas.
- Funções individuais.

As alterações táticas deverão influenciar o comportamento real dos jogadores.

# 8. MOTOR DE PARTIDA

- Estados da partida.
- Início e encerramento.
- Onze jogadores por equipe.
- Bola.
- Árbitros.
- Placar.
- Cronômetro.
- Intervalo.
- Acréscimos.
- Prorrogação.
- Disputa de pênaltis.
- Eventos.
- Registro do resultado.

# 9. FÍSICA E GAMEPLAY

- Movimentação dos jogadores.
- Controle da bola.
- Passes.
- Chutes.
- Dribles.
- Domínio.
- Desarmes.
- Defesa.
- Goleiros.
- Colisões.
- Rebotes.
- Bolas paradas.
- Faltas.
- Escanteios.
- Laterais.
- Tiros de meta.

A física deverá produzir resultados coerentes e integrados ao gameplay.

# 10. FUTEBOL PROFISSIONAL E ARBITRAGEM

- Impedimento.
- Faltas.
- Cartões.
- Arbitragem.
- Pênaltis.
- Vantagem.
- VAR.
- Tecnologia de linha do gol.
- Bolas paradas.

O VAR deverá revisar somente situações elegíveis.

# 11. INTELIGÊNCIA ARTIFICIAL

- IA individual.
- IA de equipe.
- IA ofensiva.
- IA defensiva.
- IA de goleiros.
- Posicionamento.
- Marcação.
- Cobertura.
- Movimentação sem bola.
- Comportamento dos adversários.
- Níveis de dificuldade.
- Interpretação das táticas.

A IA deverá reagir ao estado real da partida.

# 12. PERSONAGENS E ANIMAÇÕES

- Modelos 3D.
- Esqueletos e rigging.
- Animações de corrida.
- Animações de passe.
- Animações de chute.
- Animações de defesa.
- Animações de goleiros.
- Transições.
- Mistura de animações.
- LOD.

# 13. CÂMERAS, ESTÁDIOS E ÁUDIO

- Câmeras de partida.
- Câmeras cinematográficas.
- Estádios 3D.
- Gramado.
- Arquibancadas.
- Torcida.
- Iluminação.
- Áudio da partida.
- Narração, quando houver recursos disponíveis.

# 14. MODOS DE JOGO

- Partida rápida.
- Modo carreira.
- Ligas.
- Torneios.
- Treinamento.
- Disputa de pênaltis.
- Multiplayer online.
- Multiplayer local, se tecnicamente suportado.

Cada modo deverá possuir fluxo de navegação, regras e integração com o motor de partida.

# 15. CARREIRA E PROGRESSÃO

- Evolução de jogadores.
- Progressão.
- Mercado.
- Transferências.
- Gerenciamento de elenco.
- Competições.
- Temporadas.
- Conquistas.
- Recompensas.
- Personalização.

# 16. CONTROLES E HUD

- Direcional touchscreen.
- Passe.
- Chute.
- Corrida.
- Drible.
- Defesa.
- Troca de jogador.
- Controles de goleiro.
- Placar.
- Cronômetro.
- Informações da partida.

Todos os controles deverão funcionar de verdade.

# 17. REPLAY E ESTATÍSTICAS

- Replays.
- Eventos importantes.
- Estatísticas individuais.
- Estatísticas das equipes.
- Histórico de partidas.
- Resultados.
- Desempenho.

# 18. DESEMPENHO MOBILE

- LOD.
- Culling.
- Instancing.
- Cache.
- Carregamento assíncrono.
- Controle de memória.
- Qualidade gráfica baixa, média e alta.
- Controle de FPS.
- Recuperação de contexto gráfico.
- Compatibilidade com dispositivos Android.

# 19. SALVAMENTO E PROTEÇÃO

- Sistema de save versionado.
- Validação de dados.
- Backup.
- Recuperação.
- Proteção contra corrupção.
- Isolamento por perfil.
- Persistência do progresso.
- Tratamento de falhas.

# 20. MULTIPLAYER E BACKEND

- Autenticação.
- Servidores.
- Matchmaking.
- Sincronização.
- Ranking.
- Proteção contra dados inválidos.
- Tratamento de desconexões.
- Reconexão.
- Validação de resultados.

Nenhum recurso online será declarado concluído sem teste real com backend funcional.

# 21. TESTES E QUALIDADE

Criar testes para:

- Sistemas nativos.
- Integração Java/C++.
- Física.
- Gameplay.
- Arbitragem.
- IA.
- Salvamento.
- Desempenho.
- Inicialização.
- Estabilidade.

Corrigir erros críticos antes de avançar.

# 22. AUTOMAÇÃO E COMPILAÇÃO

Utilizar GitHub Actions para:

- Compilação Android.
- Testes nativos.
- Verificação de qualidade.
- Compilação Release.

Assinaturas deverão utilizar secrets protegidos.

Um APK só poderá ser declarado compilado quando houver resultado real de build. Um APK só poderá ser declarado testado quando tiver sido executado e verificado.

# 23. ORDEM OBRIGATÓRIA DE DESENVOLVIMENTO

**FASE 1 — Fundação:** motor, Android, C++, JNI, renderização e inicialização.

**FASE 2 — Lobby:** interface, navegação, perfil, elenco e configurações.

**FASE 3 — Campo:** estádio, jogadores, bola, câmera e controles.

**FASE 4 — Gameplay:** passes, chutes, dribles, defesa e goleiros.

**FASE 5 — Futebol profissional:** impedimento, faltas, arbitragem, cartões, VAR e bolas paradas.

**FASE 6 — Inteligência artificial:** jogadores, equipes, adversários e táticas.

**FASE 7 — Conteúdo:** times, jogadores, uniformes, estádios e animações.

**FASE 8 — Modos:** partida rápida, carreira, ligas, torneios e treinamento.

**FASE 9 — Online:** contas, servidores, matchmaking e ranking.

**FASE 10 — Finalização:** otimização, testes, correções e compilação Release.

Não avançar ignorando erros críticos de fases anteriores.

# 24. REGRAS DE IMPLEMENTAÇÃO NO REPOSITÓRIO

Antes de modificar:

1. Inspecionar o repositório inteiro.
2. Identificar os arquivos existentes.
3. Verificar o estado atual do projeto.
4. Identificar funcionalidades que já funcionam.
5. Verificar dependências.
6. Planejar as alterações.
7. Criar backup ou utilizar commits seguros.

Durante a implementação:

- Preservar funcionalidades existentes.
- Não apagar sistemas funcionais.
- Não substituir arquivos sem verificar seu conteúdo.
- Não duplicar módulos.
- Não deixar referências quebradas.
- Não adicionar bibliotecas incompatíveis.
- Não esconder erros de compilação.
- Implementar integração real entre os sistemas.

Se uma funcionalidade depender de recursos ainda inexistentes, criar uma implementação progressiva e documentar a dependência.

# 25. CRITÉRIOS DE ACEITAÇÃO

O projeto somente poderá ser considerado funcional quando:

- O aplicativo abrir sem crash.
- O lobby aparecer primeiro.
- Os menus funcionarem.
- A partida iniciar pelo botão correto.
- O campo 3D renderizar corretamente.
- Os jogadores se movimentarem.
- A bola possuir física funcional.
- Os passes e chutes funcionarem.
- Os goleiros defenderem.
- A IA agir conforme as táticas.
- O impedimento ser calculado corretamente.
- A arbitragem detectar infrações.
- O VAR revisar situações elegíveis.
- O placar e cronômetro funcionarem.
- O resultado da partida for registrado.
- O progresso for salvo.
- O APK for compilado e testado.

Recursos online somente poderão ser considerados concluídos depois de testados com backend funcional.

# 26. ENTREGA DE CADA FASE

Ao concluir cada fase, apresentar:

1. Arquivos criados.
2. Arquivos modificados.
3. Sistemas implementados.
4. Sistemas ainda pendentes.
5. Testes executados.
6. Resultado real dos testes.
7. Resultado real da compilação.
8. Erros encontrados.
9. Próximas etapas.

Não afirmar que algo foi implementado sem verificar o código.

Não afirmar que o APK foi compilado sem possuir resultado de compilação.

Não considerar arquivos vazios como sistemas concluídos.

# 27. REGRA PERMANENTE DE CONSULTA DO PLANO

ANTES DE CADA ALTERAÇÃO NO REPOSITÓRIO:

1. Consultar o documento mais recente do plano mestre.
2. Consultar o estado atual do GitHub e da branch de trabalho.
3. Comparar os arquivos reais com o plano.
4. Identificar o que já foi comprovadamente concluído.
5. Remover da lista de pendências somente os itens cuja conclusão foi confirmada por evidências.
6. Manter histórico do que foi concluído.
7. Atualizar o plano com as novas pendências.
8. Não repetir implementações já concluídas.
9. Não tratar código existente como funcional sem testes.
10. Não iniciar alterações sem autorização do usuário.

Se o documento atualizado não estiver disponível, solicitar sua versão mais recente em vez de inventar o conteúdo.

# 28. CONTROLE DE STATUS

Utilizar estes estados:

- **PENDENTE:** ainda não implementado.
- **EM DESENVOLVIMENTO:** implementação iniciada.
- **IMPLEMENTADO:** código criado e integrado, mas ainda sem validação completa.
- **TESTADO:** testes executados e aprovados.
- **CONCLUÍDO:** critérios de aceitação atendidos e evidências verificadas.
- **BLOQUEADO:** depende de recurso, correção ou decisão externa.

Somente itens com evidências suficientes poderão sair da lista de pendências.

# 29. ESTADO INICIAL CONHECIDO DO PROJETO

**Repositório:** `davidiasdonacimento-cell/FOOTBALL-LEGENDS`

**Branch de trabalho conhecida:** `native-android-opengles`

Existe uma estrutura inicial Android nativa com Java, C++, JNI, OpenGL ES, CMake, Gradle e workflow de compilação.

Documentação inicial e arquivos de fundação foram criados anteriormente.

A existência desses arquivos não comprova que o aplicativo esteja funcional. O renderizador inicial não equivale a uma partida jogável. Não existe confirmação de APK final compilado e testado.

A branch original `main` deve ser preservada. Não modificar a branch original sem autorização específica.

# 30. RESULTADO FINAL ESPERADO

FOOTBALL LEGENDS deverá ser um jogo de futebol mobile 3D completo, com:

- Lobby profissional.
- Jogadores inteligentes.
- Física de bola.
- Gameplay fluido.
- Impedimento.
- Arbitragem.
- VAR.
- Campeonatos.
- Carreira.
- Personalização.
- Multiplayer.
- Interface moderna.
- Estabilidade.
- Desempenho adequado para dispositivos Android.

**PRIORIDADE ABSOLUTA: JOGABILIDADE REAL, ESTABILIDADE, QUALIDADE VISUAL, ARQUITETURA PROFISSIONAL E FUNCIONALIDADE DE TODOS OS SISTEMAS.**

**REGRA FINAL:** nunca declarar sucesso sem evidência. Nunca marcar como concluído apenas porque um arquivo foi criado. O objetivo é entregar um jogo funcional, não apenas uma estrutura de projeto.


# 31. REGISTRO DE IMPLEMENTAÇÃO — 2026-11-26

## FASE 1 — FUNDAÇÃO
- **Status:** TESTADO quanto à compilação de CI.
- Java, JNI, C++17, OpenGL ES 3.0, CMake, Gradle e workflow Android estão integrados.
- Execução GitHub Actions: `45725032103` — conclusão `success`.
- A aprovação da compilação não comprova, por si só, execução e estabilidade em aparelho físico.

## FASE 2 — LOBBY
- **Status geral:** EM DESENVOLVIMENTO.
- Commit de implementação: `3c2a0915b4c63a52d4f38d6f6afa4f8d5a786fc8`.
- Ambiente de estádio 3D procedural integrado ao renderizador nativo OpenGL ES.
- Campo de apresentação configurado em 110 x 72 metros.
- Arquibancadas, cobertura, refletores, placar, marcações e traves adicionados à cena.
- Onze modelos procedurais de jogadores, com volumes anatômicos arredondados, uniformes e movimento de repouso.
- Quatro formações selecionáveis: 4-3-3, 4-4-2, 3-5-2 e 4-2-3-1. A escolha é salva localmente e reposiciona os modelos no cenário.
- Interface Android com navegação entre início, modos, elenco, perfil e configurações.
- Perfil local com nome persistido no dispositivo.
- Configuração de câmera cinematográfica persistida e aplicada ao renderizador.
- Os cartões de modos informam corretamente que as partidas e os demais modos ainda dependem das fases de gameplay e modos; não simulam partidas funcionais.
- Compilação GitHub Actions: execução `45726314758` — conclusão `success`.
- Artefato Debug publicado: `football-legends-foundation-debug`, ID `15074835805`.
- **Não existe ainda validação visual ou teste de execução em aparelho Android nesta fase.**
- Pendente: inspeção visual no dispositivo, testes de navegação e persistência em execução real, evolução do gerenciamento do elenco, animações esqueléticas completas, materiais/texturas e integração futura dos modos com o motor de partida.
- Não marcar a FASE 2 como CONCLUÍDA até que os testes em dispositivo e os critérios correspondentes sejam verificados.

## REGRA DE EVIDÊNCIA
Uma compilação CI aprovada comprova que o projeto compilou e que o artefato foi gerado. Ela não comprova, isoladamente, qualidade visual, ausência de crash em aparelho físico ou funcionamento de recursos que ainda dependem de fases posteriores.


# 32. REQUISITOS ADICIONAIS DE GAMEPLAY E ARBITRAGEM — 2026-11-26

Solicitação do usuário para integrar às fases de gameplay, futebol profissional, IA, animações e modos:

## Arbitragem configurável
- Incluir opção real de ativar/desativar arbitragem nas configurações de cada partida.
- Com arbitragem ativa: aplicar regras de faltas, vantagem, cartões, expulsões, impedimento e VAR conforme os sistemas implementados.
- A opção não poderá ser apenas visual: deverá alterar efetivamente as decisões do motor da partida.
- Definir com o usuário o comportamento exato do modo quando a arbitragem estiver desativada antes de integrar a configuração final.

## Reações e cenas de arbitragem
- Jogadores devem reagir a faltas, cartões amarelos/vermelhos e decisões contestadas.
- Após expulsão, permitir animação de protesto/reclamação do jogador, com legenda contextual quando apropriado (ex.: jogador contestando a decisão).
- Reações devem ser controladas por eventos reais da arbitragem, sem tocar animações aleatórias desconectadas do lance.
- O sistema de animação deve permitir transições, interrupções e retorno ao estado de jogo.

## Impedimento visual
- Mostrar linha de referência alinhada ao penúltimo defensor no instante do passe.
- A linha deve acompanhar a perspectiva da câmera e destacar atacante/defensor em situação relevante.
- O overlay só aparece durante revisão/indicação de impedimento, não como decoração permanente.
- A decisão deve vir do cálculo real de impedimento.

## Controle de partida e espectador
- Pausar de verdade: congelar relógio, IA, física, bola e eventos enquanto pausado.
- Modo espectador: partida completa 11 contra 11, ambas as equipes controladas por IA.
- Permitir alternar entre assistir e controlar equipe somente se os controles e a simulação estiverem integrados.
- Não expor estes modos como funcionais antes da implementação e dos testes.

## Dependências e ordem
- Campo, bola, jogadores e câmera: FASE 3.
- Física, ações e regras de jogo: FASE 4.
- Arbitragem, cartões, VAR e impedimento: FASE 5.
- IA 11 contra 11 e comportamento tático: FASE 6.
- Animações de reação e conteúdo de jogadores: FASE 7, integradas aos eventos das fases anteriores.
- Pausa, modo espectador e seleção de modos: FASE 8, apoiados pelos sistemas anteriores.

## Atualização visual do lobby
- Commit `b35d9f261bca27549184611df4bfd558ecfeca4c` adiciona rig articulado de cinco grupos ósseos (tronco/cabeça e quatro membros animados por matrizes de osso) e animação procedural de repouso.
- Adiciona textura procedural de superfície em GPU, variação de gramado, padrão de tecido, iluminação difusa e especular e parâmetros de rugosidade.
- **Status:** IMPLEMENTADO, aguardando compilação e validação visual em dispositivo.
- Não declarar a evolução visual como TESTADA até aprovação da compilação e teste real no Android.


# 33. PRIMEIRA ENTREGA DO MOTOR DE PARTIDA — 2026-11-26

## Núcleo de simulação C++17
- **Status:** IMPLEMENTADO; aguardando CI e validação funcional.
- Criado `MatchSimulation.hpp/.cpp` em módulo separado do renderizador.
- Estado de partida: Ready, Running, Paused e Finished.
- Inicialização determinística de 22 jogadores, 11 por equipe, com posições-base de formação.
- Estado da bola, posse, velocidade, atrito, limites do campo e reposição após cruzar a linha de gol.
- Relógio de partida que só avança em estado Running.
- Atualização fixa limitada a 50 ms por chamada para reduzir instabilidade em quadros lentos.
- IA inicial de movimentação: perseguição da bola, avanço do portador, apoio e recomposição defensiva.
- Ações nativas: passe direcionado a companheiro, chute, drible, corrida e tentativa de desarme com validação de posse/proximidade.
- Controle de expulsão de jogador e contagem de atletas ativos.
- Modo espectador sinalizado no núcleo e bloqueio de comando manual para equipe do usuário quando ativo.
- Integração do núcleo ao GameEngine e inclusão no CMake Android.
- Ciclo de vida de partida exposto por JNI e wrappers Java/GameSurface: iniciar, pausar, retomar e reiniciar. Ainda não há controles visuais de partida conectados ao lobby.
- Testes nativos adicionados para 22 jogadores, estados, congelamento real do relógio/posições durante pausa, expulsão, bloqueio de comandos em espectador e reinicialização.
- Workflow atualizado para compilar e executar os testes nativos antes do APK Debug.

## Limitações explícitas desta entrega
- Os jogadores simulados ainda não são renderizados nas posições dinâmicas do MatchSimulation. O lobby permanece sendo a cena visual anterior.
- A IA é uma base de movimentação e posicionamento; ainda não representa futebol tático completo nem 11 contra 11 competitivo.
- Passe e chute aplicam velocidade à bola, mas ainda não há recepção avançada, goleiros, colisões completas, faltas ou cálculo de gol com geometria de baliza.
- Modo espectador ainda não possui seleção de modo/tela; o núcleo apenas bloqueia comandos humanos na equipe configurada.
- Arbitragem configurável, faltas/cartões, impedimento e linha visual, reações de protesto, VAR, replay em câmera lenta, HUD e controles touchscreen permanecem pendentes.
- A atualização do núcleo está integrada ao GameEngine, mas a partida ainda não é iniciada automaticamente nem existe botão de início na interface.

## Evidência
- A validação será registrada após a execução do workflow Android no commit desta seção. A existência dos testes não significa que já passaram.


# 34. CONTROLE DIRETO E MODO ÁRBITRO — 2026-11-26

## Entrega implementada na branch nativa
- **Status:** IMPLEMENTADO NO CÓDIGO; aguardando CI e teste em aparelho.
- O lobby oferece duas entradas reais para a partida: Jogar como atleta e Ser o árbitro.
- HUD Android nativo sobre a cena 3D, com pausa e troca de função durante a partida.
- Modo jogador: seleção entre os 11 atletas da equipe azul, controle direcional por toque e ações de passe, chute, drible, corrida e desarme.
- O jogador selecionado é controlado pelo usuário; os demais 21 são atualizados pela IA.
- Passe sem alvo explícito procura um companheiro à frente na direção escolhida.
- Modo árbitro: seleção de qualquer um dos 22 jogadores, apito que pausa/retoma a partida, cartões amarelos e vermelhos.
- Dois amarelos expulsam; vermelho expulsa imediatamente. A decisão de cartão só é aceita quando a função ativa é Árbitro.
- Renderer recebe o estado atualizado do motor e desenha 22 modelos de jogadores nas posições simuladas e uma bola 3D móvel.
- Eixos do campo foram alinhados aos gols reais da cena: comprimento no eixo longitudinal e largura no eixo transversal.
- JNI e wrappers Java conectam os comandos de partida, movimento, ações, troca de função e cartões.
- Testes nativos ampliados para seleção/movimentação de atleta, restrição de ações ao modo correto e regras de cartões.

## Limitações que permanecem
- O sistema ainda não possui animações específicas de corrida, chute, carrinho, reclamação, cartão vermelho ou expulsão; os modelos usam a animação procedural já existente.
- A arbitragem manual permite apitar e aplicar cartões, mas a detecção automática de faltas, impedimento, VAR, linha virtual e decisões de pênalti ainda não está implementada.
- O apito pausa a partida; não há ainda uma sequência visual de infração nem reinício de bola parada.
- A IA é funcional como movimentação básica, mas ainda precisa de goleiros, tática avançada, colisões, recepção e regras completas.
- HUD/renderer e comandos precisam ser validados em aparelho Android real. Compilação CI ainda pendente.


# 35. GOLEIRO, ANIMAÇÕES DE JOGADA E VALIDAÇÃO DE ARBITRAGEM — 2026-11-26

## Implementação na branch nativa
- **Status:** implementado no código; aguardando CI desta revisão e teste visual em Android.
- O seletor do modo jogador oferece o goleiro (camisa 1); ao selecioná-lo, aparecem comandos contextuais de mergulho/defesa e agarrar a bola.
- Ações nativas do goleiro verificam a função do atleta e a distância da bola; mergulho desloca o goleiro em direção à bola e captura quando alcança o raio de defesa.
- Estado de animação por jogador com temporizador: protesto após amarelo, reclamação/expulsão após vermelho ou segundo amarelo, mergulho e defesa com as mãos.
- O renderer recebe o estado de animação e aplica poses esqueléticas procedurais com blend temporal; jogador expulso protesta e depois se desloca para fora do campo.
- A linha de impedimento passa a considerar os 11 adversários ativos, incluindo o goleiro, ordenados pela progressão em relação ao gol atacado. A linha é exibida no lance detectado.
- Cenários de teste incluem passe em impedimento, passe em posição legal, linha virtual, revisão VAR anulando decisão, defesa/agarre do goleiro e cobrança de pênalti após falta na área.

## Limitações e validação
- As animações são poses esqueléticas procedurais do modelo atual, não captura de movimento nem clipes cinematográficos completos.
- A detecção automática de impedimento avalia o destinatário selecionado no instante do passe; ainda não analisa interferência, participação ativa, rebotes ou segundo toque.
- O VAR atual guarda e restaura o estado da bola e a decisão da arbitragem; não há replay multiângulo nem análise automática de imagens.
- A defesa do goleiro usa distância e movimento simplificados; ainda faltam colisão física completa, trajetória de voo e animações de queda/recuperação mais detalhadas.
- CI e compilação APK desta revisão ainda precisam terminar; nenhum teste em aparelho foi realizado.


# 36. CICLO COMPLETO DE DEFESA DO GOLEIRO — 2026-11-26

## Implementação desta revisão
- **Status:** IMPLEMENTADO NO CÓDIGO; aguardando testes de CI.
- A bola possui altura e velocidade vertical no núcleo, com gravidade e quique simplificado.
- O renderer posiciona a bola conforme a altura calculada pelo motor.
- O mergulho deixa de teleportar o goleiro: aplica deslocamento temporal com velocidade e limites da área.
- O ciclo passa por mergulho, queda e recuperação, com temporizadores e retorno ao estado neutro.
- A ação de agarrar aceita bolas dentro de uma faixa de altura ampliada e diferencia a pose de defesa alta.
- A IA pode interceptar uma bola próxima e dentro do alcance vertical do goleiro.
- Testes adicionados para captura de bola alta, trajetória aérea e progressão de queda/recuperação.

## Limites e validação
- Animações procedurais, sem captura de movimento.
- Física vertical simplificada; ainda sem colisões 3D completas com traves, rede, mãos e corpo.
- IA ainda não prevê trajetórias complexas nem escolhe entre espalmar, socar e encaixar por atributos.
- Validação visual em Android permanece pendente; CI não substitui teste físico.
- A FASE 3 permanece **EM DESENVOLVIMENTO** até inspeção visual e testes no dispositivo.


# 37. FASE 3 — EVOLUÇÃO DO CAMPO E CÂMERA DE PARTIDA — 2026-11-26

## Alterações em andamento na branch nativa
- **Status:** IMPLEMENTADO NO CÓDIGO; aguardando CI e validação visual no Android.
- A câmera de partida deixa de permanecer fixa no centro do estádio: acompanha o atleta controlado e a posição da bola, com enquadramento mais fechado que o lobby.
- No modo árbitro, o enquadramento acompanha a região da bola.
- O campo recebeu arcos de pênalti com geometria própria, complementando as marcações da área.
- As redes das duas balizas receberam subdivisões geométricas para melhorar a leitura visual da profundidade.
- O estádio, o gramado procedural, os modelos de atletas, a bola e o renderizador existentes foram preservados; as alterações evoluem os módulos atuais, sem duplicar a cena.

## Correção da validação do goleiro
- A execução do workflow 45732495301 revelou uma expectativa antiga no teste: ele exigia captura instantânea no comando de mergulho, embora a nova implementação faça o goleiro se deslocar antes da defesa.
- O teste foi ajustado para verificar primeiro o estado de mergulho, avançar a simulação e então confirmar a captura após o deslocamento.
- A correção ainda precisa ser confirmada por uma nova execução do CI.

## Pendências obrigatórias antes de aprovar a Fase 3
- Confirmar que os testes nativos passam e que o APK Debug compila.
- Verificar a câmera em diferentes proporções de tela e confirmar que não corta o lance.
- Testar a cena no Android físico: qualidade do gramado, linhas, balizas, redes, escala dos jogadores, bola e fluidez.
- Evoluir colisões de bola com postes/travessão/rede e revisar o enquadramento de replay.
- Não declarar a fase concluída apenas com aprovação do CI: é necessário teste visual real no aparelho.


# 38. CAMPO 3D — GRAMADO, TORCIDA E APRESENTAÇÃO DE ESTÁDIO — 2026-11-26

## Implementação preparada nesta revisão
- **Status:** código alterado na branch; aguardando CI único desta revisão e inspeção visual em Android físico.
- O gramado ganhou variação procedural em múltiplas escalas: faixas de corte mais legíveis, microtextura e variação fina de superfície no shader.
- Foi adicionada uma faixa de escape em volta do campo para separar visualmente a grama da área técnica.
- As arquibancadas receberam silhuetas estáticas de torcedores em geometria leve, geradas uma única vez na construção da cena.
- O perímetro recebeu painéis estáticos de publicidade em geometria 3D, sem depender de imagens externas ou rede.
- Foi criado um teste nativo específico para validar construção da cena, quantidade mínima de geometria, valores finitos e posições das quatro formações.
- O workflow existente foi ampliado para executar esse teste de cena antes de montar o APK; continua sendo uma única execução de CI por revisão.

## Limites e critérios ainda pendentes
- A torcida desta etapa é estática e estilizada; não representa pessoas animadas individualmente.
- Os painéis são elementos visuais geométricos, ainda sem marcas, textos ou conteúdo dinâmico.
- A textura do gramado é procedural no shader; não é uma textura fotográfica de alta resolução.
- O teste automatizado valida a geometria gerada, mas não substitui a avaliação visual, de FPS, memória e temperatura no aparelho Android.
- Não marcar a FASE 3 como concluída até verificar a cena em aparelho real e corrigir eventuais problemas de enquadramento ou desempenho.


# 39. FASE 4 — FÍSICA DE GOL E COLISÕES COM A BALIZA — 2026-11-26

## Implementação
- **Status:** TESTADO em CI na execução 45736218871 (testes nativos de partida e estádio aprovados; APK Debug montado e artefato enviado). Teste físico/visual no Android continua pendente.
- Gol agora só é contabilizado quando a bola cruza a linha dentro da largura útil da baliza e abaixo do travessão.
- Bola que passa ao lado ou acima da baliza não aumenta o placar.
- Colisões simplificadas foram adicionadas para os dois postes e o travessão, com reflexão de velocidade.
- Após um gol, o motor registra o evento, pausa a partida, posiciona a bola no centro e atribui a saída à equipe adversária.
- O reinício de jogo existente passa a permitir a saída após gol sem apagar o placar.
- Testes de regressão cobrem gol válido, chute para fora, chute acima do travessão, colisão com poste e reinício após gol.

## Limites e próximos passos
- Colisões de baliza são aproximações 2D com altura separada; ainda não são um solver físico 3D contínuo.
- Ainda faltam rede deformável, contato corporal, desvio por jogadores e arbitragem completa para bola fora.
- A aprovação de CI não comprova sensação de chute ou qualidade visual; validar no aparelho continua obrigatório.


# 40. REINÍCIO CORRETO APÓS GOL E PARADAS — 2026-11-26

## Correção de fluxo
- **Status:** IMPLEMENTADO NO CÓDIGO; aguardando CI.
- A ação comum de continuar não pode apenas descongelar o relógio quando existe um evento que exige bola parada.
- MatchSimulation::resume() agora encaminha eventos pausados com equipe de reinício definida para restartPlay().
- A cobrança após gol executa a saída da equipe adversária, preservando o placar e a posse inicial correta.
- Durante uma revisão VAR aberta, o comando comum de continuar não libera a partida antes da decisão do árbitro.
- Pausas manuais sem evento continuam retomando normalmente.
- Teste de regressão adicionado para reproduzir o fluxo de gol seguido do botão Continuar.

## Validação pendente
- Executar testes nativos e compilação Android Debug no mesmo workflow.
- Testar o fluxo visual de gol, pausa, menu Continuar e saída no aparelho.
- A correção não representa ainda a implementação de laterais, escanteios ou tiros de meta; esses reinícios permanecem no escopo seguinte da FASE 5.


# 41. FASE 5 — REINÍCIOS, VANTAGEM, PÊNALTIS E VAR — 2026-11-26

## Escopo implementado nesta revisão
- **Status:** TESTADO em CI na execução `45823163280`; os testes nativos de partida e o APK Debug foram aprovados. A validação em Android físico permanece pendente.
- Bola que cruza a linha lateral gera lateral para a equipe adversária ao último toque.
- Bola que cruza a linha de fundo fora da baliza gera escanteio quando o último toque é da defesa; caso contrário, tiro de meta para a defesa.
- Reinícios posicionam a bola nas zonas correspondentes e escolhem cobrador ativo; tiro de meta prioriza o goleiro, usando atleta ativo alternativo se ele foi expulso.
- Falta manual só é aceita durante a partida em andamento e continua distinguindo falta comum de pênalti conforme a área.
- Botão real de vantagem integrado à interface Android, GameSurface, GameRenderer, JNI e GameEngine. A vantagem mantém o jogo ativo por uma janela curta; se a equipe beneficiada conquista a posse, a partida segue; se perde a posse ou a janela expira, a falta é marcada.
- VAR guarda placar anterior e snapshot da bola; anular um gol também desfaz o ponto e gera tiro de meta para a defesa.

## Testes de regressão adicionados
- Lateral, escanteio e tiro de meta com equipe beneficiada definida pelo último toque.
- Vantagem convertida em posse da equipe beneficiada.
- VAR anulando gol, restaurando placar e encaminhando tiro de meta.
- Os testes serão executados junto com os testes de estádio e a compilação APK Debug no workflow único desta revisão.

## Limitações explícitas
- Reposições ainda não aplicam todas as restrições de posicionamento, distância dos adversários, execução manual com as mãos no lateral ou impedimento diretamente do reinício.
- A janela de vantagem usa posse da bola como critério; não modela intensidade da infração, oportunidade clara, distância do gol ou sinal gestual do árbitro.
- A detecção automática de infrações permanece limitada ao desarme de alta velocidade; não há árbitro IA completo.
- O VAR não tem replay multiângulo, linhas calibradas nem revisão automática de imagens; a decisão continua manual.
- A aprovação de CI não substitui teste de jogabilidade, interface e desempenho em Android físico.


# 42. FASE 6 — PRIMEIRA CAMADA DE IA TÁTICA — 2026-11-26

## Implementação preparada nesta revisão
- **Status:** TESTADO em CI na execução `45823163280`; testes de partida, IA tática e estádio passaram, e o APK Debug foi montado e enviado como artefato. A validação visual e de desempenho em Android físico permanece pendente.
- Criado módulo separado `TeamAI.hpp/.cpp`, sem concentrar a lógica tática no renderizador.
- A IA passa a calcular alvos diferentes por função: goleiro, linha defensiva, meio-campo e atacantes.
- A linha defensiva ajusta profundidade conforme posse e avanço da bola; meio-campistas oferecem apoio e cobertura; atacantes procuram profundidade e largura.
- O jogador mais próximo é designado para pressionar quando a equipe não tem a posse, evitando que os 10 jogadores persigam a bola ao mesmo tempo.
- Adicionadas estratégias configuráveis no núcleo: equilibrada, posse, contra-ataque, pressão alta e bloco defensivo.
- Estratégia altera posicionamento e multiplicador de deslocamento; a seleção ainda não está conectada a um menu Android nesta etapa.
- A IA do goleiro ajusta a posição lateral conforme a bola, mantendo limites de área simplificados.
- Integração com MatchSimulation e CMake Android; os jogadores controlados pelo usuário mantêm o caminho de movimento manual.
- Adicionados testes nativos específicos para pressão, linha defensiva, ataque em profundidade, espelhamento das equipes, limites do campo e configuração/reset de estratégia.
- Workflow ampliado para executar os testes de IA antes da montagem do APK, na mesma execução de CI.

## Limitações e trabalho restante da FASE 6
- Ainda não há decisão autônoma de passe, chute, cruzamento, drible ou finalização pela IA; o módulo atual calcula posicionamento e deslocamento.
- Marcação por zona/homem, cobertura coordenada, troca de marcador, linhas de impedimento e gatilhos de pressão ainda precisam evoluir.
- A estratégia está exposta no núcleo C++, mas falta conectá-la ao menu tático Android e persistir a escolha por equipe.
- Níveis de dificuldade, atributos individuais influenciando decisões, comportamento de substitutos e ajuste dinâmico de formação permanecem pendentes.
- Os testes validam a lógica matemática e integração nativa; ainda é necessário validar comportamento visual e desempenho no Android físico.
- A FASE 6 permanece EM DESENVOLVIMENTO; esta entrega não representa IA completa nem jogo competitivo 11 contra 11.


# 43. FASE 6 — DECISÕES AUTÔNOMAS DO PORTADOR DA BOLA — 2026-11-26

## Implementação
- **Status:** IMPLEMENTADO NO CÓDIGO; primeira execução falhou no teste de IA porque o teste tentou usar um helper privado declarado apenas no arquivo de testes do motor. O teste de módulo foi isolado para validar a política de decisão sem acessar esse helper; integração ponta a ponta permanece pendente de teste.
- TeamAI agora decide entre passe, chute e condução por uma função determinística e separada do motor de partida.
- A decisão considera distância e corredor até o gol, pressão adversária, companheiro em progressão e estilo tático.
- A IA procura companheiros em posição adiantada e avalia a pressão próxima do receptor.
- O portador controlado pelo usuário não recebe decisões autônomas no modo de jogador.
- Em modo espectador, modo árbitro ou quando outro atleta da equipe está com a bola, a IA pode assumir decisões do portador.
- A condução mantém a posse e desloca o atleta em direção ao gol; as decisões têm intervalo de 0,62 segundo para evitar spam.
- O comando interno reutiliza as validações existentes de passe, chute e impedimento.
- Testes unitários adicionados para chute, passe, condução e contexto inválido. A integração ponta a ponta e a preservação do controle humano ainda precisam de testes específicos no motor.

## Limitações
- Ainda não há cruzamentos contextuais, fintas encadeadas, seleção de tipo de passe, finalização por atributos ou decisões baseadas em personalidade.
- A avaliação de linha de passe e pressão é geométrica e simplificada.
- Desmarcação coordenada, cobertura, marcação por zona/homem e decisão dos jogadores sem bola ainda precisam evoluir.
- Testes nativos não comprovam equilíbrio ou desempenho visual em Android físico.
