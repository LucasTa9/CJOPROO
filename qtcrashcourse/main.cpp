#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // inicializa a aplicacao qt gerenciando recursos e eventos
    QApplication a(argc, argv);

    // instancia a nossa janela principal
    MainWindow w;

    // exibe a janela na tela
    w.show();

    // entra no loop de execucao do programa e espera a interacao
    return a.exec();
}
