#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "utilities/DependencyManager.h"
#include "CourseInfoWidget.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "CourseLabel.h"
#include <QGraphicsProxyWidget>
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    Ui::MainWindow *ui;
    DependencyManager *dependencyManager;
    CourseInfoWidget  *courseInfoWidget;
	QGraphicsScene *scene;
	QGraphicsRectItem *nodeBoxes;


	void setupTreeView();
	QTreeWidgetItem* treeViewAddRoot(QString string);
	void treeViewAddChild(QTreeWidgetItem* parent, QString string);

private slots:
    void searchCourse(const QString &text);
    void searchEnterPressed();
	QGraphicsProxyWidget* addCourseLabel(QString name, qreal x, qreal y);
	void connectCourseLabels(QGraphicsProxyWidget* from , QGraphicsProxyWidget* to, Qt::GlobalColor color);

};

#endif // MAINWINDOW_H
