#include "CourseLabel.h"
#include "ui_CourseLabel.h"

CourseLabel::CourseLabel(QWidget *parent, QString name) :
	QFrame(parent),
	ui(new Ui::CourseLabel),
	name(name)
{
	ui->setupUi(this);
	ui->lineEdit->setText(name);
}

CourseLabel::~CourseLabel()
{
	delete ui;
}
