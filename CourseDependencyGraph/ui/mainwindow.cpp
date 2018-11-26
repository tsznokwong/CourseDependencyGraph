#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dependencyManager(new DependencyManager) {
	ui->setupUi(this);

    this->dependencyManager->loadCSV();

	setupTreeView();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTreeView(){
	ui->treeWidget->setColumnCount(1);
}

void MainWindow::treeViewAddRoot(QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
	item->setText(0, string);
	treeViewAddChild(item, "asdfasdfa");
	treeViewAddChild(item, "asdfasdfa");
	treeViewAddChild(item, "asdfasdfa");
	treeViewAddChild(item, "asdfasdfa");
	ui->treeWidget->addTopLevelItem(item);
}

void MainWindow::treeViewAddChild(QTreeWidgetItem *parent, QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, string);
	parent->addChild(item);
}
