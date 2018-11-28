#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "utilities/DependencyManager.h"
#include "CourseInfoWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DependencyManager *dependencyManager;
    CourseInfoWidget  *courseInfoWidget;

	void setupTreeView();
	QTreeWidgetItem* treeViewAddRoot(QString string);
	void treeViewAddChild(QTreeWidgetItem* parent, QString string);

private slots:
    void searchCourse(const QString &text);
    void searchEnterPressed();
};

#endif // MAINWINDOW_H
