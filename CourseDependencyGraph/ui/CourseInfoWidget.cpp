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

CourseInfoWidget::~CourseInfoWidget(){
    delete ui;
}

void CourseInfoWidget::treeWidgetItemClicked(QTreeWidgetItem *item, int column) {

}
