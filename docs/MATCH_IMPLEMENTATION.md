# Sistemas iniciais de partida

## Modos disponíveis no lobby

- **Jogar partida**: abre a cena Match com o HUD de controle.
- **Ser árbitro • IA x IA**: abre a cena Match sem analógico, com painel de decisão para lances sinalizados.

## Regra de impedimento

`OffsideRuleSystem` avalia a posição do jogador que recebe o passe no instante do passe. A implementação considera:
- metade ofensiva do campo;
- posição da bola;
- segundo último adversário;
- direção de ataque de cada equipe.

A cena desenha uma linha transversal no campo: vermelha para posição de impedimento e amarela para lance regular. No modo jogador, o passe sinalizado é cancelado automaticamente. No modo árbitro, o jogo pausa e o usuário escolhe entre marcar impedimento ou deixar seguir.

## Controles Android

- Analógico virtual: movimentação.
- Passe: envia a bola em direção ao companheiro selecionado.
- Chute: aplica impulso à bola.
- Correr: aumenta temporariamente a velocidade.
- Trocar: alterna o atleta controlado.
- Desarmar: tenta recuperar a bola quando há um adversário próximo.
- Sair: retorna ao lobby.

## Limitações conhecidas

Esta é a primeira camada de gameplay, não uma partida completa. O modo IA x IA gera cenários de passe para exercitar a decisão de impedimento; ainda faltam movimentação tática contínua, goleiros, placar por gols, faltas, cartões, laterais, escanteios, animações esqueléticas, modelos finais, áudio e testes em aparelho real. Os atletas e o campo são objetos provisórios criados em runtime.

A compilação Unity e o teste Android continuam pendentes.