#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dependencyManager(new DependencyManager) {
    ui->setupUi(this);

    this->dependencyManager->loadCSV();
}

MainWindow::~MainWindow()
{
    delete ui;
}
