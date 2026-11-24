# Sistemas de partida — FOOTBALL LEGENDS

## Modos

- **Jogar partida**: controle manual de um atleta, com IA para os demais jogadores.
- **Ser árbitro • IA x IA**: duas equipes controladas pela IA e painel de arbitragem manual.

## IA tática — versão em desenvolvimento

O componente `TacticalPlayerAI` possui uma camada de coordenação baseada em regras:

- Reavalia decisões em intervalos curtos e mantém deslocamento/animação atualizados a cada frame.
- Identifica o jogador mais próximo da bola em cada equipe.
- Organiza pressão no portador da bola e cobertura pelo segundo defensor mais próximo.
- Distribui jogadores em opções de apoio, largura e profundidade.
- Recompõe uma estrutura defensiva compacta sem posse.
- Mantém o goleiro em uma área de atuação própria.

### Decisão de passe

O portador da bola avalia companheiros elegíveis antes de tocar:

- distância de passe;
- progressão em direção ao gol;
- distância do marcador mais próximo do receptor;
- espaço livre no corredor entre passador e receptor;
- posição de impedimento no instante da decisão.

O passe escolhido recebe um impulso físico e o jogador executa a animação básica de chute. Se não houver opção com pontuação suficiente, a IA conduz a bola para um corredor com mais espaço.

### Movimentação ofensiva

- Atacantes procuram profundidade e canais próximos à última linha defensiva.
- Meio-campistas oferecem linhas de apoio em diferentes alturas.
- Os jogadores mantêm largura baseada na posição inicial e ajustam o posicionamento à bola.

### Estratégias de marcação

- O jogador mais próximo pressiona o portador.
- O segundo jogador mais próximo oferece cobertura.
- Os demais recompõem o bloco defensivo e procuram adversários em sua zona.
- A aproximação ao adversário é ponderada pela distância e pela proximidade da bola.

**Limite atual:** trata-se de uma IA determinística baseada em regras, pontuações e distâncias. Não é aprendizado de máquina e ainda não possui planejamento avançado de jogadas, estratégias configuráveis por treinador, análise de fadiga ou tomada de decisão profissional. O comportamento ainda precisa ser observado e balanceado em partidas completas.

## Goleiros, gols e placar

- Um goleiro por equipe, com posicionamento lateral próximo à área.
- Tentativa de defesa quando a bola chega perto do goleiro.
- Verificação de cruzamento da linha do gol dentro da largura e altura válidas.
- Placar separado para equipe azul e vermelha.
- Reinício da bola após gol.

## Regra de impedimento

`OffsideRuleSystem` verifica a posição do receptor no momento do passe, considerando metade ofensiva, bola e segundo último adversário. A linha virtual é desenhada no campo. No modo árbitro, um lance sinalizado pausa a simulação e permite marcar impedimento ou deixar seguir.

## Modo árbitro livre

- Selecionar qualquer um dos 22 jogadores.
- Parar e retomar a partida.
- Aplicar cartão amarelo ou vermelho sem exigir falta ou justificativa.
- Dois amarelos também resultam em expulsão.
- Jogador punido faz gesto de reclamação e exibe uma fala na interface.

## Controles do modo jogador

- Analógico virtual para movimentação.
- Condução básica da bola quando o atleta está perto.
- Passe manual com avaliação de impedimento.
- Chute com impulso físico.
- Corrida temporária, troca de jogador e tentativa de desarme.

## Animação e modelos

Os atletas atuais são personagens procedurais compostos por primitivas 3D, com movimentos básicos de braços e pernas, varredura da cabeça e animação de reclamação. Isso é um blockout animado, não um modelo final de produção. Ainda são necessários modelos humanos de alta qualidade, malhas, UVs, texturas, rig esquelético profissional, animações de corrida/chute/queda/defesa e integração com Animator.

## Limites e validação pendente

- A nova seleção de passes, os desmarques e a marcação zonal precisam ser testados em partidas completas.
- Verificar se a IA evita passes interceptáveis e respeita impedimento nos diferentes lados do campo.
- Balancear velocidade, distância de passe, pontuação e frequência de decisão.
- A bola e os jogadores precisam de balanceamento de física e colisões.
- O sistema de gols, goleiros, placar, impedimento e cartões precisa ser testado.
- Ainda faltam faltas, laterais, escanteios, pênaltis, substituições, cronômetro e áudio.
- Ainda não houve compilação no Unity 6 nem teste em dispositivo Android real.
- Nenhum APK desta etapa foi gerado ou validado.


## Escolha do atleta controlado antes da partida

- No modo de partida manual, a simulação começa pausada e apresenta um painel de pré-jogo.
- O usuário escolhe um dos 11 atletas do time azul, visualizando número da camisa e posição.
- Ao iniciar, somente o atleta escolhido recebe controle manual; a IA desse atleta fica desativada.
- Os outros dez companheiros e os onze adversários continuam com a IA tática ativa.
- O botão TROCAR continua permitindo transferir o controle para outro companheiro durante a partida; a IA volta a controlar o atleta anterior.
- O modo árbitro IA x IA não utiliza essa tela de seleção.
- A IA dos companheiros pode conduzir, passar e apoiar as jogadas sem exigir que o usuário controle mais de um atleta.
