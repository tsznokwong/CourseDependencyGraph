#ifndef COURSELABEL_H
#define COURSELABEL_H

#include <QFrame>
#include <QEvent>

namespace Ui {
class CourseLabel;
}

class CourseLabel : public QFrame
{
	Q_OBJECT

public:
	explicit CourseLabel(QWidget *parent, QString name);
	~CourseLabel();
	QString getName(){return name;}

private:
	Ui::CourseLabel *ui;
	QString name;
};

#endif // COURSELABEL_H
