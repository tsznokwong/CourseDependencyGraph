#include "CourseInfoWidget.h"
#include "ui_CourseInfoWidget.h"

CourseInfoWidget::CourseInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseInfoWidget) {
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
