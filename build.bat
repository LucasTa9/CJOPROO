@echo off
:: Desativa a repetição dos comandos no terminal. Isso mantém a "tela preta" 
:: limpa e focada apenas nas mensagens que você escolheu mostrar.

echo Compilando AventuraSlime...
:: Imprime essa mensagem na tela para você saber que o processo começou.

g++ *.cpp -o AventuraSlime.exe -O2 -Wall -I C:\raylib\raylib\src -L C:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
:: O coração do script. É a chamada real do compilador que transforma o código de texto em software.
:: (Detalhei essa linha inteira logo abaixo).

if %errorlevel% neq 0 (
    :: O Windows guarda o resultado do último comando na variável oculta "%errorlevel%". 
    :: Se for diferente (neq) de zero, significa que o compilador g++ esbarrou em algum erro no seu código C++.
    
    echo.
    echo [ERRO] A compilacao falhou!
    
    pause
    :: Comando vital! Ele congela a tela do terminal pedindo para você "pressionar qualquer tecla". 
    :: Sem isso, a janela do terminal fecharia instantaneamente e você não conseguiria ler qual linha do seu C++ deu erro.
    
    exit /b %errorlevel%
    :: Aborta o script imediatamente. Impede que o sistema tente rodar uma versão antiga ou quebrada do jogo.
)

echo [SUCESSO] Arquivo AventuraSlime.exe gerado.
echo Executando...
:: Se o script passou pelo bloco 'if' sem entrar nele, significa que a compilação foi um sucesso.

AventuraSlime.exe
:: Como a última etapa, ele executa o jogo finalizado para você testar.