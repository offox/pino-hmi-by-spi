#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "spicomm.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SPIComm spiComm;
}

MainWindow::~MainWindow()
{
    delete ui;
}
