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
