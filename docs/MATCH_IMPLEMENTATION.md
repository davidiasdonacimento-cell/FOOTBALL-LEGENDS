# Sistemas de partida — FOOTBALL LEGENDS

## Modos

- **Jogar partida**: controle manual de um atleta, com IA para os demais jogadores.
- **Ser árbitro • IA x IA**: duas equipes controladas pela IA e painel de arbitragem manual.

## IA e movimentação

`TacticalPlayerAI` executa movimentação contínua por formação, aproximação ao portador da bola, avanço de atacantes, recomposição de defensores e posicionamento do goleiro. A lógica atual é uma primeira IA tática baseada em regras; ainda não é uma IA avançada de futebol profissional, com planejamento de jogadas, pressão coordenada e tomada de decisão completa.

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
- Marcar impedimento ou deixar o jogo seguir.
- Jogador punido faz gesto de reclamação, movimenta os braços e a boca, vira a cabeça e exibe uma fala na interface, por exemplo: “Ah, fui roubado!”.

## Controles do modo jogador

- Analógico virtual para movimentação.
- Condução básica da bola quando o atleta está perto.
- Passe com avaliação de impedimento.
- Chute com impulso físico.
- Corrida temporária, troca de jogador e tentativa de desarme.

## Animação e modelos

Os atletas atuais são personagens procedurais compostos por primitivas 3D, com movimentos básicos de braços e pernas, varredura da cabeça e animação de reclamação. Isso é um blockout animado, não um modelo final de produção. Ainda são necessários modelos humanos de alta qualidade, malhas, UVs, texturas, rig esquelético profissional, animações de corrida/chute/queda/defesa e integração com Animator.

## Limites e validação pendente

- A IA tática ainda é baseada em regras simples e precisa de refinamento.
- A bola e os jogadores precisam de balanceamento de física e colisões.
- O sistema de gols, goleiros, placar, impedimento e cartões precisa ser testado em partidas completas.
- Ainda faltam faltas, laterais, escanteios, pênaltis, substituições, cronômetro e áudio.
- O APK ainda não foi compilado no Unity nem testado em dispositivo Android real.
