#ifndef DEPENDENCYMANAGER_H
#define DEPENDENCYMANAGER_H

#include "sources/AVLTree.tpp"
#include "sources/Course.h"

class DependencyManager {
private:

public:
	AVLTree<QString, AVLTree<CourseCode, Course>* > courses;
    DependencyManager();

    void loadCSV();
};

#endif // DEPENDENCYMANAGER_H
