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
    CourseInfoWidget  *courseInfoWidget;
	QGraphicsScene *scene;
	QGraphicsRectItem *nodeBoxes;
	DependencyGraph<QString, Course*> *dependencyGraph = nullptr;
	Course* selectedCourse = nullptr;
	AVLTree<QString, QGraphicsProxyWidget*> printedLabels;

	void setupTreeView();
	QTreeWidgetItem* treeViewAddRoot(QString string);
	void treeViewAddChild(QTreeWidgetItem* parent, QString string);

	QGraphicsProxyWidget* addCourseLabel(QString name, qreal x, qreal y);
	void connectCourseLabels(QGraphicsProxyWidget* from , QGraphicsProxyWidget* to, Qt::GlobalColor color);
	void clearCourseLabel();
	void pushPreRequisite(Course* course);
	void pushAvailableAfter(Course* course);
	int printPreRequisite(AVLTree<int, vector<Course*>> &map, int depth = 0, Course* parent = nullptr, qreal xOffset = 0, qreal yOffset = 0);
	int printAvailableAfter(AVLTree<int, vector<Course*>> &map, int depth = 0, Course* parent = nullptr, qreal xOffset = 0, qreal yOffset = 0);
	int getPreRequisiteTreeSize(AVLTree<int, vector<Course*>> &map, int depth, Course* parent);
private slots:
    void searchCourse(const QString &text);
    void searchEnterPressed();
	void treeWidgetItemClicked();

signals:
	void treeWidgetItemClick(CourseCode*);

};

#endif // MAINWINDOW_H
