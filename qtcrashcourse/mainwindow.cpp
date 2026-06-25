#include "mainwindow.h"
#include "ui_mainwindow.h"

// construtor da janela principal
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // carrega e inicializa os elementos visuais desenhados no qt designer
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// variaveis globais pra armazenar os valores e o resultado
int firstNum, secondNum, result;

// evento de clique do botao de adicao
void MainWindow::on_btnAdd_clicked()
{
    // pega os valores digitados nos campos e converte pra inteiro
    firstNum = ui->txtFirstNum->text().toInt();
    secondNum = ui->txtSecondNum->text().toInt();
    result = firstNum + secondNum;

    // transforma o resultado em texto de novo e mostra no campo correspondente
    ui->txtResult->setText(QString::number(result));
}

// evento de clique do botao de subtracao
void MainWindow::on_btnSubtract_clicked()
{
    // pega os valores digitados nos campos e converte pra inteiro
    firstNum = ui->txtFirstNum->text().toInt();
    secondNum = ui->txtSecondNum->text().toInt();
    result = firstNum - secondNum;
    ui->txtResult->setText(QString::number(result));
}

// evento de clique do botao de divisao
void MainWindow::on_btnDivide_clicked()
{
    // pega os valores digitados nos campos e converte pra inteiro
    firstNum = ui->txtFirstNum->text().toInt();
    secondNum = ui->txtSecondNum->text().toInt();
    result = firstNum / secondNum;
    ui->txtResult->setText(QString::number(result));
}

// evento de clique do botao de adicao
void MainWindow::on_btnMultiply_clicked()
{
    firstNum = ui->txtFirstNum->text().toInt();
    secondNum = ui->txtSecondNum->text().toInt();
    result = firstNum * secondNum;

    ui->txtResult->setText(QString::number(result));
}




