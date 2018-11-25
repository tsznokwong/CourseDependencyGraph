#ifndef DEPENDENCYMANAGER_H
#define DEPENDENCYMANAGER_H

#include "sources/AVLTree.tpp"
#include "sources/Course.h"

class DependencyManager {
private:
    AVLTree<CourseCode, Course> courses;

public:
    DependencyManager();

    void loadCSV();
};

#endif // DEPENDENCYMANAGER_H
