#include "Platform.h"

// construtor da plataforma estática. ideal para montar o chão, tetos e paredes normais da fase
Platform::Platform(float x, float y, float w, float h, Color c) {
    rect = { x, y, w, h };
    isCircle = false; // garante que o motor físico vai tratar esse objeto como um bloco quadrado
    color = c;
    
    // como ela fica parada, a velocidade e os limites de patrulha ficam iguais à posição de origem
    speedX = 0.0f; 
    minX = x; 
    maxX = x;
}

// construtor da plataforma dinâmica. além da estrutura base, pede a velocidade e o percurso
Platform::Platform(float x, float y, float w, float h, Color c, float vel, float limiteEsq, float limiteDir) {
    rect = { x, y, w, h };
    isCircle = false;
    color = c;
    speedX = vel; 
    minX = limiteEsq; // ponto máximo que ela pode ir para a esquerda antes de voltar
    maxX = limiteDir; // ponto máximo que ela pode ir para a direita
}

// construtor exclusivo para os obstáculos esféricos do cenário
Platform::Platform(Vector2 centro, float raio, Color c) {
    center = centro;
    radius = raio;
    isCircle = true; // essa flag é fundamental para avisar a classe slime para usar a colisão radial (pitágoras)
    color = c;
    speedX = 0.0f; // no design atual do nível, as bolinhas não se movem
}

// processa a lógica de movimento. como só as plataformas dinâmicas andam, fazemos uma filtragem
void Platform::Update() {
    // checa se o objeto tem velocidade e se é um bloco retangular
    if (speedX != 0.0f && !isCircle) {
        rect.x += speedX; // desloca a plataforma no eixo x
        
        // se a plataforma encostar no limite do percurso (minX ou maxX), inverte o vetor da velocidade
        // multiplicar por -1 faz ela bater e voltar infinitamente
        if (rect.x > maxX || rect.x < minX) {
            speedX *= -1.0f; 
        }
    }
}

// renderização condicional: lê a estrutura do objeto para saber como ele deve aparecer na tela
void Platform::Draw() {
    if (isCircle) {
        // chama a função nativa da raylib otimizada para desenhar esferas vetoriais
        DrawCircleV(center, radius, color); 
    } else {
        // chama a função nativa da raylib otimizada para preencher retângulos na tela
        DrawRectangleRec(rect, color);      
    }
}