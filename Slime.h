#ifndef SLIME_H
#define SLIME_H

// previne que o compilador leia este arquivo de cabeçalho mais de uma vez, evitando erro de redefinição
#include "raylib.h"
#include "Platform.h"
#include <vector>

class Slime {
private:
    // os atributos ficam escondidos aqui dentro (privados) para respeitar o encapsulamento
    Vector2 position; // guarda a coordenada x e y atual do personagem na tela
    Vector2 velocity; // controla a inércia, ou seja, para onde e com que força ele está indo
    float radius;     // define o tamanho da bolinha invisível de colisão (hitbox)
    bool isStuck;     // avisa para o sistema se o jogador está grudado em uma parede

    Texture2D sprite; // variável da raylib que segura a imagem png carregada na placa de vídeo
    
    // variáveis que configuram a sensação de peso, pulo e velocidade da física do jogo
    float gravity;
    float moveSpeed;        
    float jumpCharge;       // a força do pulo que está sendo acumulada no momento
    float maxJumpCharge;    // o limite máximo da mola para a slime não sair voando do mapa
    bool isGrounded;        // variável de controle que avisa se a slime está pisando em terreno seguro
    bool isCharging;        // avisa se o jogador está segurando a tecla de espaço agora

public:
    // construtor: é chamado automaticamente quando a slime nasce no nível
    Slime(float x, float y);
    
    // destrutor: chamado quando a fase acaba ou o jogo fecha, serve para limpar a textura da memória ram
    ~Slime();

    // os três métodos principais que rodam a cada quadro dentro do game loop
    void Update(const std::vector<Platform>& platforms); // processa a matemática da gravidade e as colisões
    void Draw();                                         // renderiza a imagem e a barrinha de força visualmente
    void HandleInput();                                  // lê os botões do teclado e altera as velocidades

    // getters e setters
    // como a posição e o raio são privados, a classe game precisa dessas funções para conseguir ler os valores de fora
    Vector2 GetPosition() { return position; }
    void SetPosition(Vector2 pos) { position = pos; }
    float GetRadius() { return radius; }
};

#endif