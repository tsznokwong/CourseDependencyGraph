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
#include "sources/DependencyGraph.tpp"
#include <QEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;
	bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    Ui::MainWindow *ui;

    DependencyManager *dependencyManager;

    // course information ui
    CourseInfoWidget  *courseInfoWidget;


    // course catalog ui
	void setupTreeView();
	QTreeWidgetItem* treeViewAddRoot(QString string);
	void treeViewAddChild(QTreeWidgetItem* parent, QString string);

    // course graph
    DependencyGraph<QString, Course*> *dependencyGraph = nullptr;
    Course* selectedCourse = nullptr;

    // course graph ui
    QGraphicsScene *scene;
    QGraphicsRectItem *nodeBoxes;
    AVLTree<QString, QGraphicsProxyWidget*> printedLabels;

    // course graph ui operations
	QGraphicsProxyWidget* addCourseLabel(QString name, qreal x, qreal y);
	void connectCourseLabels(QGraphicsProxyWidget* from , QGraphicsProxyWidget* to, Qt::GlobalColor color);
	void clearCourseLabel();
    void pushPreRequisite(CourseCode course);
    void pushAvailableAfter(CourseCode course);
	int printPreRequisite(AVLTree<int, vector<Course*>> &map, int depth = 0, Course* parent = nullptr, qreal xOffset = 0, qreal yOffset = 0);
	int printAvailableAfter(AVLTree<int, vector<Course*>> &map, int depth = 0, Course* parent = nullptr, qreal xOffset = 0, qreal yOffset = 0);
	int printCoRequisite(Course* parent);
	int printExclusion(Course* parent);

	int getPreRequisiteTreeSize(AVLTree<int, vector<Course*>> &map, int depth, Course* parent);

private slots:
    void searchCourse(const QString &text);
    void searchEnterPressed();
    void treeWidgetItemSelected();

signals:
	void treeWidgetItemClick(CourseCode*);

};

#endif // MAINWINDOW_H
