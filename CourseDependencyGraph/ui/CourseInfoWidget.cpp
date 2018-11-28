#include <QDebug>

#include "CourseInfoWidget.h"
#include "ui_CourseInfoWidget.h"

CourseInfoWidget::CourseInfoWidget(QWidget *parent,
                                   const AVLTree<QString, AVLTree<CourseCode, Course* >* >* courses,
                                   const Course* const course) :
    QWidget(parent),
    ui(new Ui::CourseInfoWidget),
    course(course),
    courses(courses) {
    ui->setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

CourseInfoWidget::~CourseInfoWidget() {
    delete ui;
}

void CourseInfoWidget::setCourseInfo() {
    if (this->course == nullptr) { return; }
    this->ui->courseCodeLabel->setText(this->course->getCourseCode().description());
}

void CourseInfoWidget::treeWidgetItemClicked(QTreeWidgetItem *item, int column) {
    QTreeWidgetItem *subject = item->parent();
    if (subject == nullptr) { return; }

    AVLTree<CourseCode, Course* >* subjectTree = this->courses->find(subject->text(0));
    if (subjectTree == nullptr) { return; }

    CourseCode *courseCode = CourseCode::create(subject->text(0) + item->text((0)));
    if (courseCode == nullptr) { return; }
    this->course = subjectTree->find(*courseCode);
    delete courseCode;

    this->setCourseInfo();
}

