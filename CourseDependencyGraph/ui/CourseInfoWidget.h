#ifndef COURSEINFOWIDGET_H
#define COURSEINFOWIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include <QTreeWidgetItem>
#include "sources/Course.h"

namespace Ui {
class CourseInfoWidget;
}

class CourseInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseInfoWidget(QWidget *parent = nullptr,
                              const AVLTree<QString, AVLTree<CourseCode, Course* >* >* courses = nullptr,
                              const Course* const course = nullptr,
                              const QTreeWidget* const treeWidget = nullptr);
    ~CourseInfoWidget();
    void treeWidgetItemSelected(CourseCode *courseCode);

private:
    Ui::CourseInfoWidget *ui;
    const Course *course;
    const AVLTree<QString, AVLTree<CourseCode, Course* >* >* courses;
    const QTreeWidget *treeWidget;

    void setCourseInfo();

};

#endif // COURSEINFOWIDGET_H
