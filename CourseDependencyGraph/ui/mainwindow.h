#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "utilities/DependencyManager.h"

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

	void setupTreeView();
	void treeViewAddRoot(QString string);
	void treeViewAddChild(QTreeWidgetItem* parent, QString string);
};

#endif // MAINWINDOW_H
