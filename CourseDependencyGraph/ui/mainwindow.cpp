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

	vector<QString> subjectVector;
	dependencyManager->courses.toKeyVector(subjectVector);
	for (unsigned int i = 0; i < subjectVector.size(); ++i){
		treeViewAddRoot(subjectVector[i]);
	}
}

void MainWindow::treeViewAddRoot(QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
	item->setText(0, string);
	ui->treeWidget->addTopLevelItem(item);
}

void MainWindow::treeViewAddChild(QTreeWidgetItem *parent, QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, string);
	parent->addChild(item);
}
