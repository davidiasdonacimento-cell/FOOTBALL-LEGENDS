# Arquitetura — FOOTBALL LEGENDS

## 1. Visão geral

Aplicação Unity 6 para Android, organizada em módulos com dependências explícitas. O projeto contém lobby e uma primeira camada de partida com IA tática baseada em regras, física básica da bola e modo árbitro.

## 2. Camadas

### Core
- **GameBootstrap**: inicialização e composição dos serviços.
- **SceneFlow**: troca assíncrona de cenas e retorno seguro ao lobby.
- **SaveService**: leitura/gravação local com versão de dados.
- **AudioService**: música, ambiente e efeitos de interface.
- **Diagnostics**: logs e captura de erros relevantes.

### Lobby
- **LobbyController**: estado geral e ciclo de vida do lobby.
- **LobbyNavigation**: navegação entre áreas e seleção do destino.
- **LobbyView**: apresentação da interface e atualização visual.
- **LobbyTransition**: animações e transições.

### Apresentação 3D
- **PlayerShowcase**: personagem, uniforme, pose e animações.
- **StadiumEnvironment**: campo, arquibancadas, iluminação e ambiente.
- **CameraRig**: enquadramento do atleta e composição da cena.
- **AssetCatalog**: referências e carregamento de modelos e materiais.

### Funcionalidades do lobby
- **ProfileSystem**: perfil, nível e progresso.
- **TeamPreview**: prévia do elenco e formação.
- **EventSystem**: eventos, desafios e torneios apresentados no lobby.
- **QuickMatchEntry**: entrada para partida manual ou modo árbitro IA x IA.

## 3. Fluxo de inicialização

1. Carregar configurações e serviços essenciais.
2. Validar os dados locais e aplicar migrações necessárias.
3. Abrir a cena Lobby.
4. Carregar em segundo plano os recursos de apresentação.
5. Exibir o lobby quando os elementos essenciais estiverem prontos.

## 4. Cena Lobby

Composição prevista:
- estádio 3D como cenário de fundo;
- atleta 3D à esquerda;
- navegação e painéis à direita;
- barra superior para perfil, moeda fictícia e notificações;
- painéis de eventos, elenco e acesso rápido.

A interface deve usar Canvas Scaler e áreas seguras do Android. O layout será testado em diferentes proporções de tela.

## 5. Persistência

- Dados locais em formato versionado.
- Escrita atômica: gravar em arquivo temporário e substituir o arquivo principal após sucesso.
- Valores padrão quando não existir salvamento.
- Tratamento de arquivo inválido sem travar a inicialização.
- Não armazenar segredos no repositório.

## 6. Desempenho Android

- Preferir modelos e texturas otimizados para dispositivos móveis.
- Controlar luzes em tempo real, sombras e transparências.
- Usar LOD quando os modelos justificarem.
- Evitar carregar o estádio e o elenco completo de uma vez.
- Medir FPS, memória e tempo de carregamento em dispositivo real.

## 7. Build e integração contínua

GitHub Actions será usado para validar os arquivos e, após configuração da licença Unity e do módulo Android, executar a compilação. Um workflow verde não substitui o teste do APK em aparelho real.

## 8. Sistemas de partida

- **MatchBootstrap**: composição da partida, placar, gols, HUD e fluxo de arbitragem.
- **MatchPlayer**: atleta procedural, estado disciplinar, movimentação e animações básicas.
- **TacticalPlayerAI**: posicionamento por formação, aproximação da bola, avanço e recomposição.
- **OffsideRuleSystem**: avaliação de impedimento no instante do passe.
- **VirtualJoystick**: entrada de toque para movimentação e condução.
- **RefereeMode**: seleção de atleta, pausa, cartões livres e decisões de impedimento.

## 9. Limites e validação

A IA tática e as regras implementadas são uma primeira camada jogável, não uma simulação de futebol profissional completa. Os atletas são modelos procedurais provisórios; modelos 3D finais, rig profissional, animações avançadas e áudio ainda precisam ser integrados. A física, a arbitragem e o placar precisam de compilação e testes em aparelho. Multiplayer e licenciamento de clubes/atletas não estão implementados.
