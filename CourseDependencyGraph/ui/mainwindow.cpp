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
            this, SLOT(treeWidgetItemSelected()));

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
		boundingRect.setLeft(200);
		boundingRect.setRight(-200);
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
    if (event->type() == QEvent::GraphicsSceneMousePress){
		// parsing courselabel from QObject
		QString name = static_cast<CourseLabel*>(static_cast<QGraphicsProxyWidget*>(obj)->widget())->getName();
		ui->searchBar->setText(name);
		return true;
	}
	return false;
}

void MainWindow::treeWidgetItemSelected(){
	clearCourseLabel();
	QList<QTreeWidgetItem *> items = ui->treeWidget->selectedItems();
	if (items.size() == 0) { return; }
	QTreeWidgetItem *item = items.first();
	if (item == nullptr) { return; }
	QTreeWidgetItem *subject = item->parent();
	if (subject == nullptr) { return; }

	AVLTree<CourseCode, Course* >* subjectTree = dependencyManager->courses.find(subject->text(0));
	if (subjectTree == nullptr) { return; }

    // change course information
	CourseCode *courseCode = CourseCode::create(subject->text(0) + item->text((0)));
    this->courseInfoWidget->treeWidgetItemSelected(courseCode);
	selectedCourse = dependencyManager->findCourse(courseCode->description());
	delete courseCode;

    // construct dependency graph
	addCourseLabel(selectedCourse->getCourseCode().description(), 0, 0)->widget()->setStyleSheet(".QLineEdit { background-color : green }");
	if (dependencyGraph == nullptr){
		dependencyGraph = new DependencyGraph<QString, Course*>(selectedCourse->getCourseCode().description(), selectedCourse);
	}
    else {
		dependencyGraph->reset(selectedCourse->getCourseCode().description(), selectedCourse);
    }
    pushPreRequisite(selectedCourse->getCourseCode());
    pushAvailableAfter(selectedCourse->getCourseCode());
	AVLTree<int, vector<Course*>> map;
	dependencyGraph->getNodesInMap(map);
	printExclusion(selectedCourse);
	printCoRequisite(selectedCourse);
	printPreRequisite(map, 0, selectedCourse);
	printAvailableAfter(map, 0, selectedCourse);

    // debug log of constructed graph
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

void MainWindow::pushPreRequisite(CourseCode courseCode){
    Course* course = this->dependencyManager->findCourse(courseCode.description());
    if (course == nullptr) { return; }

    for (CourseCode nextCourseCode: course->getPrerequisite().getCourseCodes()){

        Course* nextCourse = this->dependencyManager->findCourse(nextCourseCode.description());
        if (nextCourse == nullptr) {
            nextCourse = new Course { nextCourseCode };
        }
        this->dependencyGraph->addNode(nextCourseCode.description(), nextCourse);
        this->dependencyGraph->addEdge(courseCode.description(), nextCourseCode.description(), dependencyGraph->DependencyGraph::Direction::PREVIOUS, Relationship::Type::PREREQUISITE);
        pushPreRequisite(nextCourseCode);
	}
}

void MainWindow::pushAvailableAfter(CourseCode courseCode){
	Course* course = this->dependencyManager->findCourse(courseCode.description());
    if (course == nullptr) { return; }

    for (CourseCode nextCourseCode: course->getAvailableAfter().getCourseCodes()){

        Course* nextCourse = this->dependencyManager->findCourse(nextCourseCode.description());
        if (nextCourse == nullptr) {
            nextCourse = new Course { nextCourseCode };
        }
        this->dependencyGraph->addNode(nextCourseCode.description(), nextCourse);
		this->dependencyGraph->addEdge(courseCode.description(), nextCourseCode.description(), dependencyGraph->DependencyGraph::Direction::NEXT, Relationship::Type::AVAILABLE_AFTER);
        pushAvailableAfter(nextCourseCode);
	}
}

int MainWindow::printPreRequisite(AVLTree<int, vector<Course*>> &map, int depth, Course* parent,
								   qreal xOffset, qreal yOffset){
    if (parent == nullptr) { return 1; }
	int maxDepth = map.min().key;
	 qreal Y_OFFSET_PER_BLOCK = 25*pow(2,depth-maxDepth);
	 static int X_OFFSET_PER_BLOCK = 200;
	 //if (!map.contains(depth)) return 0;



     vector<CourseCode> courseCodes = parent->getPrerequisite().getCourseCodes();
     int child_size = 0;
     for (unsigned int i = 0; i < courseCodes.size(); ++i){
         int childDepth = depth - 1;
         Course* course = this->dependencyGraph->getNode(courseCodes[i].description())->getNode();
         for (int j = map.min().key; j < map.max().key; ++j){
             if (map.contains(j) && std::find(map.find(j).begin(), map.find(j).end(), course)!= map.find(j).end()){
				childDepth = j;
                qDebug() << courseCodes[i].description() << " " << childDepth;
			 }
		 }
		 vector<QGraphicsProxyWidget* > childChildrenLabels;
         addCourseLabel(courseCodes[i].description(), xOffset-((depth-childDepth)*X_OFFSET_PER_BLOCK), yOffset + (i - 0.5 * (courseCodes.size()-1) )*Y_OFFSET_PER_BLOCK);
         int tree_size = printPreRequisite(map, childDepth, course,
                                           xOffset-((depth-childDepth)*X_OFFSET_PER_BLOCK) ,  yOffset + (i - 0.5 * (courseCodes.size()-1) )*Y_OFFSET_PER_BLOCK);
		 connectCourseLabels(printedLabels.find(parent->getCourseCode().description()), printedLabels.find(courseCodes[i].description()), Qt::blue);
		 for (QGraphicsProxyWidget *label: childChildrenLabels){
			 label->moveBy(0, tree_size * Y_OFFSET_PER_BLOCK/2);
         }
	 }
     return child_size > courseCodes.size()?child_size: courseCodes.size();
 }

int MainWindow::printAvailableAfter(AVLTree<int, vector<Course*>> &map, int depth, Course* parent,
								   qreal xOffset, qreal yOffset){
	int maxDepth = map.max().key;
	 qreal Y_OFFSET_PER_BLOCK = 75;
	 static int X_OFFSET_PER_BLOCK = 200;
	 //if (!map.contains(depth)) return 0;



	 vector<CourseCode> courseCodes = parent->getAvailableAfter().getCourseCodes();
     int child_size = 0;
     for (unsigned int i = 0; i < courseCodes.size(); ++i){
         int childDepth = depth + 1;
         Course* course = this->dependencyGraph->getNode(courseCodes[i].description())->getNode();
         for (int j = map.min().key; j < map.max().key; ++j){
             if (map.contains(j) && std::find(map.find(j).begin(), map.find(j).end(), course)!= map.find(j).end()){
				childDepth = j;
                qDebug() << courseCodes[i].description() << " " << childDepth;
			 }
		 }
		 vector<QGraphicsProxyWidget* > childChildrenLabels;
         addCourseLabel(courseCodes[i].description(), xOffset+((childDepth-depth)*X_OFFSET_PER_BLOCK), yOffset + (i - 0.5 * (courseCodes.size()-1) )*Y_OFFSET_PER_BLOCK);
         int tree_size = printAvailableAfter(map, childDepth, course,
                                           xOffset+((childDepth-depth)*X_OFFSET_PER_BLOCK) ,  yOffset + (i - 0.5 * (courseCodes.size()-1) )*Y_OFFSET_PER_BLOCK);
         connectCourseLabels(printedLabels.find(parent->getCourseCode().description()), printedLabels.find(courseCodes[i].description()), Qt::green);
		 for (QGraphicsProxyWidget *label: childChildrenLabels){
			 label->moveBy(0, tree_size * Y_OFFSET_PER_BLOCK/2);
         }
	 }
     return child_size > courseCodes.size()?child_size: courseCodes.size();
 }

int MainWindow::printCoRequisite(Course* parent){
	int y_offset = 75;
	qreal Y_OFFSET_PER_BLOCK = 75;
	for (Course* course: parent->getCorequisite().getEdges()){
		connectCourseLabels(
					addCourseLabel(course->getCourseCode().description(), 0, y_offset += Y_OFFSET_PER_BLOCK),
					printedLabels.find(parent->getCourseCode().description()), Qt::black);

	}
}

int MainWindow::printExclusion(Course* parent){
	int y_offset = -75;
	qreal Y_OFFSET_PER_BLOCK = -75;
	for (Course* course: parent->getExclusion().getEdges()){
		connectCourseLabels(
					addCourseLabel(course->getCourseCode().description(), 0, y_offset += Y_OFFSET_PER_BLOCK),
					printedLabels.find(parent->getCourseCode().description()), Qt::gray);

	}
}

int MainWindow::getPreRequisiteTreeSize(AVLTree<int, vector<Course*>> &map, int depth, Course* parent){
	if (!map.contains(depth)) return 0;
	vector<Course*> courses = parent->getPrerequisite().getEdges();
	int child_size = 0;/*
	for (Course *course: parent->getPrerequisite({

	}*/
	return child_size > courses.size()?child_size: courses.size();
}

