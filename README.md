# 🧼 Jogo Interativo Educativo: Lavar as Mãos

![Status](https://img.shields.io/badge/Status-Concluído-success)
![Plataforma](https://img.shields.io/badge/Plataforma-ESP32-blue)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-orange)

Este projeto consiste num **Guia Educativo Interativo**, desenvolvido com o objetivo de ajudar e incentivar as crianças a lavar as mãos de forma correta e divertida. O sistema cruza hardware eletrónico com design multimédia (áudio e imagens) para criar uma experiência gamificada.

---

## Como Funciona o Jogo?

O sistema guia a criança através de **5 etapas fundamentais** da lavagem das mãos:
1. Pôr sabão nas mãos.
2. Ligar a torneira.
3. Passar as mãos por água para tirar o sabão.
4. Fechar a torneira.
5. Secar as mãos.

Para manter a criança concentrada na atividade, implementámos um mini-jogo de memória:
* **Mecânica de Foco:** Antes de cada etapa, é exibido um animal de forma aleatória (🦀 **Caranguejo** ou 🐟 **Peixe**) acompanhado de um aviso sonoro (*"Presta atenção!"*), indicando que esse animal vai realizar a etapa.

### Quiz Interativo de Memória
Após as 5 etapas, o sistema entra em modo de avaliação:
* Gera uma pergunta aleatória cruzando um animal com uma das etapas (ex: *"Este animal fez a etapa de lavar as mãos?"* juntamente com a imagem do animal).
* A criança responde através de dois botões físicos (**"Sim"** e **"Não"**).
* **Feedback:** Se a criança errar, o ecrã fica vermelho e ouve-se um feedback sonoro (*"Tenta outra vez"*). O sistema repete a mesma pergunta para não desmotivar a criança até que ela acerte.

---

## Arquitetura e Hardware

O sistema foi desenhado para ser autónomo, portátil e seguro para utilização infantil. 

### Componentes Principais:
* **Microcontrolador:** ESP32-WROOM-32 (Cérebro da operação)
* **Interface Visual:** Display LCD TFT ST7735 (128x128px)
* **Interface Áudio:** Módulo DFPlayer Mini + Altifalante
* **Inputs:** 3x Botões Táteis (Sim, Não e Ação Principal)
* **Gestão de Energia:** * Bateria Li-Po 3.7V (800mAh)
  * Módulo TP4056 (Para carregamento via USB)
  * Módulo Step-Up MT3608 (Para elevar a tensão para 5V, alimentando os periféricos)


[Clique aqui para ver o Diagrama do Circuito (PDF)](https://github.com/ThZedd/Project-Factory/blob/main/media/Circuitos_Eletricos.pdf)

---

## Detalhes Técnicos e Funcionalidades

* **Sincronização de Áudio Assíncrono:** O sistema foi configurado com pausas dinâmicas ajustadas ao tamanho dos ficheiros MP3 lidos pelo DFPlayer. Isto garante que a execução do código flui sem que nenhum áudio seja cortado a meio.
* **Bloqueio de Segurança (Cooldown / Debounce):** Implementação de um bloqueio temporal via software para ignorar leituras de "duplos cliques" acidentais nos botões por parte das crianças, garantindo a estabilidade e fluidez do jogo.

---

## Equipa e Distribuição de Tarefas

Este projeto foi desenvolvido de forma colaborativa, com a seguinte distribuição de responsabilidades:

* **Bernardo Carvalho:** Lógica principal do jogo, programação do display TFT, gestão do DFPlayer e implementação do sistema de bloqueio de segurança (Cooldown).
* **Adjami Regula:** Desenvolvimento da lógica do Quiz Interativo de memória e gestão do feedback em tempo real.
* **Tiago Rato:** Otimização e implementação da Sincronização de Áudio Assíncrono.

---
*Projeto desenvolvido no âmbito da disciplina [Project Factory/Engenharia Informática] - 2026*