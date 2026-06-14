#include "Game.h"

// ponto de partida obrigatório de qualquer programa escrito em c++
int main() {
    
    // cria o objeto principal do jogo na memória. 
    // é exatamenta nessa linha que o construtor da classe Game entra em ação,
    // configurando a tela inicial e abrindo a janela gráfica do raylib.
    Game aventuraSlime;
    
    // chama o método que segura o programa no "game loop". 
    // o código fica travado executando essa função (atualizando a física e desenhando os frames) 
    // e só passa para a próxima linha quando o jogador clica no X para fechar a janela.
    aventuraSlime.Run();
    
    // avisa para o sistema operacional que o software foi encerrado com sucesso e sem travamentos (código zero)
    return 0;
}