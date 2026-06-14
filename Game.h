#pragma once
#include "raylib.h"
#include <vector>
#include "Slime.h"
#include "Platform.h"

// definições globais da resolução da janela. 
// manter como constante fora da classe facilita caso seja necessário alterar a proporção da tela no futuro
const int screenWidth = 800;
const int screenHeight = 450;

// implementação de uma máquina de estados (state machine) finita.
// estrutura fundamental para controlar o fluxo da aplicação, garantindo que o 
// sistema saiba diferenciar quando deve renderizar o menu ou processar a física da fase
enum GameState { MENU, LEVEL_SELECT, PLAYING, ENDING };

class Game {
private:
    // variáveis de controle de progresso
    GameState currentState;
    int currentLevel;
    
    // ponteiro para o jogador. o uso do ponteiro permite instanciar (new) e destruir (delete) 
    // a slime dinamicamente na memória sempre que a fase é carregada ou reiniciada
    Slime* player;
    
    // lista dinâmica que armazena a geometria do cenário atual
    std::vector<Platform> platforms;
    
    // variáveis da raylib que guardam as imagens de fundo na memória da placa de vídeo
    Texture2D bgMenu;
    Texture2D bgNuvens; 
    Texture2D bgEspaco;
    
    // propriedades geométricas e lógicas do objetivo final do jogo
    Vector2 starPos;
    bool showStar;
    float starRadius;

    // gerencia a stream de áudio contínua (trilha sonora)
    Music bgMusic; 

public:
    // construtor: inicia o dispositivo de áudio, cria a janela da raylib e define o estado inicial como MENU
    Game();
    
    // destrutor: chamado ao fechar o jogo. previne vazamento de memória (memory leak) 
    // ao descarregar as texturas, fechar o dispositivo de áudio e deletar o ponteiro do jogador
    ~Game();
    
    // o método principal da classe. é ele que contém o while principal do jogo (o game loop),
    // controlando a ordem de atualização (update) e desenho (draw) a 60 quadros por segundo
    void Run();
    
    // método gerenciador de cenários. limpa o vetor de plataformas antigo e reconstrói 
    // o design da fase (posições e tamanhos dos blocos) de acordo com o número passado por parâmetro
    void LoadLevel(int level);
};