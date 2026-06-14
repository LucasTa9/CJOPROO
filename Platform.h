#pragma once // faz a mesma coisa que o ifndef, impede que o arquivo seja importado duas vezes e quebre a compilação

#include "raylib.h"

class Platform {
public:
    // deixamos os atributos públicos aqui propositalmente, porque a classe slime 
    // precisa ler essas formas geométricas o tempo todo para calcular a colisão
    
    // atributos usados se a plataforma for um bloco retangular
    Rectangle rect;
    
    // atributos usados se a plataforma for uma bolinha flutuante
    bool isCircle;  // chave que avisa ao motor físico qual lógica de colisão ele deve aplicar
    Vector2 center; // ponto central da esfera no mapa
    float radius;   // o tamanho da bolinha

    Color color;    // cor de preenchimento para a renderização na tela

    // variáveis exclusivas para as plataformas móveis
    float speedX;   // a velocidade atual e a direção
    float minX;     // limite esquerdo do mapa onde ela bate e volta
    float maxX;     // limite direito onde ela bate e volta

    // construtores: aqui aplicamos a sobrecarga de métodos. 
    // o sistema sabe qual plataforma criar com base na quantidade de parâmetros que passamos.

    // constrói um bloco estático normal que não se mexe
    Platform(float x, float y, float w, float h, Color c);
    
    // constrói um bloco dinâmico que fica patrulhando de um lado para o outro
    Platform(float x, float y, float w, float h, Color c, float vel, float limiteEsq, float limiteDir);

    // constrói um obstáculo esférico no cenário
    Platform(Vector2 centro, float raio, Color c);

    // métodos principais do game loop para o cenário
    void Update(); // atualiza a matemática de movimentação (só afeta as plataformas móveis)
    void Draw();   // desenha a forma certa na tela (círculo ou retângulo) lendo a variável isCircle
};