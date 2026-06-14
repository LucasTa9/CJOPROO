#include "Slime.h"
#include "Platform.h"
#include <vector>
#include <cmath>

// Define o estado inicial da Slime quando o jogo começa ou a fase reseta
Slime::Slime(float x, float y) {
    position = { x, y };
    velocity = { 0, 0 };
    
    // Define o tamanho da hitbox circular do personagem
    radius = 25.0f; 
    
    // Variáveis que controlam a sensação física do jogo
    gravity = 0.35f;         // Puxa a slime pra baixo a cada frame
    moveSpeed = 1.2f;        // Velocidade da andadinha no chão
    jumpCharge = 0.0f;       // Começa sem nenhuma força acumulada
    maxJumpCharge = 14.0f;   // Limite máximo para a mola não quebrar o jogo
    
    isGrounded = false;
    isCharging = false;
    isStuck = false; 

    // Puxa a imagem da pasta. Importante: tem que dar unload no final pra não dar memory leak
    sprite = LoadTexture("images/Player.png");
}

Slime::~Slime() {
    // Libera a memória da placa de vídeo quando o objeto é destruído
    UnloadTexture(sprite);
}

// Essa função só checa o teclado, ela não aplica a física de fato
void Slime::HandleInput() {
    // Só deixa o jogador se mover se ele estiver apoiado em algo sólido
    if (isGrounded || isStuck) {
        
        // Movimentação básica lateral, só funciona se ele não estiver carregando o pulo
        if (!isCharging) {
            if (IsKeyDown(KEY_RIGHT)) velocity.x = moveSpeed;
            else if (IsKeyDown(KEY_LEFT)) velocity.x = -moveSpeed;
            else velocity.x = 0;
        }

        // Se segurar espaço, trava o eixo X e começa a encher a barrinha de força
        if (IsKeyDown(KEY_SPACE)) {
            isCharging = true;
            velocity.x = 0; 
            if (jumpCharge < maxJumpCharge) jumpCharge += 0.15f; 
        }

        // O momento crítico: soltou o espaço, aplica o tiro
        if (IsKeyReleased(KEY_SPACE) && isCharging) {
            
            // Permite dar um pulinho fraco pra baixo se o jogador segurar a seta pra baixo
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                velocity.y = jumpCharge;
            } else {
                velocity.y = -jumpCharge; // Vai pra cima (negativo no Raylib)
            }
            
            // Controle de direção no ar (o impulso lateral do pulo)
            if (IsKeyDown(KEY_RIGHT)) velocity.x = 3.5f;
            else if (IsKeyDown(KEY_LEFT)) velocity.x = -3.5f;
            // Lógica do Wall-jump: se tiver grudado na parede, joga ele pro lado oposto
            else if (isStuck) {
                velocity.x = (position.x < 400) ? 2.5f : -2.5f; 
            }

            // Reseta todos os estados pra ele não ficar voando ou preso no ar
            jumpCharge = 0.0f;
            isCharging = false;
            isGrounded = false;
            isStuck = false; 
        }
    }
}

// O coração do motor físico. É chamado a cada frame do Game Loop.
void Slime::Update(const std::vector<Platform>& platforms) {
    HandleInput();

    // Aplica a gravidade constantemente, a menos que ele esteja grudado numa parede/teto
    if (!isStuck) {
        velocity.y += gravity;
    } else {
        velocity = { 0, 0 };
    }
    
    // Atualiza a posição baseada na velocidade calculada
    position.x += velocity.x;
    position.y += velocity.y;

    // Zera os estados de colisão pra testar de novo no loop abaixo
    isGrounded = false; 
    isStuck = false; 
    
    // Varre todas as plataformas da fase pra ver se a slime bateu em alguma
    for (const auto& plat : platforms) {
        
        // Tratamento de colisão para as plataformas retangulares (chão e paredes)
        if (!plat.isCircle) {
            
            if (CheckCollisionCircleRec(position, radius, plat.rect)) {
                
                // Acha o meio matemático da plataforma pra calcular por onde a slime bateu
                float centerX = plat.rect.x + plat.rect.width / 2;
                float centerY = plat.rect.y + plat.rect.height / 2;
                
                // Calcula o quanto o círculo da slime entrou dentro do retângulo
                float overlapX = (radius + plat.rect.width / 2) - fabsf(position.x - centerX);
                float overlapY = (radius + plat.rect.height / 2) - fabsf(position.y - centerY);

                // Colisão no eixo X (bateu nas paredes laterais)
                if (overlapX < overlapY) {
                    if (position.x < centerX) position.x = plat.rect.x - radius;
                    else position.x = plat.rect.x + plat.rect.width + radius;
                    
                    // Se estiver caindo e bater na parede, ele gruda. Se estiver subindo, ele só desliza.
                    if (position.y > plat.rect.y) {
                        isStuck = true;
                        position.x += plat.speedX; // Acompanha a plataforma móvel se for o caso
                    } else {
                        isStuck = false; 
                    }
                } 
                // Colisão no eixo Y (bateu no chão ou no teto)
                else {
                    if (position.y < centerY) { 
                        // Caiu no chão certinho
                        position.y = plat.rect.y - radius;
                        velocity.y = 0;
                        isGrounded = true;
                        isStuck = false;
                        position.x += plat.speedX; // Herda a inércia da plataforma móvel
                    } else { 
                        // Bateu a cabeça no teto
                        position.y = plat.rect.y + plat.rect.height + radius;
                        velocity.y = 0;
                        isStuck = true;
                        position.x += plat.speedX; 
                    }
                }
                // Trava o boneco se a lógica disser que ele grudou
                if (isStuck) velocity = { 0, 0 };
            }
        } 
        // Tratamento para esferas flutuantes
        else {
            if (CheckCollisionCircles(position, radius, plat.center, plat.radius)) {
                
                // Usa Pitágoras pra achar a distância entre o centro da slime e o da bolinha
                float dx = position.x - plat.center.x;
                float dy = position.y - plat.center.y;
                float distance = sqrtf(dx * dx + dy * dy);
                
                if (distance > 0) {
                    // Normaliza o vetor pra saber a direção do impacto
                    float normalX = dx / distance;
                    float normalY = dy / distance;
                    
                    // Empurra a slime pra fora da bolinha pra ela não entrar nela
                    float overlap = (radius + plat.radius) - distance;
                    position.x += normalX * overlap;
                    position.y += normalY * overlap;

                    // Se a slime caiu muito perto do topo da bolinha, conta como chão. Senão, ela só gruda.
                    if (normalY < -0.5f) { 
                        velocity.y = 0;
                        isGrounded = true;
                        isStuck = false;
                    } 
                    else { 
                        isStuck = true;
                        velocity = { 0, 0 };
                    }
                }
            }
        }
    }

    // Paredes invisíveis nas laterais da tela pra não deixar a slime cair pro vazio
    if (position.x - radius < 0) {
        position.x = radius; 
        velocity.x = 0;
    }

    if (position.x + radius > 800) {
        position.x = 800 - radius;
        velocity.x = 0;
    }
}

void Slime::Draw() {
    float diameter = radius * 2.0f;

    // Desloca o ponto de desenho porque o raylib desenha a partir do canto superior esquerdo da imagem
    float drawPosX = position.x - radius;
    float drawPosY = position.y - radius;

    // Filtros de cor pra dar um feedback visual pro jogador
    Color tint = WHITE; 
    if (isCharging) tint = LIME;  // Fica verde enquanto carrega o pulo
    if (isStuck) tint = SKYBLUE;  // Fica azul quando gruda na parede

    // Essa função gigantesca pega a textura original e estica pra caber certinho no diâmetro de colisão
    DrawTexturePro(sprite, 
                   (Rectangle){ 0, 0, (float)sprite.width, (float)sprite.height }, 
                   (Rectangle){ drawPosX, drawPosY, diameter, diameter }, 
                   (Vector2){ 0, 0 }, 0.0f, tint); 
    
    // Desenha uma barrinha retangular solta em cima da cabeça indicando a força
    if (isCharging) {
        DrawRectangle(position.x - 25, position.y - 45, jumpCharge * 2.5, 5, RED);
    }
}