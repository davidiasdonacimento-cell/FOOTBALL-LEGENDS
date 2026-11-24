# Sistemas de partida — FOOTBALL LEGENDS

## Modos

- **Jogar partida**: controle manual de um atleta, com IA para os demais jogadores.
- **Ser árbitro • IA x IA**: duas equipes controladas pela IA e painel de arbitragem manual.

## IA e movimentação

O componente `TacticalPlayerAI` foi evoluído para uma primeira camada de coordenação coletiva:

- Reavalia decisões táticas em intervalos curtos, sem limitar o movimento ao intervalo de pensamento.
- Atualiza deslocamento e animação a cada frame, corrigindo a movimentação em pequenos impulsos que ocorria quando o movimento era chamado somente após o temporizador.
- Identifica o atleta mais próximo da bola em cada equipe.
- Organiza pressão no portador da bola e cobertura pelo segundo defensor mais próximo.
- Quando a equipe tem a posse, distribui os demais atletas em opções de apoio, largura e profundidade de acordo com suas posições.
- Sem posse, os jogadores recompõem uma estrutura defensiva compacta.
- Mantém o goleiro em uma área de atuação própria, ajustando sua posição lateral à bola.

**Limite atual:** esta é uma IA tática baseada em regras e distâncias. Ainda não possui planejamento de jogadas, estratégias de equipe configuráveis, avaliação avançada de risco, aprendizado ou simulação profissional. Os comportamentos precisam ser validados em partidas reais no Unity.

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
- Jogador punido faz gesto de reclamação, movimenta os braços e a boca, vira a cabeça e exibe uma fala na interface.

## Controles do modo jogador

- Analógico virtual para movimentação.
- Condução básica da bola quando o atleta está perto.
- Passe com avaliação de impedimento.
- Chute com impulso físico.
- Corrida temporária, troca de jogador e tentativa de desarme.

## Animação e modelos

Os atletas atuais são personagens procedurais compostos por primitivas 3D, com movimentos básicos de braços e pernas, varredura da cabeça e animação de reclamação. Isso é um blockout animado, não um modelo final de produção. Ainda são necessários modelos humanos de alta qualidade, malhas, UVs, texturas, rig esquelético profissional, animações de corrida/chute/queda/defesa e integração com Animator.

## Limites e validação pendente

- A IA tática melhorada ainda precisa de testes de comportamento e balanceamento em partidas completas.
- A bola e os jogadores precisam de balanceamento de física e colisões.
- O sistema de gols, goleiros, placar, impedimento e cartões precisa ser testado em partidas completas.
- Ainda faltam faltas, laterais, escanteios, pênaltis, substituições, cronômetro e áudio.
- Ainda não houve compilação no Unity 6 nem teste em dispositivo Android real.
- Nenhum APK desta etapa foi gerado ou validado.
