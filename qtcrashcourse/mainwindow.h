#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


// classe principal da janela
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // funcoes que disparam quando o usuario clica nos botoes
    void on_btnAdd_clicked();

    void on_btnSubtract_clicked();

    void on_btnDivide_clicked();

    void on_btnMultiply_clicked();

private:
    // ponteiro que faz a ponte com a interface grafica gerada pelo qt designer
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
