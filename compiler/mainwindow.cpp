#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    c = new CompilerMainWindow(this);
    this->setCentralWidget(c);
    this->setWindowTitle("Compiler for MyLang");
}

MainWindow::~MainWindow()
{

}
