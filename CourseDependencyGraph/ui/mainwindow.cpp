#include <QLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CourseInfoWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dependencyManager(new DependencyManager) {
	ui->setupUi(this);

    this->dependencyManager->loadCSV();
    this->dependencyManager->linkCourses();

    CourseInfoWidget *courseInfoWidget = new CourseInfoWidget(this->ui->courseInfoWidget);


	setupTreeView();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTreeView(){
	ui->treeWidget->setHeaderLabel("Courses");
	ui->treeWidget->setColumnCount(1);

	vector<QString> subjectVector;
	vector<CourseCode> courseVector;
	dependencyManager->courses.toKeyVector(subjectVector);
	for (unsigned int i = 0; i < subjectVector.size(); ++i){
		QTreeWidgetItem* root = treeViewAddRoot(subjectVector[i]);
		courseVector.clear();
		dependencyManager->courses.find(subjectVector[i])->toKeyVector(courseVector);
		for (unsigned int j = 0; j < courseVector.size(); ++j){
			treeViewAddChild(root, QString::number(courseVector[j].getCode()));
		}
	}
}

QTreeWidgetItem* MainWindow::treeViewAddRoot(QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
	item->setText(0, string);
	ui->treeWidget->addTopLevelItem(item);
	return item;
}

void MainWindow::treeViewAddChild(QTreeWidgetItem *parent, QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, string);
	parent->addChild(item);
}
