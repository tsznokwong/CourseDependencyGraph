#ifndef DEPENDENCYMANAGER_H
#define DEPENDENCYMANAGER_H

//#include "sources/AVLTree.tpp"
#include "sources/Course.h"

/**
 * @brief The DependencyManager class.
 * The manager stores and manage relationship of courses.
 */
class DependencyManager {
private:

public:
    DependencyManager();

    AVLTree<QString, AVLTree<CourseCode, Course* >* > courses;

    void loadCSV();
    void linkCourses();
	Course* findCourse(QString code);
};

#endif // DEPENDENCYMANAGER_H
