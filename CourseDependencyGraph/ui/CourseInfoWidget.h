#ifndef COURSEINFOWIDGET_H
#define COURSEINFOWIDGET_H

#include <QWidget>

namespace Ui {
class CourseInfoWidget;
}

class CourseInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseInfoWidget(QWidget *parent = nullptr);
    ~CourseInfoWidget();

private:
    Ui::CourseInfoWidget *ui;

};

#endif // COURSEINFOWIDGET_H
