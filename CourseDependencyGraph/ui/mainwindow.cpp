#include <QDebug>
#include <QTreeWidgetItemIterator>
#include <QLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CourseInfoWidget.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dependencyManager(new DependencyManager){
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);
    this->dependencyManager->loadCSV();
    this->dependencyManager->linkCourses();

    this->courseInfoWidget = new CourseInfoWidget(this->ui->courseInfoWidget,
                                                  &this->dependencyManager->courses,
                                                  nullptr,
                                                  this->ui->treeWidget);

    connect(this->ui->searchBar, SIGNAL(textChanged(const QString&)),
            this, SLOT(searchCourse(const QString&)));
    connect(this->ui->searchBar, SIGNAL(returnPressed()),
            this, SLOT(searchEnterPressed()));

	setupTreeView();
}



MainWindow::~MainWindow() {
    delete this->dependencyManager;
    delete this->courseInfoWidget;
    delete ui;
}

void MainWindow::setupTreeView(){
	ui->treeWidget->setHeaderLabel("Courses");
	ui->treeWidget->setColumnCount(1);
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()),
			this, SLOT(treeWidgetItemClicked()));\

	vector<QString> subjectVector;
	vector<CourseCode> courseVector;
	dependencyManager->courses.toKeyVector(subjectVector);
	for (unsigned int i = 0; i < subjectVector.size(); ++i){
		QTreeWidgetItem* root = treeViewAddRoot(subjectVector[i]);
		courseVector.clear();
		dependencyManager->courses.find(subjectVector[i])->toKeyVector(courseVector);
		for (unsigned int j = 0; j < courseVector.size(); ++j){
            treeViewAddChild(root, QString::number(courseVector[j].getCode()) + courseVector[j].getExtension());
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

void MainWindow::searchCourse(const QString &text) {
    this->ui->treeWidget->clearSelection();
    this->ui->treeWidget->collapseAll();
    QString keyword = text.toUpper();
    keyword.remove(" ");
    if (keyword == "") { return; }

    QTreeWidgetItemIterator iterator { this->ui->treeWidget };
    while ((*iterator)->text(0) < keyword.left(4)) {
        ++iterator;
    }

    QTreeWidgetItem *subjectItem = *iterator;
    if (!subjectItem->text(0).startsWith(keyword.left(4))) { return; }
    subjectItem->setSelected(true);
    this->ui->treeWidget->setCurrentItem(subjectItem);

    if (subjectItem->text(0) != keyword.left(4)) { return; }
    this->ui->treeWidget->expandItem(subjectItem);

    ++iterator;
    while ((*iterator)->text(0) < keyword.mid(4)) {
        ++iterator;
    }
    QTreeWidgetItem *codeItem = *iterator;

    if (!codeItem->text(0).startsWith(keyword.mid(4))) { return; }
    this->ui->treeWidget->clearSelection();
    codeItem->setSelected(true);
    this->ui->treeWidget->setCurrentItem(codeItem);
}


void MainWindow::searchEnterPressed() {
    QList<QTreeWidgetItem *> items = this->ui->treeWidget->selectedItems();
    if (items.size() == 0) { return; }
    QTreeWidgetItem *item = items.first();
    if (item == nullptr) { return; }

    QString keyword = this->ui->searchBar->text().toUpper();
    keyword.remove(" ");

    if (item->parent() == nullptr) {
        if (item->text(0).startsWith(keyword.left(4))) {
            this->ui->searchBar->setText(item->text(0));
        }
    } else {
        if (item->text(0).startsWith(keyword.mid(4))) {
            this->ui->searchBar->setText(item->parent()->text(0) + item->text(0));
        }
    }

}


QGraphicsProxyWidget* MainWindow::addCourseLabel(QString name, qreal x, qreal y){
	QGraphicsProxyWidget *label = scene->addWidget(new CourseLabel(nullptr, name));

	courseLabels.add(name, label);
    label->setPos(x, y);
	label->setZValue(1);
	static const qreal Y_OFFSET = 30;
	static const qreal X_OFFSET = 120;
	label->installEventFilter(this);
	static const qreal MARGIN = 20;
	scene->addRect(x-MARGIN, y-MARGIN, 1, 1, QPen(Qt::transparent), QBrush(Qt::transparent));
	scene->addRect(x+X_OFFSET + MARGIN, y + Y_OFFSET + MARGIN, 1, 1, QPen(Qt::transparent), QBrush(Qt::transparent));
    return label;
}

void MainWindow::connectCourseLabels(QGraphicsProxyWidget* from, QGraphicsProxyWidget* to, Qt::GlobalColor color){
    static const qreal Y_OFFSET = 16;
    static const qreal X_OFFSET = 61;
    QPen pen(color);
    pen.setWidth(2);
    scene->addLine(from->x() + X_OFFSET, from->y() + Y_OFFSET,
                   to->x() + X_OFFSET, to->y() + Y_OFFSET, pen);
}

void MainWindow::clearCourseLabel(){
	vector<QGraphicsProxyWidget*> courseLabelVector;
	courseLabels.toVector(courseLabelVector);
	for (QGraphicsProxyWidget *courseLabel: courseLabelVector){
		scene->removeItem(courseLabel);
	}
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event){
	if (event->type() == QEvent::FocusIn){
		// parsing courselabel from QObject
		QString name = static_cast<CourseLabel*>(static_cast<QGraphicsProxyWidget*>(obj)->widget())->getName();
		ui->searchBar->setText(name);
		return true;
	}
	return false;
}

void MainWindow::treeWidgetItemClicked(){
	QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
	if (items.size() == 0) { return; }
	QTreeWidgetItem *item = items.first();
	if (item == nullptr) { return; }
	QTreeWidgetItem *subject = item->parent();
	if (subject == nullptr) { return; }

	AVLTree<CourseCode, Course* >* subjectTree = dependencyManager->courses.find(subject->text(0));
	if (subjectTree == nullptr) { return; }

	CourseCode *courseCode = CourseCode::create(subject->text(0) + item->text((0)));
	this->courseInfoWidget->treeWidgetItemClicked(courseCode);
	delete courseCode;
	return;
}
