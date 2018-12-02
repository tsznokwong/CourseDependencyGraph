#include <QDebug>
#include <QTreeWidgetItemIterator>
#include <QLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CourseInfoWidget.h"
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dependencyManager(new DependencyManager){
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
	ui->graphicsView->setScene(scene);
    this->dependencyManager->loadCSV();
    this->dependencyManager->linkCourses();

	// add a courseinfowidget on the right hand side of the screen and link it to the treewidget on the lefthand side
    this->courseInfoWidget = new CourseInfoWidget(this->ui->courseInfoWidget,
                                                  &this->dependencyManager->courses,
                                                  nullptr,
                                                  this->ui->treeWidget);

	// linking searchBar and corresponding member functions
    connect(this->ui->searchBar, SIGNAL(textChanged(const QString&)),
            this, SLOT(searchCourse(const QString&)));
    connect(this->ui->searchBar, SIGNAL(returnPressed()),
            this, SLOT(searchEnterPressed()));

	// populate the Tree View on the left
	setupTreeView();
}



MainWindow::~MainWindow() {
    delete this->dependencyManager;
    delete this->courseInfoWidget;
    delete ui;
}

void MainWindow::setupTreeView(){
	ui->treeWidget->setHeaderLabel("Courses");
	ui->treeWidget->setColumnCount(1); // only one column is needed
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(treeWidgetItemClicked()));

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

/***
 * Adds an item to the tree view and returns it
 */
QTreeWidgetItem* MainWindow::treeViewAddRoot(QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
	item->setText(0, string);
	ui->treeWidget->addTopLevelItem(item);
	return item;
}


/***
 * Adds a child to root *parent
 */
void MainWindow::treeViewAddChild(QTreeWidgetItem *parent, QString string){
	QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, string);
	parent->addChild(item);
}


/***
 * searches for course and highlights it in treeview
 */
void MainWindow::searchCourse(const QString &text) {
	// reset the treeview
    this->ui->treeWidget->clearSelection();
    this->ui->treeWidget->collapseAll();

    QString keyword = text.toUpper();
    keyword.remove(" ");
    if (keyword == "") { return; }

	//go to the correct root (subject) base on first 4 characters in search bar
    QTreeWidgetItemIterator iterator { this->ui->treeWidget };
    while ((*iterator)->text(0) < keyword.left(4)) {
        ++iterator;
    }

	//highlight the corresponding root
    QTreeWidgetItem *subjectItem = *iterator;
    if (!subjectItem->text(0).startsWith(keyword.left(4))) { return; }
    subjectItem->setSelected(true);
    this->ui->treeWidget->setCurrentItem(subjectItem);

	//selected the corresponding root when 4 characters are inputted and an exact match is found
    if (subjectItem->text(0) != keyword.left(4)) { return; }
    this->ui->treeWidget->expandItem(subjectItem);

	//go to correct child
    ++iterator;
    while ((*iterator)->text(0) < keyword.mid(4)) {
        ++iterator;
    }
    QTreeWidgetItem *codeItem = *iterator;

	//select best-fit child
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

	//auto fill in the search bar base on current highlighted item
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
	if (printedLabels.contains(name)) return printedLabels.find(name);
	QGraphicsProxyWidget *label = scene->addWidget(new CourseLabel(nullptr, name));

	static const qreal MARGIN = 20;
	label->setPos(x+MARGIN, y+MARGIN);
	label->setZValue(1);
	static const qreal Y_OFFSET = 30;
	static const qreal X_OFFSET = 120;
	label->installEventFilter(this);
	QRectF boundingRect = scene->itemsBoundingRect();
	boundingRect.adjust(-20, -20, 20, 29);
	scene->setSceneRect(boundingRect);
	if (boundingRect.height() < 500){
		boundingRect.setTop(250);
		boundingRect.setBottom(-250);
	}
	if (boundingRect.width() < 400){
		boundingRect.setTop(200);
		boundingRect.setBottom(-200);
	}
	ui->graphicsView->fitInView(boundingRect, Qt::KeepAspectRatioByExpanding);
	printedLabels.add(name, label);
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
	scene->clear();
	printedLabels.clear();
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
	clearCourseLabel();
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
	selectedCourse = dependencyManager->findCourse(courseCode->description());
	delete courseCode;
	addCourseLabel(selectedCourse->getCourseCode().description(), 0, 0);
	if (dependencyGraph == nullptr){
		dependencyGraph = new DependencyGraph<QString, Course*>(selectedCourse->getCourseCode().description(), selectedCourse);
	}
	else
		dependencyGraph->reset(selectedCourse->getCourseCode().description(), selectedCourse);
	pushPreRequisite(selectedCourse);
	AVLTree<int, vector<Course*>> map;
	dependencyGraph->getNodesInMap(map);
	printPreRequisite(map, 0, selectedCourse);

    std::vector<int> depths;
    map.toKeyVector(depths);
    for (int depth: depths) {
        qDebug() << depth;
        for (Course* course: map.find(depth)) {
            qDebug() << course->getCourseCode().description();
        }
    }

	return;
}

void MainWindow::pushPreRequisite(Course* course){
	for (Course* nextCourse: course->getPrerequisite().getEdges()){
		dependencyGraph->addNode(nextCourse->getCourseCode().description(), nextCourse);
        dependencyGraph->addEdge(course->getCourseCode().description(), nextCourse->getCourseCode().description(), dependencyGraph->DependencyGraph::Direction::PREVIOUS, Relationship::Type::PREREQUISITE);
		pushPreRequisite(nextCourse);
	}
}



int MainWindow::printPreRequisite(AVLTree<int, vector<Course*>> &map, int depth, Course* parent,
								   qreal xOffset, qreal yOffset){
	int maxDepth = map.min().key;
	 qreal Y_OFFSET_PER_BLOCK = 25*pow(2,depth-maxDepth);
	 static int X_OFFSET_PER_BLOCK = 200;
	 //if (!map.contains(depth)) return 0;



	 vector<Course*> courses = parent->getPrerequisite().getEdges();
	 int child_size = 0;
	 for (int i = 0; i < courses.size(); ++i){
		 int childDepth;
		 for (int j = map.min().key; j < map.max().key; ++j){
			 if (map.contains(j) && std::find(map.find(j).begin(), map.find(j).end(), courses[i])!= map.find(j).end()){
				childDepth = j;
				qDebug() << courses[i]->getCourseCode().description() << " " << childDepth;
			 }
		 }
		 vector<QGraphicsProxyWidget* > childChildrenLabels;
		 addCourseLabel(courses[i]->getCourseCode().description(), xOffset-((depth-childDepth)*X_OFFSET_PER_BLOCK), yOffset + (i - 0.5 * (courses.size()-1) )*Y_OFFSET_PER_BLOCK);
		 int tree_size = printPreRequisite(map, childDepth, courses[i],
										   xOffset-((depth-childDepth)*X_OFFSET_PER_BLOCK) ,  yOffset + (i - 0.5 * (courses.size()-1) )*Y_OFFSET_PER_BLOCK);
		 connectCourseLabels(printedLabels.find(parent->getCourseCode().description()), printedLabels.find(courses[i]->getCourseCode().description()), Qt::red);
		 for (QGraphicsProxyWidget *label: childChildrenLabels){
			 label->moveBy(0, tree_size * Y_OFFSET_PER_BLOCK/2);
		 }
	 }
	 return child_size > courses.size()?child_size: courses.size();
 }
int MainWindow::getPreRequisiteTreeSize(AVLTree<int, vector<Course*>> &map, int depth, Course* parent){
	if (!map.contains(depth)) return 0;
	vector<Course*> courses = parent->getPrerequisite().getEdges();
	int child_size = 0;/*
	for (Course *course: parent->getPrerequisite({

	}*/
	return child_size > courses.size()?child_size: courses.size();
}

