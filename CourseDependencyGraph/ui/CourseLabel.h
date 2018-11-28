#ifndef COURSELABEL_H
#define COURSELABEL_H

#include <QFrame>

namespace Ui {
class CourseLabel;
}

class CourseLabel : public QFrame
{
	Q_OBJECT

public:
	explicit CourseLabel(QWidget *parent, QString name);
	~CourseLabel();

private:
	Ui::CourseLabel *ui;
	QString name;
};

#endif // COURSELABEL_H
