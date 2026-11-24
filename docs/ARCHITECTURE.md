# Arquitetura — FOOTBALL LEGENDS

## 1. Visão geral

Aplicação Unity 6 para Android, organizada em módulos com dependências explícitas. A primeira entrega técnica é o lobby; sistemas de partida, física de bola, IA de adversários e multiplayer ficam fora do escopo desta etapa.

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
- **QuickMatchEntry**: entrada para a futura experiência de partida; durante esta fase, deve indicar claramente se o modo ainda não está disponível.

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

## 8. Limites desta etapa

Esta arquitetura descreve o lobby e a base técnica. Não afirma que exista gameplay de futebol, multiplayer, times licenciados ou assets 3D prontos. Esses itens exigem implementação e validação em etapas próprias.
