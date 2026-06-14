#include "Game.h"
#include <cmath>

// ================================================================
// CONSTRUTOR
// ================================================================
Game::Game() {
    InitWindow(screenWidth, screenHeight, "Aventura Slime - O Desafio Final");
    SetTargetFPS(60);
    
    SetExitKey(0); // Isso impede que o ESC feche a janela do jogo!

    InitAudioDevice(); 
    bgMusic = LoadMusicStream("summit_of_winds.mp3");
    bgMusic.looping = true; 
    PlayMusicStream(bgMusic);

    // --- CARREGANDO AS IMAGENS DE FUNDO (Caminhos e extensões corrigidos!) ---
    bgMenu = LoadTexture("images/BackgroundMenu.png");
    bgNuvens = LoadTexture("images/Background5a8.png"); // Fases 1 a 4 (Céu)
    bgEspaco = LoadTexture("images/BackGround1a4.png"); // Fases 5 a 8 (Espaço)
    // --------------------------------------

    currentState = MENU; 
    currentLevel = 1;
    starRadius = 50.0f;
    showStar = false;
    
    player = new Slime(screenWidth / 2.0f, screenHeight - 100.0f);
    LoadLevel(currentLevel);
}

// ================================================================
// DESTRUTOR
// ================================================================
Game::~Game() {
    delete player;
    platforms.clear();
    
    // --- DESCARREGANDO OS RECURSOS DA MEMÓRIA ---
    UnloadTexture(bgMenu);
    UnloadTexture(bgNuvens);
    UnloadTexture(bgEspaco);

    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    // --------------------------------------------
    
    CloseWindow();
}

// ================================================================
// RUN: Loop Principal
// ================================================================
void Game::Run() {
    bool sairDoJogo = false;

    while (!WindowShouldClose() && !sairDoJogo) {
        
        UpdateMusicStream(bgMusic); 

        Vector2 mousePos = GetMousePosition();
        Rectangle btnStart = { 250, 200, 300, 40 };
        Rectangle btnDev   = { 250, 260, 300, 40 };
        Rectangle btnSair  = { 250, 320, 300, 40 };

        // --- 1. LÓGICA DE PROCESSAMENTO ---
        switch (currentState) {
            case MENU:
                if (CheckCollisionPointRec(mousePos, btnStart) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentLevel = 1;
                    LoadLevel(currentLevel);
                    currentState = PLAYING;
                }
                if (CheckCollisionPointRec(mousePos, btnDev) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentState = LEVEL_SELECT;
                }
                if (CheckCollisionPointRec(mousePos, btnSair) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    sairDoJogo = true; 
                }
                if (IsKeyPressed(KEY_ESCAPE)) sairDoJogo = true;
                break;

            case LEVEL_SELECT:
                if (IsKeyPressed(KEY_UP)) currentLevel++;
                if (IsKeyPressed(KEY_DOWN) && currentLevel > 1) currentLevel--;
                if (currentLevel > 8) currentLevel = 8; 

                if (IsKeyPressed(KEY_ENTER)) {
                    LoadLevel(currentLevel);
                    player->SetPosition({ 400, 300 }); 
                    currentState = PLAYING;
                }
                if (IsKeyPressed(KEY_ESCAPE)) currentState = MENU;
                break;

            case PLAYING:
                for (auto& plat : platforms) plat.Update();
                player->Update(platforms);

                if (showStar) {
                    if (CheckCollisionCircles(player->GetPosition(), player->GetRadius(), starPos, starRadius)) {
                        currentState = ENDING;
                    }
                }

                if (player->GetPosition().y + player->GetRadius() < 0 && currentLevel < 8) {
                    currentLevel++;
                    LoadLevel(currentLevel);
                    float novaPosy = (float)screenHeight - player->GetRadius() - 5;
                    player->SetPosition({ player->GetPosition().x, novaPosy });
                }

                if (player->GetPosition().y - player->GetRadius() > screenHeight) {
                    if (currentLevel > 1) {
                        currentLevel--;
                        LoadLevel(currentLevel);
                        player->SetPosition({ player->GetPosition().x, 10 });
                    } else {
                        player->SetPosition({ (float)screenWidth / 2, 100 });
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                    currentState = MENU;
                }
                break;

            case ENDING:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                    currentLevel = 1;
                    LoadLevel(currentLevel);
                    player->SetPosition({ screenWidth / 2.0f, screenHeight - 100.0f });
                    currentState = MENU;
                }
                break;
        }

        // --- 2. DESENHO ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // --- NOVA LÓGICA DE DESENHO DE FUNDOS (CORRIGINDO O ZOOM) ---
            Texture2D bgToDraw;
            bool shouldDrawBg = false;

            if (currentState == MENU || currentState == LEVEL_SELECT || currentState == ENDING) {
                bgToDraw = bgMenu;
                shouldDrawBg = true;
            } 
            else if (currentState == PLAYING) {
                shouldDrawBg = true;
                if (currentLevel >= 1 && currentLevel <= 4) {
                    bgToDraw = bgNuvens;
                } else {
                    bgToDraw = bgEspaco;
                }
            }

            if (shouldDrawBg) {
                // Pega a imagem inteira
                Rectangle sourceRec = { 0.0f, 0.0f, (float)bgToDraw.width, (float)bgToDraw.height };
                // Estica para o tamanho da tela do jogo
                Rectangle destRec = { 0.0f, 0.0f, (float)screenWidth, (float)screenHeight };
                DrawTexturePro(bgToDraw, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }
            // ------------------------------------------------------------

            switch (currentState) {
                case MENU:
                    DrawText("AVENTURA SLIME", 220, 100, 40, GREEN);

                    DrawRectangleRec(btnStart, CheckCollisionPointRec(mousePos, btnStart) ? LIGHTGRAY : GRAY);
                    DrawText("INICIAR JOGO", btnStart.x + 80, btnStart.y + 10, 20, WHITE);

                    DrawRectangleRec(btnDev, CheckCollisionPointRec(mousePos, btnDev) ? LIGHTGRAY : GRAY);
                    DrawText("SELETOR DE FASES", btnDev.x + 50, btnDev.y + 10, 20, WHITE);

                    DrawRectangleRec(btnSair, CheckCollisionPointRec(mousePos, btnSair) ? RED : MAROON);
                    DrawText("SAIR", btnSair.x + 125, btnSair.y + 10, 20, WHITE);
                    break;

                case LEVEL_SELECT:
                    DrawText("SELETOR DE FASES", 250, 100, 30, BLUE);
                    DrawText(TextFormat("FASE SELECIONADA: %i", currentLevel), 240, 200, 25, WHITE);
                    DrawText("SETAS para mudar | ENTER para jogar", 210, 300, 20, LIGHTGRAY);
                    DrawText("ESC para voltar ao Menu", 280, 340, 15, WHITE);
                    break;

                case PLAYING:
                    DrawText(TextFormat("FASE: %i", currentLevel), 10, 10, 20, WHITE);
                    DrawText("ESC para Menu", 670, 10, 15, WHITE); 
                    
                    for (auto& plat : platforms) plat.Draw();
                    
                    if (showStar) {
                        float pulse = sin(GetTime() * 4.0f) * 4.0f;
                        DrawCircleV(starPos, starRadius + pulse, GOLD);
                        DrawCircleV(starPos, (starRadius + pulse) * 0.6f, YELLOW);
                    }
                    player->Draw();
                    break;

                case ENDING:
                    DrawText("PARABENS!", 280, 120, 50, GOLD);
                    DrawText("Recolheste a Estrela do Ceu!", 230, 200, 25, WHITE);
                    DrawText("Obrigado por jogar Aventura Slime.", 215, 260, 20, LIGHTGRAY);
                    DrawText("ESC para recomecar", 270, 380, 20, WHITE);
                    DrawCircle(400, 330, 25, GREEN);
                    break;
            }
        EndDrawing();
    }
}

// ================================================================
// LOADLEVEL: Definição das 8 Fases
// ================================================================
void Game::LoadLevel(int level) {
    platforms.clear();
    showStar = false;

    if (level == 1) {
        platforms.push_back(Platform(0, 440, 800, 10, DARKGRAY));
        platforms.push_back(Platform(70, 370, 130, 80, DARKGRAY));
        platforms.push_back(Platform(40, 250, 80, 200, DARKGRAY));
        platforms.push_back(Platform(320, 250, 140, 80, DARKGRAY));
        platforms.push_back(Platform(520, 160, 180, 290, DARKGRAY));
    }
    else if (level == 2) {
        platforms.push_back(Platform(280, 400, 160, 20, DARKGRAY));
        platforms.push_back(Platform(50, 180, 20, 150, DARKGRAY));
        platforms.push_back(Platform(80, 80, 130, 20, DARKGRAY));
        platforms.push_back(Platform(280, 160, 130, 20, DARKGRAY));
        platforms.push_back(Platform(460, 240, 50, 50, DARKGRAY));
        platforms.push_back(Platform(570, 210, 140, 20, DARKGRAY));
    }
    else if (level == 3) {
        platforms.push_back(Platform(40, 180, 100, 270, DARKGRAY));
        platforms.push_back(Platform(220, 220, 80, 230, DARKGRAY));
        platforms.push_back(Platform(450, 160, 160, 60, DARKGRAY));
        platforms.push_back(Platform(650, 60, 120, 50, DARKGRAY));
    }
 else if (level == 4) {

    // Início
    platforms.push_back(Platform(10, 300, 100, 20, DARKGRAY));

    // Primeira móvel
    platforms.push_back(
        Platform(180, 350, 120, 20, DARKGRAY,
                 1.0f, 150, 400));

    // Segunda móvel
    platforms.push_back(
        Platform(500, 220, 120, 20, DARKGRAY,
                 -1.2f, 350, 650));

    // Plataforma intermediária
    platforms.push_back(
        Platform(350, 130, 100, 20, DARKGRAY));

    // Plataforma final perto do topo esquerdo
    platforms.push_back(
        Platform(50, 60, 150, 20, DARKGRAY));
}
   else if (level == 5) {

    // Início
    platforms.push_back(Platform(20, 400, 100, 20, DARKGRAY));

    // Escalada inicial
    platforms.push_back(Platform({180, 340}, 22.0f, DARKGRAY));
    platforms.push_back(Platform({320, 280}, 22.0f, DARKGRAY));

    // Área segura
    platforms.push_back(Platform(420, 250, 120, 20, DARKGRAY));

    // Plataforma móvel
    platforms.push_back(
        Platform(550, 190, 100, 20, RED,
                 1.5f, 450, 650));

    // Plataforma de descanso
    platforms.push_back(Platform(300, 140, 120, 20, DARKGRAY));

    // Último desafio
    platforms.push_back(Platform({550, 100}, 22.0f, DARKGRAY));

    // Chegada
    platforms.push_back(Platform(620, 70, 140, 20, DARKGRAY));
}
else if (level == 6) {
    // Início
    platforms.push_back(Platform(250, 430, 120, 20, DARKGRAY));

    // Salto longo para a direita
    platforms.push_back(Platform(560, 360, 120, 20, DARKGRAY));

    // Bola para mudar direção
    platforms.push_back(Platform({650, 250}, 20.0f, DARKGRAY));

    // Plataforma segura à esquerda
    platforms.push_back(Platform(280, 210, 140, 20, DARKGRAY));

    // Bola alta
    platforms.push_back(Platform({150, 140}, 20.0f, DARKGRAY));

    // Plataforma final
    platforms.push_back(Platform(520, 80, 140, 20, DARKGRAY));
}
 else if (level == 7) {

    // Início
    platforms.push_back(Platform(340, 430, 120, 20, DARKGRAY));

    // Plataforma móvel 1
    platforms.push_back(Platform(250, 340, 120, 20, RED, 1.2f, 150, 450));

    // Área segura
    platforms.push_back(Platform(500, 290, 100, 20, DARKGRAY));

    // Esfera
    platforms.push_back(Platform({250, 220}, 20.0f, DARKGRAY));

    // Área segura
    platforms.push_back(Platform(400, 180, 120, 20, DARKGRAY));

    // Esfera alta
    platforms.push_back(Platform({620, 130}, 20.0f, DARKGRAY));

    // Plataforma móvel final
    platforms.push_back(Platform(350, 80, 120, 20, RED, -1.3f, 250, 600));
}
   else if (level == 8) {

    // Início
    platforms.push_back(Platform(50, 380, 150, 20, DARKGRAY));

    // Plataforma móvel central
    platforms.push_back(Platform(250, 320, 120, 20, RED, 1.0f, 200, 500));

    // Plataforma segura
    platforms.push_back(Platform(550, 260, 140, 20, DARKGRAY));

    // Esfera final
    platforms.push_back(Platform({400, 180}, 22.0f, DARKGRAY));

    // Plataforma da estrela
    platforms.push_back(Platform(320, 130, 180, 20, DARKGRAY));

    showStar = true;
    starPos = { 410, 80 };
}
}