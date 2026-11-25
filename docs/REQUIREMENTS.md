# Requisitos — FOOTBALL LEGENDS MOBILE

## Plataforma
- [x] Direção técnica Android nativo, C++17 e OpenGL ES 3.0.
- [ ] APK ARM64 compilado e validado.
- [ ] Orientação paisagem e adaptação a telas.
- [ ] Testes em aparelhos reais.

## Apresentação 3D
- [ ] Campo 3D com marcações.
- [ ] Estádio, arquibancadas, iluminação e câmera.
- [ ] Atletas com malha, rig, uniformes e animações.
- [ ] Bola 3D com física.
- [ ] HUD e menus próprios.

## Partida offline
- [ ] Seleção de equipes e formação.
- [ ] Controles de movimento, passe, chute e corrida.
- [ ] IA de companheiros e adversários.
- [ ] Goleiros, gols, placar e cronômetro.
- [ ] Regras básicas e reinícios.
- [ ] Pausa e retorno ao menu.

## Multiplayer online
- [ ] Backend autoritativo.
- [ ] Autenticação e sessões seguras.
- [ ] Matchmaking e salas.
- [ ] Protocolo versionado e sincronização.
- [ ] Reconexão e tratamento de latência.
- [ ] Validação de resultados no servidor.
- [ ] Testes com múltiplos dispositivos.

## Estabilidade
- [ ] Persistência local versionada e gravação atômica.
- [ ] Recuperação de save inválido.
- [ ] Gerenciamento de memória e recursos GPU.
- [ ] Ciclo de vida Android testado.
- [ ] Logs sem dados secretos.

## Critérios da primeira etapa
- [ ] Projeto Gradle compila no GitHub Actions.
- [ ] APK de depuração é produzido.
- [ ] APK instala e abre em Android ARM64.
- [ ] OpenGL ES inicializa sem crash.
- [ ] Cena de teste 3D é visível em aparelho real.

Itens pendentes não devem ser descritos como implementados.
